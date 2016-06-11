#include "database/recordmanager.h"
#include "glog/logging.h"

namespace tetris {

const char CREATE_DATABASE[] = "CREATE TABLE [rank_records] ("
                               "[id] INTEGER NOT NULL ON CONFLICT IGNORE PRIMARY KEY ON CONFLICT ABORT AUTOINCREMENT, "
                               "[player_name] CHAR(64) NOT NULL ON CONFLICT ABORT, "
                               "[token_id] CHAR(64) NOT NULL ON CONFLICT ABORT, "
                               "[score] INTEGER NOT NULL DEFAULT 0, "
                               "[start_time] CHAR(32), "
                               "[end_time] CHAR(32));";

const char INSERT_RECORD[] = "INSERT INTO rank_records(player_name, token_id, score, start_time, end_time) "
                             "values(?, ?, ?, ?, ?);";

const char SELECT_RECORDS[] = "SELECT * FROM rank_records ORDER BY rank_records.[score] DESC LIMIT 32;";

RecordManager::RecordManager() {
}

RecordManager::~RecordManager() {
}

bool RecordManager::InitRecordManager(const std::string database) {

  PrecheckDatabase(database);

  int res = sqlite3_open(database.c_str(), &db_instance_);
  if(res) {
    LOG(ERROR) << "Failure to open the database";
    sqlite3_close(db_instance_);
    return false;
  }

  res = sqlite3_prepare_v2(db_instance_,
                           INSERT_RECORD,
                           strlen(INSERT_RECORD),
                           &insert_stmt_,
                           0);
  if(res != SQLITE_OK) {
    LOG(ERROR) << INSERT_RECORD;
    return false;
  }
  res = sqlite3_prepare_v2(db_instance_,
                           SELECT_RECORDS,
                           strlen(SELECT_RECORDS),
                           &select_stmt_,
                           0);
  if(res != SQLITE_OK) {
    LOG(ERROR) << SELECT_RECORDS;
    return false;
  }
  return true;
}

bool RecordManager::InsertRecord(const std::string player_name,
                                 const std::string token_id,
                                 unsigned int score,
                                 const std::string start_time,
                                 const std::string end_time) {

  int res = sqlite3_reset(insert_stmt_);

  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to reset insert_stmt_*";
    return false;
  }

  // 1.
  res = sqlite3_bind_text(insert_stmt_, 1,
                          player_name.c_str(),
                          player_name.size(),
                          SQLITE_STATIC);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to call sqlite3_bind_*";
    return false;
  }
  // 2.
  res = sqlite3_bind_text(insert_stmt_, 2,
                          token_id.c_str(),
                          token_id.size(),
                          SQLITE_STATIC);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to call sqlite3_bind_*";
    return false;
  }
  // 3.
  res = sqlite3_bind_int(insert_stmt_, 3, score);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to call sqlite3_bind_*";
    return false;
  }
  // 2.
  res = sqlite3_bind_text(insert_stmt_, 4,
                          start_time.c_str(),
                          start_time.size(),
                          SQLITE_STATIC);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to call sqlite3_bind_*";
    return false;
  }
  // 2.
  res = sqlite3_bind_text(insert_stmt_, 5,
                          end_time.c_str(),
                          end_time.size(),
                          SQLITE_STATIC);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to call sqlite3_bind_*";
    return false;
  }

  res = sqlite3_step(insert_stmt_);
  if(res != SQLITE_DONE) {
    LOG(ERROR) << "Failure to update the record state";
  }

  return true;
}


void RecordManager::UnintRecordManager() {
  int res = 0;

  res = sqlite3_finalize(insert_stmt_);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to finalize insert_stmt_";
  }

  res = sqlite3_finalize(select_stmt_);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to finalize select_stmt_";
  }

  sqlite3_close(db_instance_);
}

bool RecordManager::GetRankRecords(std::vector<Record> &records) {
  int res = 0;
  // 2.
  res = sqlite3_reset(select_stmt_);
  if(res != SQLITE_OK) {
    LOG(ERROR) << "Failure to reset select_stmt_*";
    return false;
  }
  while(true) {
    // 3.
    res = sqlite3_step(select_stmt_);
    if(res == SQLITE_ROW) {
      Record record;
      record.player_name.append((const char *)sqlite3_column_text(select_stmt_, 1),
                                sqlite3_column_bytes(select_stmt_, 1));
      record.token_id.append((const char *)sqlite3_column_text(select_stmt_, 2),
                             sqlite3_column_bytes(select_stmt_, 2));
      record.score = sqlite3_column_int(select_stmt_, 3);
      record.start_time.append((const char *)sqlite3_column_text(select_stmt_, 4),
                               sqlite3_column_bytes(select_stmt_, 4));
      record.end_time.append((const char *)sqlite3_column_text(select_stmt_, 5),
                             sqlite3_column_bytes(select_stmt_, 5));
      records.push_back(record);
    } else if (res == SQLITE_DONE) {
      //LOG(INFO) << "Select succeed";
      break;
    } else {
      LOG(ERROR) << sqlite3_errmsg(db_instance_);
      break;
    }
  }
  return true;
}

bool RecordManager::PrecheckDatabase(const std::string database_file) {

  FILE *fp = fopen(database_file.c_str(), "r");

  if(fp != NULL) {
    fclose(fp);
    return true;
  }

  sqlite3 *db_instance = NULL;
  int res = sqlite3_open(database_file.c_str(), &db_instance);
  if(res) {
    LOG(ERROR) << "Failure to open the database";
    sqlite3_close(db_instance);
    return true;
  }

  res = sqlite3_exec(db_instance, CREATE_DATABASE, 0,0,0);
  if(res != SQLITE_OK) {
    LOG(ERROR) << CREATE_DATABASE;
    return false;
  }

  sqlite3_close(db_instance);
  return true;
}
}