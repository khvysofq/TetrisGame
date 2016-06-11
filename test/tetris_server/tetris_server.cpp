// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved
#define CS_NDEBUG
#include "server/tetris_web_server.h"



int main(void) {

  tetris::TetrisServerSettings tss;

  tss.bind_addr = "0.0.0.0";
  tss.http_port = "8000";
  tss.html_root = "F:/code/osc/TetrisGame/test/tetris_server/web_root";
  tss.database_path = "F:/code/osc/TetrisGame/test/tetris_server/rank_database.db";

  tetris::TetrisWebServer server(tss);

  server.StartTetrisWebServer();
  server.RunTetrisWebServer();
  server.StopTetrisWebServer();

  return EXIT_SUCCESS;
}