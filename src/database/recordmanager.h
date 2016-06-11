#ifndef TETRIS_RECORD_MANAGER_H_
#define TETRIS_RECORD_MANAGER_H_

#include "database/sqlite3.h"
#include <string>
#include <vector>

namespace tetris {

struct Record {
  std::string player_name;
  std::string token_id;
  unsigned int score;
  std::string start_time;
  std::string end_time;
};

class RecordManager {
 public:
  RecordManager();
  virtual ~RecordManager();
  bool InitRecordManager(const std::string database);
  void UnintRecordManager();
  bool InsertRecord(const std::string player_name,
                    const std::string token_id,
                    unsigned int score,
                    const std::string start_time,
                    const std::string end_time);
  bool GetRankRecords(std::vector<Record> &records);
 private:
  bool PrecheckDatabase(const std::string database_file);
 private:
  sqlite3 *db_instance_;
  sqlite3_stmt *insert_stmt_;
  sqlite3_stmt *select_stmt_;
};

}

#endif  // TETRIS_RECORD_MANAGER_H_
