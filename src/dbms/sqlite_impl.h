#pragma once

#include "database.h"

#include <sqlite3.h>

class SQLiteWrap : public DBWrap {
private:
    sqlite3* db; // database context
public:
    SQLiteWrap();
    virtual ~SQLiteWrap();
    virtual std::string version() override;
    virtual void touch(const std::string& databaseFile) override;
    virtual ExecResult execute(const std::string& query) override;
    virtual void close() override;
};
