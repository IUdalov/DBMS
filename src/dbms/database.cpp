#include "database.h"

int push_callback(void* execResult, int argc, char** argv, char** azColName) {
    ExecResult& agrRes = *static_cast<ExecResult*>(execResult);
    std::vector<std::string> row;

    for(int i = 0; i < argc; ++i) {
        // think about it, NULL and empty string are not the same
        row.push_back(argv[i] ? argv[i] : "");
    }
    if (!agrRes.title.size()) {
        for(int i = 0; i < argc; ++i) {
            agrRes.title.push_back(azColName[i]);
        }
    }

    agrRes.data.push_back(row);
    return 0;
}

ExecResult::ExecResult() {
}

ExecResult::~ExecResult() {
}

std::vector<std::string> ExecResult::operator[](size_t row) {
    return data[row];
}

size_t ExecResult::cols() {
    return title.size();
}

size_t ExecResult::rows() {
    return data.size();
}

void ExecResult::log() {
    for(auto colName = title.begin(); colName != title.end(); colName++) {
        std::cout << *colName << "|";
    }
    std::cout << std::endl;
    for(auto row = data.begin(); row != data.end(); row++) {
        for(auto value = (*row).begin(); value != (*row).end(); value++) {
            std::cout << *value << "|";
        }
        std::cout << std::endl;
    }
}

DBWrap::DBWrap() : db(nullptr) {
}

DBWrap::~DBWrap() {
    close();
}

std::string DBWrap::version() {
    return sqlite3_libversion();
}

void DBWrap::touch(const std::string& databaseFile) {
    int rc = sqlite3_open(databaseFile.c_str(), &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Unable to touch database!");
    }
 
}

ExecResult DBWrap::execute(const std::string& query) {
    ExecResult res;
    char *err_msg = 0;

    int rc = sqlite3_exec(db, query.c_str(), push_callback, &res, &err_msg);

    if (rc != SQLITE_OK ) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        throw std::runtime_error(err_msg);
    }
    return res;
}

void DBWrap::close() {
    sqlite3_close(db);
}