#include <iostream>
#include "interface.h"

int main(void) {
    touchDB("test.sqlite.db");
    std::cout << "Hello from SQLite " << getSQLiteVersion() << std::endl;
    createCarsTable();
    printCars();
    return 0;
}