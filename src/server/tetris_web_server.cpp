
#include "server/tetris_web_server.h"
#include "glog/logging.h"
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#include <iconv.h>
#endif

namespace tetris {

#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp) {
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;
  GetLocalTime(&wtm);
  tm.tm_year     = wtm.wYear - 1900;
  tm.tm_mon     = wtm.wMonth - 1;
  tm.tm_mday     = wtm.wDay;
  tm.tm_hour     = wtm.wHour;
  tm.tm_min     = wtm.wMinute;
  tm.tm_sec     = wtm.wSecond;
  tm.tm_isdst    = -1;
  clock = mktime(&tm);
  tp->tv_sec = (unsigned long)clock;
  tp->tv_usec = wtm.wMilliseconds * 1000;
  return (0);
}
#endif

const std::string GetVzTime(time_t convert_time) {
  struct tm *local_time = localtime(&convert_time);
  std::stringstream ss;
  ss << local_time->tm_year + 1900 << "-"
     << local_time->tm_mon + 1 << "-"
     << local_time->tm_mday << " "
     << local_time->tm_hour << ":"
     << local_time->tm_min  << ":"
     << local_time->tm_sec;
  return ss.str();
}
// -----------------------------------------------------------------------------

HttpQueryParameter::HttpQueryParameter() {
}

HttpQueryParameter::~HttpQueryParameter() {
}

bool HttpQueryParameter::Parse(const char *data, std::size_t size) {
  std::string key;
  std::string value;
  std::size_t index = 0;
  while (index < size) {
    for(index; index < size; index++) {
      if(data[index] == '=') {
        index++;
        break;
      }
      key.push_back(data[index]);
    }
    for(index; index < size; index++) {
      if(data[index] == '&') {
        index++;
        break;
      }
      value.push_back(data[index]);
    }
    if(!key.empty() && !value.empty()) {
      key_values_.insert(std::pair<std::string, std::string>(key, value));
      key.clear();
      value.clear();
    }
  }
  return true;
}

const std::string HttpQueryParameter::GetKey(const std::string key_name) {
  std::map<std::string, std::string>::iterator iter = key_values_.find(key_name);
  if(iter != key_values_.end()) {
    return iter->second;
  }
  return std::string();
}
// -----------------------------------------------------------------------------
TetrisWebServer::TetrisWebServer(TetrisServerSettings &tss)
  : tss_(tss) {
}

TetrisWebServer::~TetrisWebServer() {
}

bool TetrisWebServer::StartTetrisWebServer() {

  memset(&s_http_server_opts_, 0, sizeof(mg_serve_http_opts));

  srand((unsigned int)time(NULL));

  mg_mgr_init(&mgr_, NULL);
  nc_ = mg_bind(&mgr_,
                tss_.http_port.c_str(),
                &TetrisWebServer::MongooseEventHandler);
  nc_->user_data = (void *)this;
  mg_set_protocol_http_websocket(nc_);
  s_http_server_opts_.document_root = tss_.html_root.c_str();

  record_manager_.InitRecordManager(tss_.database_path);

  return true;
}

void TetrisWebServer::RunTetrisWebServer() {
  std::cout << "Starting web server on port " <<  tss_.http_port << std::endl;
  for (;;) {
    mg_mgr_poll(&mgr_, 1000);
  }
}

void TetrisWebServer::StopTetrisWebServer() {
  mg_mgr_free(&mgr_);
  record_manager_.UnintRecordManager();
}

void TetrisWebServer::MongooseEventHandler(
  mg_connection *nc, int ev, void *ev_data) {
  // LOG(INFO) << ev;
  switch (ev) {
  case MG_EV_HTTP_REQUEST:
    ((TetrisWebServer *)nc->user_data)->ProcessHttpRequest(nc,
        (http_message *)(ev_data));
    break;
  case MG_EV_POLL:
    // Safe to delete the tetris instance
    ((TetrisWebServer *)nc->user_data)->MongoosePollEvent();
    break;
  default:
    break;
  }
}

