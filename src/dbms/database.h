#pragma once

#include <sqlite3.h>
#include <exception>
#include <iostream>
#include <vector>

int push_callback(void* execResult, int argc, char** argv, char** azColName);

class ExecResult {
public:
    std::vector<std::string> title;
    std::vector<std::vector<std::string>> data;
public:
    ExecResult();
    ~ExecResult();
    std::vector<std::string> operator[](size_t row);
    size_t cols();
    size_t rows();
    // just for debug
    void log();
};

class DBWrap {
private:
    sqlite3* db; // database context
public:
    DBWrap();
    ~DBWrap();
    std::string version();
    void touch(const std::string& databaseFile);
    ExecResult execute(const std::string& query);
    void close();
};
