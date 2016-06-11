#ifndef TETRIS_TETRIS_GAME_H_
#define TETRIS_TETRIS_GAME_H_

#include "tetris/tetromino.h"
#include <memory>
#include<thread>
#include<mutex>

const unsigned int TETRIS_MAP_HEIGHT      = 24;
const unsigned int TETRIS_MAP_WIDTH       = 14;
const unsigned int TETRIS_MAP_NONE_MASK   = ' ';
const unsigned int TETRIS_MAP_RELAY_MASK  = '#';

#define TETRIS_INDEX(x, y) (((x) * TETRIS_MAP_WIDTH) + (y))

namespace tetris {

enum TetrisOperator {
  TETRIS_UP,
  TETRIS_DOWN,
  TETRIS_LEFT,
  TETRIS_RIGHT,
  TETRIS_NONE
};

enum TetrisStatus {
  TETRIS_INIT,
  TETRIS_START,
  TETRIS_RUNING,
  TETRIS_PAUSE,
  TETRIS_RESUME,
  TETRIS_GAME_OVER,
  TETRIS_ERROR
};

class Tetris;

class TetrisInterface {
 public:
  virtual void OnTetrisGameOver(Tetris *tetris_instance) = 0;
};

class Tetris {
 public:

  typedef std::shared_ptr<Tetris> Ptr;

  Tetris(TetrisInterface *tetris_interface,
         unsigned int rand_key,
         const std::string play_name,
         const std::string token_id);

  virtual ~Tetris();

  bool Start();

  const std::string ProcessGame(TetrisOperator oper);

  const std::string PauseGame();

  const std::string ResumeGame();

  const std::string StopGame();

  bool IsGameOver();
  unsigned int score();
  Tetromino *NextTetromino();
  unsigned int TetrominoIndex();
  const std::string tetris_map();

  const std::string play_name() {
    return play_name_;
  }

  const std::string GetTetrisStatus();
  static const std::string TetrisStatusToString(TetrisStatus status);

  const std::string TokenId() {
    return token_id_;
  }
  time_t start_time() {
    return start_time_;
  }
 private:
  void RunTetris();

  void ProcessTetris(TetrominoStanza &next_stanza,
                     bool do_collision_detection= false);

  void AddTerominoPoint(TetrominoStanza &next_stanza);

  void CollisionDetection(TetrominoStanza &next_stanza);

  unsigned int ScoringDetection();

  void MoveDownOneLine(int convert_line);

  void CopyLine(int des_line, int src_line);

  unsigned int ToScore(unsigned int scoring_size);

  const std::string TetrisMapToString(const unsigned int *map);

  void InitTetrisMap(unsigned int *tetris_map);
 private:
  unsigned int tetris_map_[TETRIS_MAP_HEIGHT * TETRIS_MAP_WIDTH];
  unsigned int active_map_[TETRIS_MAP_HEIGHT * TETRIS_MAP_WIDTH];
  TetrominoFactory tetromino_factory_;
  std::shared_ptr<Tetromino> active_tetromino_;
  unsigned int score_;
  TetrisStatus tetris_status_;
  std::mutex tetris_mutex_;
  std::shared_ptr<std::thread> tetris_thread_;
  std::string play_name_;
  TetrisInterface *tetris_interface_;
  std::string token_id_;
  time_t start_time_;
};

};

#endif // TETRIS_TETRIS_GAME_H_