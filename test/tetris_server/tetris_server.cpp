// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved
#define CS_NDEBUG
#include "server/tetris_web_server.h"
#include <iostream>
#include "vzflags.h"

#ifndef WIN32
#include <getopt.h>
#endif


void PrintUsage() {
  printf("\n");
  printf("\n");
  printf("  usage:\n");
  printf("\n");
  printf(
    "        -e  <logtostderr>     Defualt value is 0 (google::GLOG_INFO)\n");
  printf(
    "        -m  <miniloglevel>    Default value is 2 (google::GLOG_ERROR)\n");
  printf(
    "        -v                    Verbose Mode\n");
  printf("\n");
  printf(
    "       [-h]                   print usage\n");
  printf("\n");
}

int main(int argc, char *argv[]) {

  tetris::TetrisServerSettings tss;

  if(argc != 4) {
    std::cout << argc << std::endl;
    std::cout << "./tetris_server 8000 F:/ad/web_root F:/database.db" << std::endl;
    return EXIT_FAILURE;
  }

  tss.bind_addr = "0.0.0.0";
  tss.http_port = argv[1];
  tss.html_root = argv[2];
  tss.database_path = argv[3];

  //int opt = 0;
  //while((opt = getopt(argc, argv, "a:p:r:d:h")) != -1) {
  //  switch (opt) {
  //  case 'a':
  //    tss.bind_addr = optarg;
  //    break;
  //  case 'p':
  //    tss.http_port = atoi(optarg);
  //    break;
  //  case 'r':
  //    tss.html_root = optarg;
  //    break;
  //  case 'd':
  //    tss.database_path = optarg;
  //    break;
  //  case 'h':
  //    PrintUsage();
  //    exit(EXIT_SUCCESS);
  //  }
  //}
  
  std::cout << tss.bind_addr << std::endl;
  std::cout << tss.http_port << std::endl;
  std::cout << tss.html_root << std::endl;
  std::cout << tss.database_path << std::endl;

  tetris::TetrisWebServer server(tss);

  server.StartTetrisWebServer();
  server.RunTetrisWebServer();
  server.StopTetrisWebServer();

  return EXIT_SUCCESS;
}