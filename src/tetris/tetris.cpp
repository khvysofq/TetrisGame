#include "tetris/tetris.h"
#include "glog/logging.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

namespace tetris {

#ifdef WIN32
#define TETRIS_SLEEP(x) Sleep((x))
#else
#define TETRIS_SLEEP(x) usleep((x) * 1000)
#endif

Tetris::Tetris(TetrisInterface *tetris_interface,
               unsigned int rand_key,
               const std::string play_name,
               const std::string token_id)
  :score_(0),
   tetromino_factory_(rand_key),
   tetris_status_(TETRIS_INIT),
   play_name_(play_name),
   tetris_interface_(tetris_interface),
   token_id_(token_id) {
  start_time_ = time(NULL);
}

Tetris::~Tetris() {
  tetris_thread_->detach();
  tetris_thread_.reset();
}

bool Tetris::IsGameOver() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  return tetris_status_ == TETRIS_GAME_OVER;
}

unsigned int Tetris::score() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  return score_;
}

Tetromino *Tetris::NextTetromino() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  return tetromino_factory_.NextTetromino();
}

unsigned int Tetris::TetrominoIndex() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  Tetromino *tetromino = tetromino_factory_.NextTetromino();
  return tetromino->CurrentTetromino().TetrominoStanzaIndex();
}

const std::string Tetris::tetris_map() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  return TetrisMapToString(active_map_);
}

const std::string Tetris::TetrisStatusToString(TetrisStatus status) {
  switch (status) {
  case tetris::TETRIS_INIT:
    return "init";
    break;
  case tetris::TETRIS_START:
    return "start";
    break;
  case tetris::TETRIS_RUNING:
    return "running";
    break;
  case tetris::TETRIS_PAUSE:
    return "pause";
    break;
  case tetris::TETRIS_RESUME:
    return "resume";
    break;
  case tetris::TETRIS_GAME_OVER:
    return "game_over";
    break;
  case tetris::TETRIS_ERROR:
    return "error";
    break;
  default:
    break;
  }
  return "";
}

const std::string Tetris::GetTetrisStatus() {
  return TetrisStatusToString(tetris_status_);
}

bool Tetris::Start() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);

  if(tetris_status_ != TETRIS_INIT) {
    LOG(ERROR) << "tetris_status_ != TETRIS_INIT";
  } else {
    tetris_status_ = TETRIS_START;
  }

  InitTetrisMap(tetris_map_);
  InitTetrisMap(active_map_);

  tetris_thread_.reset(new std::thread(std::bind(&Tetris::RunTetris, this)));

  return true;
}

void DumpMap(const unsigned int *tetris_map) {
  system("cls");
  std::stringstream ss;
  for(int x = 0; x < TETRIS_MAP_HEIGHT; x++) {
    for(int y = 0; y < TETRIS_MAP_WIDTH; y++) {
      // std::cout << TETRIS_INDEX(x, y) << " ";
      ss << (unsigned char)tetris_map[TETRIS_INDEX(x, y)] << " ";
    }
    ss << std::endl;
  }
  ss << "============================" << std::endl;
  //ss << "score = " << tetris_game.score() << std::endl;
  //tetris::Tetromino *tetromino = tetris_game.NextTetromino();
  //const tetris::TetrominoPoint *ctp = tetromino->current_teromino_points();
  //for(int x = 0; x < 4; x++) {
  //  ss << std::endl;
  //  for(int y = 0; y < 4; y++) {
  //    bool is_show = false;
  //    for(int i = 0; i < 4; i++) {
  //      if(ctp[i].x == x && ctp[i].y == y) {
  //        is_show = true;
  //        ss << "#";
  //        break;
  //      }
  //    }
  //    if(!is_show) {
  //      ss << " ";
  //    }
  //  }
  //}
  std::cout << ss.str();
}

