#pragma once

#include <sqlite3.h>
#include <exception>
#include <iostream>

int justPrintCallback(void *NotUsed, int argc, char **argv, char **azColName);

class DBWrap {
private:
    sqlite3* db; // database context
public:
    DBWrap();
    ~DBWrap();
    std::string version();
    void touch(const std::string& databaseFile);
    void execute(const std::string& query);
    void close();
};
