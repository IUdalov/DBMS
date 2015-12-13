#pragma once

#include <exception>
#include <stdexcept>
#include <iostream>
#include <vector>


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
public:
    virtual ~DBWrap(){}
    virtual std::string version() = 0;
    virtual void touch(const std::string& connectionString) = 0;
    virtual ExecResult execute(const std::string& query) = 0;
    virtual void close() = 0;
};