const std::string Tetris::ProcessGame(TetrisOperator oper) {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  if(tetris_status_ != TETRIS_RUNING) {
    // LOG(WARNING) << "tetris_status_ != TETRIS_RUNING";
    return TetrisMapToString(tetris_map_);
  }
  // LOG(INFO) << "TETRIS_RUNING";
  if(!active_tetromino_) {
    active_tetromino_.reset(tetromino_factory_.TakeTetromino(rand() % 19));
  }
  switch (oper) {
  case tetris::TETRIS_UP:
    ProcessTetris(active_tetromino_->RotationTetromino());
    break;
  case tetris::TETRIS_DOWN:
    ProcessTetris(active_tetromino_->DownTetromino());
    break;
  case tetris::TETRIS_LEFT:
    ProcessTetris(active_tetromino_->LeftTetromino());
    break;
  case tetris::TETRIS_RIGHT:
    ProcessTetris(active_tetromino_->RightTetromino());
    break;
  case tetris::TETRIS_NONE:
    ProcessTetris(active_tetromino_->DownTetromino(), true);
    break;
  }
  // DumpMap(active_map_);
  return TetrisMapToString(active_map_);
}

const std::string Tetris::PauseGame() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  if(tetris_status_ == TETRIS_RUNING) {
    tetris_status_ = TETRIS_PAUSE;
    LOG(INFO) << "tetris_status_ = TETRIS_PAUSE";
  }
  return TetrisMapToString(tetris_map_);
}

const std::string Tetris::ResumeGame() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  if(tetris_status_ == TETRIS_PAUSE) {
    tetris_status_ = TETRIS_RUNING;
    LOG(INFO) << "tetris_status_ = TETRIS_RUNING";
  }
  return TetrisMapToString(tetris_map_);
}

const std::string Tetris::StopGame() {
  // thread mutex
  std::lock_guard<std::mutex> thead_lock(tetris_mutex_);
  if(tetris_status_ == TETRIS_RUNING || tetris_status_ == TETRIS_PAUSE) {
    tetris_status_ = TETRIS_GAME_OVER;
    LOG(INFO) << "tetris_status_ = TETRIS_GAME_OVER";
  }
  return TetrisMapToString(tetris_map_);
}

void Tetris::RunTetris() {
  static const unsigned int MAX_WAIT_PAUSE_TIME = 60;
  if(tetris_status_ == TETRIS_START) {
    tetris_status_ = TETRIS_RUNING;
    LOG(INFO) << "tetris_status_ = TETRIS_RUNING";
  } else {
    LOG(INFO) << "tetris_status is not TETRIS_START";
  }
  unsigned int wait_timeout = 0;
  while (true) {
    if(tetris_status_ == TETRIS_PAUSE) {
      wait_timeout++;
      if(wait_timeout > MAX_WAIT_PAUSE_TIME) {
        tetris_status_ = TETRIS_GAME_OVER;
      }
    } else {
      wait_timeout = 0;
    }
    ProcessGame(TETRIS_NONE);
    if(tetris_status_ == TETRIS_GAME_OVER) {
      break;
    }
    TETRIS_SLEEP(1000);
  }
  tetris_interface_->OnTetrisGameOver(this);
}

void Tetris::ProcessTetris(TetrominoStanza &next_stanza,
                           bool do_collision_detection) {

  const TetrominoPoint *next_state = next_stanza.ToRealyMap();

  for(int i = 0; i < TETROMINO_SIZE; i++) {
    if(next_state[i].y <= 0
        || next_state[i].y >= TETRIS_MAP_WIDTH) {
      return ;
    }
    if(tetris_map_[TETRIS_INDEX(next_state[i].x, next_state[i].y)]
        == TETRIS_MAP_RELAY_MASK) {
      if(do_collision_detection) {
        CollisionDetection(active_tetromino_->CurrentTetromino());
      }
      return;
    }
  }
  AddTerominoPoint(next_stanza);
}

void Tetris::AddTerominoPoint(TetrominoStanza &next_stanza) {
  const TetrominoPoint *next_state = next_stanza.ToRealyMap();
  memcpy(active_map_, tetris_map_,
         sizeof(unsigned int) * TETRIS_MAP_HEIGHT * TETRIS_MAP_WIDTH);
  if(!active_tetromino_) {
    return;
  }
  for(int i = 0; i < TETROMINO_SIZE; i++) {
    active_map_[TETRIS_INDEX(next_state[i].x, next_state[i].y)]
      = TETRIS_MAP_RELAY_MASK;
  }
  active_tetromino_->SetTetrominoMap(next_stanza);
}

