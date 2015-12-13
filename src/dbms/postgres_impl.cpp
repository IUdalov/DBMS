#include "postgres_impl.h"

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

        printf("%s %s %s\n", PQgetvalue(res, i, 0),
            PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
    }

    PQclear(res);

    return ExecResult();
}

void PostgreSQLWrap::close() {
    PQfinish(db);
}


void PostgreSQLWrap::log() {
    std::cout << "touch: PSQL server:" << std::to_string(PQserverVersion(db)) << std::endl;
    std::cout << "touch: PSQL lib:" << std::to_string(PQlibVersion()) << std::endl;
    PQfinish(db);
}