void TetrisWebServer::ProcessHttpRequest(mg_connection *nc,
    http_message *http_message) {

  std::string url_method(http_message->uri.p, http_message->uri.len);
  if(url_method == URL_METHOD_GAME) {
    HttpQueryParameter hqp;
    hqp.Parse(http_message->query_string.p, http_message->query_string.len);
    ProcessGame(nc, hqp);
  } else if(url_method == URL_METHOD_RANK_LIST) {
    ProcessGetRankList(nc);
  } else if(url_method == URL_METHOD_ONLINE_LIST) {
    ProcessGetOnlineList(nc);
  } else {
    mg_serve_http(nc, http_message, s_http_server_opts_);
  }
}

void TetrisWebServer::MongoosePollEvent() {

  if(pre_remove_tetris_.size() == 0) {
    return;
  }

  for(std::size_t i = 0; i < pre_remove_tetris_.size(); i++) {
    std::string start_time = GetVzTime(pre_remove_tetris_[i]->start_time());
    std::string end_time   = GetVzTime(time(NULL));
    record_manager_.InsertRecord(pre_remove_tetris_[i]->play_name(),
                                 pre_remove_tetris_[i]->TokenId(),
                                 pre_remove_tetris_[i]->score(),
                                 start_time,
                                 end_time);
  }
  pre_remove_tetris_.clear();
}

void TetrisWebServer::ResponseJson(mg_connection *nc,
                                   const Json::Value &response) {
  Json::FastWriter fw;
  std::string str = fw.write(response);
  mg_printf(nc,
            "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n"
            "Connection: keep-alive\r\n"
            "Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0\r\n"
            "Content-Type: application/json; charset=utf-8\r\n\r\n%s",
            str.size(), str.c_str());
  // nc->flags |= MG_F_SEND_AND_CLOSE;
}

void TetrisWebServer::ProcessGame(mg_connection *nc, HttpQueryParameter &hqp) {
  std::string oper = hqp.GetKey(JSON_REQ_OPER);
  if(oper == JSON_REQ_START) {
    StartNewGame(nc, hqp);
  } else {
    PreProcess(nc, hqp);
  }
}


void TetrisWebServer::ProcessGetRankList(mg_connection *nc) {
  Json::Value value(Json::arrayValue);
  std::vector<Record> records;
  record_manager_.GetRankRecords(records);
  for(std::size_t i = 0; i < records.size(); i++) {
    Json::Value item;
    item[JSON_PLAY_NAME]  = records[i].player_name;
    item[JSON_TOKEN_ID]   = records[i].token_id;
    item[JSON_SCORE]      = records[i].score;
    item[JSON_START_TIME] = records[i].start_time;
    item[JSON_END_TIME]   = records[i].end_time;
    value.append(item);
  }
  ResponseJson(nc, value);
}

void TetrisWebServer::ProcessGetOnlineList(mg_connection *nc) {
  Json::Value value(Json::arrayValue);
  std::vector<Record> records;
  record_manager_.GetRankRecords(records);
  for(TetrisStorage::iterator iter = tetris_storage_.begin();
      iter != tetris_storage_.end(); iter++) {
    Tetris::Ptr tetris_instance = iter->second;
    Json::Value item;
    item[JSON_PLAY_NAME]  = tetris_instance->play_name();
    item[JSON_TOKEN_ID]   = tetris_instance->TokenId();
    item[JSON_SCORE]      = tetris_instance->score();
    item[JSON_START_TIME] = GetVzTime(tetris_instance->start_time());
    item[JSON_GAME_STATE]   = tetris_instance->GetTetrisStatus();
    value.append(item);
  }
  ResponseJson(nc, value);

}

void TetrisWebServer::Response(mg_connection *nc,
                               const std::string game_state,
                               const std::string view_data,
                               unsigned int next_tetrimino,
                               unsigned int score,
                               const std::string token_id,
                               const std::string play_name,
                               unsigned int state_core) {
  Json::Value response;
  response[JSON_GAME_STATE]   = game_state;
  response[JSON_DATA]         = view_data;
  response[JSON_NEXT]         = next_tetrimino;
  response[JSON_SCORE]        = score;
  response[JSON_TOKEN_ID]     = token_id;
  response[JSON_PLAY_NAME]    = play_name;
  response[JSON_STATUS_CODE]  = state_core;
  ResponseJson(nc, response);
}

