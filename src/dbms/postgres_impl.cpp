#include "postgres_impl.h"

#ifdef POSTGRESQL
PostgreSQLWrap::PostgreSQLWrap() : db(nullptr) {
}

PostgreSQLWrap::~PostgreSQLWrap() {
    close();
}

std::string PostgreSQLWrap::version() {
    return std::to_string(PQlibVersion());
}

void PostgreSQLWrap::touch(const std::string& connectionString) {
    db = PQconnectdb(connectionString.c_str());
    if (PQstatus(db) == CONNECTION_BAD) {
        std::cout << "Connection to database failed: " << PQerrorMessage(db) << std::endl;
        throw std::runtime_error(PQerrorMessage(db));
        close();
    }
    return;
}

ExecResult PostgreSQLWrap::execute(const std::string& query) {
    PGresult *res = PQexec(db, (query + ";").c_str());
    ExecResult wres();
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Error msg: %s", PQerrorMessage(db));
        close();
        throw std::runtime_error(PQerrorMessage(db));
    }
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return ExecResult();
    }

    for(int i = 0; i < PQntuples(res); i++) {
        std::vector<string> tmp;
        for(int j = 0; j < PQnfields(res); j++) {
            tmp.push_back(PQgetvalue(res, i, j));
        }
        ExecResult.data.push_back(tmp);
    }

    PQclear(res);

    return wres;
}

void PostgreSQLWrap::close() {
    PQfinish(db);
}


void PostgreSQLWrap::log() {
    std::cout << "touch: PSQL server:" << std::to_string(PQserverVersion(db)) << std::endl;
    std::cout << "touch: PSQL lib:" << std::to_string(PQlibVersion()) << std::endl;
    PQfinish(db);
}
#endif
