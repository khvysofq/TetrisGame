#ifndef TETRIS_TETROMINO_H_
#define TETRIS_TETROMINO_H_

#include <memory>

namespace tetris {

const unsigned int TETROMINO_SIZE = 4;

struct TetrominoPoint {
  unsigned int x;
  unsigned int y;
};

class TetrominoFactory;

struct TetrominoIndex {
  TetrominoPoint tetromino_map[TETROMINO_SIZE];
  unsigned int next_index;
};

class TetrominoStanza {
 public:
  TetrominoStanza(TetrominoFactory *tf, int index);
  virtual ~TetrominoStanza();
  TetrominoStanza &operator=(const TetrominoStanza &stanza);
  TetrominoStanza &SetCenterPoint(unsigned int x, unsigned y);
  TetrominoStanza &SetCenterPoint(const TetrominoPoint &tp);
  unsigned int CenterPointX() {
    return center_point_.x;
  }
  unsigned int CenterPointY() {
    return center_point_.y;
  }
  const TetrominoPoint *ToRealyMap();
  void SetTetrominoMap(unsigned int x1, unsigned int y1,
                       unsigned int x2, unsigned int y2,
                       unsigned int x3, unsigned int y3,
                       unsigned int x4, unsigned int y4);
  void SetTetrominoMap(TetrominoPoint *tetromino_points);
  void SetNextIndex(unsigned int index) {
    index_ = index;
  }
  unsigned TetrominoStanzaIndex() {
    return index_;
  }
  TetrominoStanza &NextStanza();
  const TetrominoPoint *tetromino_map() {
    return tetromino_map_;
  }
 private:
  TetrominoPoint tetromino_map_[TETROMINO_SIZE];
  TetrominoPoint temp_map_[TETROMINO_SIZE];
  unsigned int index_;
  TetrominoPoint center_point_;
  TetrominoFactory *tf_;
};

class Tetromino {
 public:
  Tetromino(TetrominoFactory *tf, int index);
  virtual ~Tetromino();
  virtual TetrominoStanza &LeftTetromino();
  virtual TetrominoStanza &RightTetromino();
  virtual TetrominoStanza &DownTetromino();
  virtual TetrominoStanza &RotationTetromino();
  virtual void SetTetrominoMap(const TetrominoStanza &tetromino_stanza);
  TetrominoStanza &CurrentTetromino();
  const TetrominoPoint *current_teromino_points() {
    return current_teromino_stanza_.tetromino_map();
  }
 protected:
  TetrominoStanza current_teromino_stanza_;
  TetrominoStanza next_tertomino_stanza_;
};

class TetrominoFactory {
 public:
  TetrominoFactory(unsigned int rand_key);
  virtual ~TetrominoFactory();
  Tetromino *TakeTetromino(unsigned int index);
  Tetromino *NextTetromino();
  void InitTetrominoList();
  TetrominoIndex &GoToNextState(unsigned int index);
 private:
  static const unsigned int MAX_STANZA_SIZE = 19;
  TetrominoIndex tetromino_indexs_[MAX_STANZA_SIZE];
  Tetromino *next_tetromino_;
  unsigned int rand_key_;
};

};

#endif // TETRIS_TETROMINO_H_