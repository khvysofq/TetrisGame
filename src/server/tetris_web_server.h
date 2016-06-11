#ifndef TETRIS_WEB_SERVER_H_
#define TETRIS_WEB_SERVER_H_

#include "mongoose/mongoose.h"
#include "tetris/tetris.h"
#include "json/json.h"
#include <string>
#include <map>
#include "database/recordmanager.h"

namespace tetris {

const char URL_METHOD_GAME[]  = "/game.php";
const char JSON_GAME_STATE[]  = "game_state";
const char JSON_DATA[]        = "view_data";
const char JSON_NEXT[]        = "next_tetrismino";
const char JSON_SCORE[]       = "score";
const char JSON_TOKEN_ID[]    = "token_id";
const char JSON_PLAY_NAME[]   = "player_name";
const char JSON_STATUS_CODE[] = "status_code";
const char JSON_START_TIME[]  = "start_time";
const char JSON_END_TIME[]    = "end_time";

const char JSON_REQ_OPER[]        = "oper";
const char JSON_REQ_PLAYER_NAME[] = "player_name";
const char JSON_REQ_RANDKEY[]     = "randkey";


const char JSON_REQ_START[]       = "start";
const char JSON_REQ_STOP[]        = "stop";
const char JSON_REQ_PAUSE[]       = "pause";
const char JSON_REQ_RESUME[]      = "resume";
const char JSON_REQ_UP[]          = "up";
const char JSON_REQ_DOWN[]        = "down";
const char JSON_REQ_LEFT[]        = "left";
const char JSON_REQ_RIGHT[]       = "right";
const char JSON_REQ_GET_STATE[]   = "get_state";

const char URL_METHOD_RANK_LIST[] = "/rank_list.php";
const char URL_METHOD_ONLINE_LIST[] = "/online_list.php";


const unsigned int REQ_PARAMETER_ERROR = 301;
const unsigned int REQ_PARAMETER_NOT_FOUND = 404;
const unsigned int REQ_PARAMETER_UNKOWN_OPER = 403;

struct TetrisServerSettings {
  std::string html_root;
  std::string bind_addr;
  std::string http_port;
  std::string database_path;
};

class HttpQueryParameter {
 public:
  HttpQueryParameter();
  virtual ~HttpQueryParameter();
  bool Parse(const char *data, std::size_t size);
  const std::string GetKey(const std::string key_name);
 private:
  std::map<std::string, std::string> key_values_;
};

class TetrisWebServer : public TetrisInterface {
 public:
  TetrisWebServer(TetrisServerSettings &tss);
  virtual ~TetrisWebServer();
  bool StartTetrisWebServer();
  void RunTetrisWebServer();
  void StopTetrisWebServer();
 private:
  static void MongooseEventHandler(struct mg_connection *nc,
                                   int ev,
                                   void *ev_data);
  void ProcessHttpRequest(mg_connection *nc,
                          http_message *http_message);
  void MongoosePollEvent();
  void ResponseJson(mg_connection *nc,
                    const Json::Value &response);
  void Response(mg_connection *nc,
                const std::string game_state,
                const std::string view_data,
                unsigned int next_tetrimino,
                unsigned int score,
                const std::string token_id,
                const std::string play_name,
                unsigned int state_core);
  void ProcessGame(mg_connection *nc, HttpQueryParameter &hqp);

  void ProcessGetRankList(mg_connection *nc);
  void ProcessGetOnlineList(mg_connection *nc);

  void StartNewGame(mg_connection *nc, HttpQueryParameter &hqp);

  void PreProcess(mg_connection *nc, HttpQueryParameter &hqp);

  const std::string RandomString(std::size_t size);

  virtual void OnTetrisGameOver(Tetris *tetris_instance);
 private:
  TetrisServerSettings &tss_;
  mg_mgr mgr_;
  mg_connection *nc_;
  mg_serve_http_opts s_http_server_opts_;

  typedef std::map<std::string, Tetris::Ptr> TetrisStorage;
  TetrisStorage tetris_storage_;
  std::mutex stroage_mutex_;

  RecordManager record_manager_;
  std::vector<Tetris::Ptr> pre_remove_tetris_;
};

}

#endif // TETRIS_WEB_SERVER_H_