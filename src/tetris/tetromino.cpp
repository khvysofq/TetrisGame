#include <string>
#include "tetris/tetromino.h"

namespace tetris {


TetrominoFactory::TetrominoFactory(unsigned int rand_key)
  : rand_key_(rand_key) {
  srand(rand_key_);
  InitTetrominoList();
  next_tetromino_ = new Tetromino(this, rand() % MAX_STANZA_SIZE);
}

TetrominoFactory::~TetrominoFactory() {
}

Tetromino *TetrominoFactory::TakeTetromino(unsigned int index) {
  Tetromino *res_tetromino = next_tetromino_;
  next_tetromino_ = new Tetromino(this, rand() % MAX_STANZA_SIZE);
  return res_tetromino;
}

Tetromino *TetrominoFactory::NextTetromino() {
  return next_tetromino_;
}

void TetrominoFactory::InitTetrominoList() {
  // Z 1
  tetromino_indexs_[0].next_index = 1;
  tetromino_indexs_[0].tetromino_map[0].x = 1;
  tetromino_indexs_[0].tetromino_map[0].y = 0;
  tetromino_indexs_[0].tetromino_map[1].x = 1;
  tetromino_indexs_[0].tetromino_map[1].y = 1;
  tetromino_indexs_[0].tetromino_map[2].x = 2;
  tetromino_indexs_[0].tetromino_map[2].y = 1;
  tetromino_indexs_[0].tetromino_map[3].x = 2;
  tetromino_indexs_[0].tetromino_map[3].y = 2;
  // Z 2
  tetromino_indexs_[1].next_index = 0;
  tetromino_indexs_[1].tetromino_map[0].x = 0;
  tetromino_indexs_[1].tetromino_map[0].y = 2;
  tetromino_indexs_[1].tetromino_map[1].x = 1;
  tetromino_indexs_[1].tetromino_map[1].y = 1;
  tetromino_indexs_[1].tetromino_map[2].x = 1;
  tetromino_indexs_[1].tetromino_map[2].y = 2;
  tetromino_indexs_[1].tetromino_map[3].x = 2;
  tetromino_indexs_[1].tetromino_map[3].y = 1;
  // S 1
  tetromino_indexs_[2].next_index = 3;
  tetromino_indexs_[2].tetromino_map[0].x = 2;
  tetromino_indexs_[2].tetromino_map[0].y = 0;
  tetromino_indexs_[2].tetromino_map[1].x = 1;
  tetromino_indexs_[2].tetromino_map[1].y = 1;
  tetromino_indexs_[2].tetromino_map[2].x = 2;
  tetromino_indexs_[2].tetromino_map[2].y = 1;
  tetromino_indexs_[2].tetromino_map[3].x = 1;
  tetromino_indexs_[2].tetromino_map[3].y = 2;
  // S 2
  tetromino_indexs_[3].next_index = 2;
  tetromino_indexs_[3].tetromino_map[0].x = 0;
  tetromino_indexs_[3].tetromino_map[0].y = 0;
  tetromino_indexs_[3].tetromino_map[1].x = 1;
  tetromino_indexs_[3].tetromino_map[1].y = 0;
  tetromino_indexs_[3].tetromino_map[2].x = 1;
  tetromino_indexs_[3].tetromino_map[2].y = 1;
  tetromino_indexs_[3].tetromino_map[3].x = 2;
  tetromino_indexs_[3].tetromino_map[3].y = 1;
  // I 1
  tetromino_indexs_[4].next_index = 5;
  tetromino_indexs_[4].tetromino_map[0].x = 1;
  tetromino_indexs_[4].tetromino_map[0].y = 0;
  tetromino_indexs_[4].tetromino_map[1].x = 1;
  tetromino_indexs_[4].tetromino_map[1].y = 1;
  tetromino_indexs_[4].tetromino_map[2].x = 1;
  tetromino_indexs_[4].tetromino_map[2].y = 2;
  tetromino_indexs_[4].tetromino_map[3].x = 1;
  tetromino_indexs_[4].tetromino_map[3].y = 3;
  // I 2
  tetromino_indexs_[5].next_index = 4;
  tetromino_indexs_[5].tetromino_map[0].x = 0;
  tetromino_indexs_[5].tetromino_map[0].y = 2;
  tetromino_indexs_[5].tetromino_map[1].x = 1;
  tetromino_indexs_[5].tetromino_map[1].y = 2;
  tetromino_indexs_[5].tetromino_map[2].x = 2;
  tetromino_indexs_[5].tetromino_map[2].y = 2;
  tetromino_indexs_[5].tetromino_map[3].x = 3;
  tetromino_indexs_[5].tetromino_map[3].y = 2;
  // T 1
  tetromino_indexs_[6].next_index = 7;
  tetromino_indexs_[6].tetromino_map[0].x = 1;
  tetromino_indexs_[6].tetromino_map[0].y = 0;
  tetromino_indexs_[6].tetromino_map[1].x = 1;
  tetromino_indexs_[6].tetromino_map[1].y = 1;
  tetromino_indexs_[6].tetromino_map[2].x = 1;
  tetromino_indexs_[6].tetromino_map[2].y = 2;
  tetromino_indexs_[6].tetromino_map[3].x = 2;
  tetromino_indexs_[6].tetromino_map[3].y = 1;
  // T 2
  tetromino_indexs_[7].next_index = 8;
  tetromino_indexs_[7].tetromino_map[0].x = 1;
  tetromino_indexs_[7].tetromino_map[0].y = 0;
  tetromino_indexs_[7].tetromino_map[1].x = 0;
  tetromino_indexs_[7].tetromino_map[1].y = 1;
  tetromino_indexs_[7].tetromino_map[2].x = 1;
  tetromino_indexs_[7].tetromino_map[2].y = 1;
  tetromino_indexs_[7].tetromino_map[3].x = 2;
  tetromino_indexs_[7].tetromino_map[3].y = 1;
  // T 3
  tetromino_indexs_[8].next_index = 9;
  tetromino_indexs_[8].tetromino_map[0].x = 1;
  tetromino_indexs_[8].tetromino_map[0].y = 1;
  tetromino_indexs_[8].tetromino_map[1].x = 2;
  tetromino_indexs_[8].tetromino_map[1].y = 0;
  tetromino_indexs_[8].tetromino_map[2].x = 2;
  tetromino_indexs_[8].tetromino_map[2].y = 1;
  tetromino_indexs_[8].tetromino_map[3].x = 2;
  tetromino_indexs_[8].tetromino_map[3].y = 2;
  // T 4
  tetromino_indexs_[9].next_index = 6;
  tetromino_indexs_[9].tetromino_map[0].x = 0;
  tetromino_indexs_[9].tetromino_map[0].y = 1;
  tetromino_indexs_[9].tetromino_map[1].x = 1;
  tetromino_indexs_[9].tetromino_map[1].y = 1;
  tetromino_indexs_[9].tetromino_map[2].x = 2;
  tetromino_indexs_[9].tetromino_map[2].y = 1;
  tetromino_indexs_[9].tetromino_map[3].x = 1;
  tetromino_indexs_[9].tetromino_map[3].y = 2;
  // L 1
  tetromino_indexs_[10].next_index = 11;
  tetromino_indexs_[10].tetromino_map[0].x = 1;
  tetromino_indexs_[10].tetromino_map[0].y = 0;
  tetromino_indexs_[10].tetromino_map[1].x = 2;
  tetromino_indexs_[10].tetromino_map[1].y = 0;
  tetromino_indexs_[10].tetromino_map[2].x = 1;
  tetromino_indexs_[10].tetromino_map[2].y = 1;
  tetromino_indexs_[10].tetromino_map[3].x = 1;
  tetromino_indexs_[10].tetromino_map[3].y = 2;
  // L 2
  tetromino_indexs_[11].next_index = 12;
  tetromino_indexs_[11].tetromino_map[0].x = 0;
  tetromino_indexs_[11].tetromino_map[0].y = 0;
  tetromino_indexs_[11].tetromino_map[1].x = 0;
  tetromino_indexs_[11].tetromino_map[1].y = 1;
  tetromino_indexs_[11].tetromino_map[2].x = 1;
  tetromino_indexs_[11].tetromino_map[2].y = 1;
  tetromino_indexs_[11].tetromino_map[3].x = 2;
  tetromino_indexs_[11].tetromino_map[3].y = 1;
  // L 3
  tetromino_indexs_[12].next_index = 13;
  tetromino_indexs_[12].tetromino_map[0].x = 2;
  tetromino_indexs_[12].tetromino_map[0].y = 0;
  tetromino_indexs_[12].tetromino_map[1].x = 2;
  tetromino_indexs_[12].tetromino_map[1].y = 1;
  tetromino_indexs_[12].tetromino_map[2].x = 1;
  tetromino_indexs_[12].tetromino_map[2].y = 2;
  tetromino_indexs_[12].tetromino_map[3].x = 2;
  tetromino_indexs_[12].tetromino_map[3].y = 2;
  // L 4
  tetromino_indexs_[13].next_index = 10;
  tetromino_indexs_[13].tetromino_map[0].x = 0;
  tetromino_indexs_[13].tetromino_map[0].y = 1;
  tetromino_indexs_[13].tetromino_map[1].x = 1;
  tetromino_indexs_[13].tetromino_map[1].y = 1;
  tetromino_indexs_[13].tetromino_map[2].x = 2;
  tetromino_indexs_[13].tetromino_map[2].y = 1;
  tetromino_indexs_[13].tetromino_map[3].x = 2;
  tetromino_indexs_[13].tetromino_map[3].y = 2;
  // 7 1
  tetromino_indexs_[14].next_index = 15;
  tetromino_indexs_[14].tetromino_map[0].x = 1;
  tetromino_indexs_[14].tetromino_map[0].y = 0;
  tetromino_indexs_[14].tetromino_map[1].x = 1;
  tetromino_indexs_[14].tetromino_map[1].y = 1;
  tetromino_indexs_[14].tetromino_map[2].x = 1;
  tetromino_indexs_[14].tetromino_map[2].y = 2;
  tetromino_indexs_[14].tetromino_map[3].x = 2;
  tetromino_indexs_[14].tetromino_map[3].y = 2;
  // 7 2
  tetromino_indexs_[15].next_index = 16;
  tetromino_indexs_[15].tetromino_map[0].x = 0;
  tetromino_indexs_[15].tetromino_map[0].y = 1;
  tetromino_indexs_[15].tetromino_map[1].x = 1;
  tetromino_indexs_[15].tetromino_map[1].y = 1;
  tetromino_indexs_[15].tetromino_map[2].x = 2;
  tetromino_indexs_[15].tetromino_map[2].y = 1;
  tetromino_indexs_[15].tetromino_map[3].x = 2;
  tetromino_indexs_[15].tetromino_map[3].y = 0;
  // 7 3
  tetromino_indexs_[16].next_index = 17;
  tetromino_indexs_[16].tetromino_map[0].x = 1;
  tetromino_indexs_[16].tetromino_map[0].y = 0;
  tetromino_indexs_[16].tetromino_map[1].x = 2;
  tetromino_indexs_[16].tetromino_map[1].y = 0;
  tetromino_indexs_[16].tetromino_map[2].x = 2;
  tetromino_indexs_[16].tetromino_map[2].y = 1;
  tetromino_indexs_[16].tetromino_map[3].x = 2;
  tetromino_indexs_[16].tetromino_map[3].y = 2;
  // 7 4
  tetromino_indexs_[17].next_index = 14;
  tetromino_indexs_[17].tetromino_map[0].x = 0;
  tetromino_indexs_[17].tetromino_map[0].y = 1;
  tetromino_indexs_[17].tetromino_map[1].x = 0;
  tetromino_indexs_[17].tetromino_map[1].y = 2;
  tetromino_indexs_[17].tetromino_map[2].x = 1;
  tetromino_indexs_[17].tetromino_map[2].y = 1;
  tetromino_indexs_[17].tetromino_map[3].x = 2;
  tetromino_indexs_[17].tetromino_map[3].y = 1;
  // + 0
  tetromino_indexs_[18].next_index = 18;
  tetromino_indexs_[18].tetromino_map[0].x = 0;
  tetromino_indexs_[18].tetromino_map[0].y = 0;
  tetromino_indexs_[18].tetromino_map[1].x = 1;
  tetromino_indexs_[18].tetromino_map[1].y = 0;
  tetromino_indexs_[18].tetromino_map[2].x = 1;
  tetromino_indexs_[18].tetromino_map[2].y = 1;
  tetromino_indexs_[18].tetromino_map[3].x = 0;
  tetromino_indexs_[18].tetromino_map[3].y = 1;
}

TetrominoIndex &TetrominoFactory::GoToNextState(unsigned int index) {
  return tetromino_indexs_[index];
}
////////////////////////////////////////////////////////////////////////////////

TetrominoStanza::TetrominoStanza(TetrominoFactory *tf, int index)
  : tf_(tf) {
  TetrominoIndex &tis = tf->GoToNextState(index);
  SetTetrominoMap(tis.tetromino_map);
  SetNextIndex(tis.next_index);
  center_point_.x = 0;
  center_point_.y = 6;
}

TetrominoStanza::~TetrominoStanza() {
}

const TetrominoPoint *TetrominoStanza::ToRealyMap() {
  for(int i = 0; i < TETROMINO_SIZE; i++) {
    temp_map_[i].x = tetromino_map_[i].x + center_point_.x;
    temp_map_[i].y = tetromino_map_[i].y + center_point_.y;
  }
  return temp_map_;
}

void TetrominoStanza::SetTetrominoMap(TetrominoPoint *tetromino_points) {
  memcpy(this->tetromino_map_, tetromino_points,
         sizeof(TetrominoPoint) * TETROMINO_SIZE);
}

TetrominoStanza &TetrominoStanza::NextStanza() {
  TetrominoIndex &tis = tf_->GoToNextState(index_);
  SetTetrominoMap(tis.tetromino_map);
  SetNextIndex(tis.next_index);
  return *this;
}

void TetrominoStanza::SetTetrominoMap(unsigned int x1, unsigned int y1,
                                      unsigned int x2, unsigned int y2,
                                      unsigned int x3, unsigned int y3,
                                      unsigned int x4, unsigned int y4) {
  tetromino_map_[0].x = x1;
  tetromino_map_[0].y = y1;
  tetromino_map_[1].x = x2;
  tetromino_map_[1].y = y2;
  tetromino_map_[2].x = x3;
  tetromino_map_[2].y = y3;
  tetromino_map_[3].x = x4;
  tetromino_map_[3].y = y4;
}

TetrominoStanza &TetrominoStanza::operator=(const TetrominoStanza &stanza) {
  memcpy(this->tetromino_map_, stanza.tetromino_map_,
         sizeof(TetrominoPoint) * TETROMINO_SIZE);
  this->index_ = stanza.index_;
  this->center_point_.x = stanza.center_point_.x;
  this->center_point_.y = stanza.center_point_.y;
  return *this;
}

TetrominoStanza &TetrominoStanza::SetCenterPoint(unsigned int x, unsigned y) {
  center_point_.x = x;
  center_point_.y = y;
  return *this;
}

TetrominoStanza &TetrominoStanza::SetCenterPoint(const TetrominoPoint &tp) {
  center_point_.x = tp.x;
  center_point_.y = tp.y;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
Tetromino::Tetromino(TetrominoFactory *tf, int index)
  : current_teromino_stanza_(tf, index),
    next_tertomino_stanza_(tf, index) {
}

Tetromino::~Tetromino() {
}

void Tetromino::SetTetrominoMap(const TetrominoStanza &tetromino_stanza) {
  current_teromino_stanza_ = tetromino_stanza;
}

TetrominoStanza &Tetromino::CurrentTetromino() {
  return current_teromino_stanza_;
}

////////////////////////////////////////////////////////////////////////////////
TetrominoStanza &Tetromino::LeftTetromino() {
  next_tertomino_stanza_ = current_teromino_stanza_;
  next_tertomino_stanza_.SetCenterPoint(
    current_teromino_stanza_.CenterPointX(),
    current_teromino_stanza_.CenterPointY() - 1
  );
  return next_tertomino_stanza_;
}

TetrominoStanza &Tetromino::RightTetromino() {
  next_tertomino_stanza_ = current_teromino_stanza_;
  next_tertomino_stanza_.SetCenterPoint(
    current_teromino_stanza_.CenterPointX(),
    current_teromino_stanza_.CenterPointY() + 1
  );
  return next_tertomino_stanza_;
}

TetrominoStanza &Tetromino::DownTetromino() {
  next_tertomino_stanza_ = current_teromino_stanza_;
  next_tertomino_stanza_.SetCenterPoint(
    current_teromino_stanza_.CenterPointX() + 1,
    current_teromino_stanza_.CenterPointY()
  );
  return next_tertomino_stanza_;
}

TetrominoStanza &Tetromino::RotationTetromino() {
  next_tertomino_stanza_ = current_teromino_stanza_;
  return next_tertomino_stanza_.NextStanza();
}

};