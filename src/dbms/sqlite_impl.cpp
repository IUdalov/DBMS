#include "sqlite_impl.h"

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


SQLiteWrap::SQLiteWrap() : db(nullptr) {
}

SQLiteWrap::~SQLiteWrap() {
    close();
}

std::string SQLiteWrap::version() {
    return sqlite3_libversion();
}

void SQLiteWrap::touch(const std::string& databaseFile) {
    int rc = sqlite3_open(databaseFile.c_str(), &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Unable to touch database!");
    }

}

ExecResult SQLiteWrap::execute(const std::string& query) {
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

void SQLiteWrap::close() {
    sqlite3_close(db);
}