void TetrisWebServer::StartNewGame(mg_connection *nc, HttpQueryParameter &hqp) {
  std::string player_name = hqp.GetKey(JSON_REQ_PLAYER_NAME);
  std::string randkey     = hqp.GetKey(JSON_REQ_RANDKEY);
  if(player_name.empty() || randkey.empty()) {
    Response(nc, Tetris::TetrisStatusToString(TETRIS_ERROR),
             "", 0, 0, "", "", REQ_PARAMETER_ERROR);
    return;
  }

  const std::string token_id = player_name + RandomString(8);
  Tetris::Ptr tetris_instance(new Tetris(this,
                                         (unsigned int)(time(NULL)),
                                         player_name,
                                         token_id));
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(stroage_mutex_);
  tetris_storage_.insert(std::pair<std::string, Tetris::Ptr>(token_id,
                         tetris_instance));
  tetris_instance->Start();
  Response(nc,
           tetris_instance->GetTetrisStatus(),
           tetris_instance->tetris_map(),
           tetris_instance->TetrominoIndex(),
           tetris_instance->score(),
           token_id,
           tetris_instance->play_name(),
           200);
}

void TetrisWebServer::OnTetrisGameOver(Tetris *tetris_instance) {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(stroage_mutex_);
  TetrisStorage::iterator iter = tetris_storage_.find(tetris_instance->TokenId());
  if(iter != tetris_storage_.end()) {
    pre_remove_tetris_.push_back(iter->second);
    LOG(INFO) << "Remove a tetris game " << tetris_instance->TokenId();
    tetris_storage_.erase(iter);
  }
}

void TetrisWebServer::PreProcess(mg_connection *nc, HttpQueryParameter &hqp) {
  std::string token_id = hqp.GetKey(JSON_TOKEN_ID);
  std::string oper     = hqp.GetKey(JSON_REQ_OPER);
  if(token_id.empty() || oper.empty()) {
    Response(nc, Tetris::TetrisStatusToString(TETRIS_ERROR),
             "", 0, 0, "", "", REQ_PARAMETER_ERROR);
    return;
  }
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(stroage_mutex_);
  TetrisStorage::iterator iter = tetris_storage_.find(token_id);
  if(iter == tetris_storage_.end()) {
    Response(nc, Tetris::TetrisStatusToString(TETRIS_GAME_OVER),
             "", 0, 0, "", "", REQ_PARAMETER_NOT_FOUND);
    return;
  }
  Tetris::Ptr tetris_instance = iter->second;
  if(oper == JSON_REQ_STOP) {
    tetris_instance->StopGame();
  } else if (oper == JSON_REQ_PAUSE) {
    tetris_instance->PauseGame();
  } else if (oper == JSON_REQ_RESUME) {
    tetris_instance->ResumeGame();
  } else if (oper == JSON_REQ_UP) {
    tetris_instance->ProcessGame(TETRIS_UP);
  } else if (oper == JSON_REQ_DOWN) {
    tetris_instance->ProcessGame(TETRIS_DOWN);
  } else if (oper == JSON_REQ_LEFT) {
    tetris_instance->ProcessGame(TETRIS_LEFT);
  } else if (oper == JSON_REQ_RIGHT) {
    tetris_instance->ProcessGame(TETRIS_RIGHT);
  }
  Response(nc,
           tetris_instance->GetTetrisStatus(),
           tetris_instance->tetris_map(),
           tetris_instance->TetrominoIndex(),
           tetris_instance->score(),
           token_id,
           tetris_instance->play_name(),
           200);
}

const std::string TetrisWebServer::RandomString(std::size_t size) {
  const char SN_SET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const int MAX_SN_SET_SIZE = 36;
  std::string rand_str;
  for (std::size_t i = 0; i < size; i++) {
    rand_str.push_back(SN_SET[rand() % MAX_SN_SET_SIZE]);
  }
  return rand_str;
}
}