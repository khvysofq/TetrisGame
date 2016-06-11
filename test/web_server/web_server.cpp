// Copyright (c) 2014 Cesanta Software
// All rights reserved

#include "mongoose/mongoose.h"
#include "tetris/tetris.h"
#include <windows.h>
#include <iostream>
#include <sstream>

HANDLE hStdin;
DWORD fdwSaveOldMode;

//VOID ErrorExit(LPSTR);
//VOID KeyEventProc(KEY_EVENT_RECORD);
//VOID MouseEventProc(MOUSE_EVENT_RECORD);
//VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);

void DumpMap(const unsigned int *tetris_map, tetris::Tetris &tetris_game) {
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
  ss << "score = " << tetris_game.score() << std::endl;
  tetris::Tetromino *tetromino = tetris_game.NextTetromino();
  const tetris::TetrominoPoint *ctp = tetromino->current_teromino_points();
  for(int x = 0; x < 4; x++) {
    ss << std::endl;
    for(int y = 0; y < 4; y++) {
      bool is_show = false;
      for(int i = 0; i < 4; i++) {
        if(ctp[i].x == x && ctp[i].y == y) {
          is_show = true;
          ss << "#";
          break;
        }
      }
      if(!is_show) {
        ss << " ";
      }
    }
  }
  std::cout << ss.str();
}

int main(void) {

  tetris::Tetris tetris_game((unsigned int)time(NULL), "Guangleihe");
  tetris_game.Start();
  unsigned int process = 0;
  srand((unsigned int)time(NULL));
  DWORD cNumRead, fdwMode, i;
  INPUT_RECORD irInBuf[128];
  int counter=0;

  // Get the standard input handle.

  hStdin = GetStdHandle(STD_INPUT_HANDLE);
  if (hStdin == INVALID_HANDLE_VALUE)
    return EXIT_SUCCESS;

  // Save the current input mode, to be restored on exit.

  if (! GetConsoleMode(hStdin, &fdwSaveOldMode) )
    return EXIT_SUCCESS;

  // Enable the window and mouse input events.

  fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
  if (! SetConsoleMode(hStdin, fdwMode) )
    return EXIT_SUCCESS;

  // Loop to read and handle the next 100 input events.

  while (true) {
    process++;
    if(!(process % 10)) {
      DumpMap(tetris_game.ProcessGame(tetris::TETRIS_NONE), tetris_game);
    }
    Sleep(100);
    // Wait for the events.

    if (! ReadConsoleInput(
          hStdin,      // input buffer handle
          irInBuf,     // buffer to read into
          128,         // size of read buffer
          &cNumRead) ) // number of records read
      return EXIT_SUCCESS;

    // Dispatch the events to the appropriate handler.

    for (i = 0; i < cNumRead; i++) {
      switch(irInBuf[i].EventType) {
      case KEY_EVENT: // keyboard input
        if(irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_UP
            && irInBuf[i].Event.KeyEvent.bKeyDown) {
          DumpMap(tetris_game.ProcessGame(tetris::TETRIS_UP), tetris_game);
        } else if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN
                   && irInBuf[i].Event.KeyEvent.bKeyDown) {
          DumpMap(tetris_game.ProcessGame(tetris::TETRIS_DOWN), tetris_game);
        } else if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_LEFT
                   && irInBuf[i].Event.KeyEvent.bKeyDown) {
          DumpMap(tetris_game.ProcessGame(tetris::TETRIS_LEFT), tetris_game);
        } else if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode == VK_RIGHT
                   && irInBuf[i].Event.KeyEvent.bKeyDown) {
          DumpMap(tetris_game.ProcessGame(tetris::TETRIS_RIGHT), tetris_game);
        }
        break;
      }
    }
  }

  // Restore input mode on exit.

  SetConsoleMode(hStdin, fdwSaveOldMode);

  return EXIT_SUCCESS;
}