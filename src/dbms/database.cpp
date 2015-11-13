#include "database.h"

int justPrintCallback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = nullptr;
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }    
    printf("\n");
    return 0;
}

DBWrap::DBWrap() : db(nullptr)
{}

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

void DBWrap::execute(const std::string& query) {
    char *err_msg = 0;

    int rc = sqlite3_exec(db, query.c_str(), justPrintCallback, nullptr, &err_msg);

    if (rc != SQLITE_OK ) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        throw std::runtime_error(err_msg);
    }
}

void DBWrap::close() {
    sqlite3_close(db);
}