void Tetris::CollisionDetection(TetrominoStanza &next_stanza) {
  const TetrominoPoint *next_state = next_stanza.ToRealyMap();
  for(int i = 0; i < TETROMINO_SIZE; i++) {
    tetris_map_[TETRIS_INDEX(next_state[i].x, next_state[i].y)]
      = TETRIS_MAP_RELAY_MASK;
  }
  score_ += ToScore(ScoringDetection());
  for(int y = 1; y < TETRIS_MAP_WIDTH - 1; y++) {
    if(tetris_map_[TETRIS_INDEX(3, y)] == TETRIS_MAP_RELAY_MASK) {
      tetris_status_ = TETRIS_GAME_OVER;
      LOG(INFO) << "tetris_status_ = TETRIS_GAME_OVER";
      break;
    }
  }
  memcpy(active_map_, tetris_map_,
         sizeof(unsigned int) * TETRIS_MAP_HEIGHT * TETRIS_MAP_WIDTH);
  active_tetromino_.reset();
}

unsigned int Tetris::ScoringDetection() {
  unsigned int scoring_size = 0;
  for(int x = TETRIS_MAP_HEIGHT - 2; x >= 2; x--) {
    bool complete = true;
    for(int y = 1; y < TETRIS_MAP_WIDTH - 1; y++) {
      if(tetris_map_[TETRIS_INDEX(x, y)]
          != TETRIS_MAP_RELAY_MASK) {
        complete = false;
        break;;
      }
      if(!complete) {
        break;
      }
    }
    if(complete) {
      scoring_size++;
      MoveDownOneLine(x);
      x++;
    }
  }
  return scoring_size;
}

void Tetris::MoveDownOneLine(int convert_line) {
  for(int x = convert_line; x >= 2; x--) {
    CopyLine(x, x - 1);
  }
}

void Tetris::CopyLine(int des_line, int src_line) {
  for(int y = 1; y < TETRIS_MAP_WIDTH - 1; y++) {
    tetris_map_[TETRIS_INDEX(des_line, y)] = tetris_map_[TETRIS_INDEX(src_line, y)];
  }
}
unsigned int Tetris::ToScore(unsigned int scoring_size) {
  if(scoring_size == 1) {
    return 1;
  } else if(scoring_size == 2) {
    return 3;
  } else if(scoring_size == 3) {
    return 6;
  } else if(scoring_size == 4) {
    return 10;
  }
  return 0;
}

const std::string Tetris::TetrisMapToString(const unsigned int *map) {
  std::string map_string;
  for(int x = 3; x < TETRIS_MAP_HEIGHT - 1; x++) {
    for(int y = 1; y < TETRIS_MAP_WIDTH - 1; y++) {
      map_string.push_back(map[TETRIS_INDEX(x, y)]);
    }
  }
  return map_string;
}

void Tetris::InitTetrisMap(unsigned int *tetris_map) {
  int size_of_map = TETRIS_MAP_HEIGHT * TETRIS_MAP_WIDTH;
  for(int i = 0; i < size_of_map; i++) {
    tetris_map[i] = TETRIS_MAP_NONE_MASK;
  }
  for(int y = 0; y < TETRIS_MAP_WIDTH; y++) {
    tetris_map[TETRIS_INDEX(TETRIS_MAP_HEIGHT - 1, y)] = TETRIS_MAP_RELAY_MASK;
  }

  for(int x= 0; x < TETRIS_MAP_HEIGHT; x++) {
    tetris_map[TETRIS_INDEX(x, 0)] = TETRIS_MAP_RELAY_MASK;
    tetris_map[TETRIS_INDEX(x, TETRIS_MAP_WIDTH - 1)] = TETRIS_MAP_RELAY_MASK;
  }
}
};
