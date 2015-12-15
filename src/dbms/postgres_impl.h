#pragma once

#include "database.h"

#ifdef POSTGRESQL
#include <libpq-fe.h>

class PostgreSQLWrap : public DBWrap {
private:
     PGconn *db; // database context
public:
    PostgreSQLWrap();
    virtual ~PostgreSQLWrap();
    virtual std::string version() override;
    virtual void touch(const std::string& databaseFile) override;
    virtual ExecResult execute(const std::string& query) override;
    virtual void close() override;
    // just for debug
    void log();
};
#endif
