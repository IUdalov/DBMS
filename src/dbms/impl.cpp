#pragma once

#include "interface.h"
#include "database.h"

DBWrap db;
int touchDB(const std::string& databaseFile) {
    db.touch(databaseFile);
    return 0;
}

std::string getSQLiteVersion()  {
    return db.version();
}

void createCarsTable() {
    db.execute(
        "DROP TABLE IF EXISTS Cars;" 
        "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
    );
    db.execute(
        "INSERT INTO Cars VALUES(1, 'Audi', 52642);" 
        "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);" 
        "INSERT INTO Cars VALUES(3, 'Skoda', 9000);" 
        "INSERT INTO Cars VALUES(4, 'Volvo', 29000);" 
        "INSERT INTO Cars VALUES(5, 'Bentley', 350000);" 
        "INSERT INTO Cars VALUES(6, 'Citroen', 21000);" 
        "INSERT INTO Cars VALUES(7, 'Hummer', 41400);" 
        "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);"
    );
}

void printCars() {
    db.execute("SELECT * FROM Cars");
}