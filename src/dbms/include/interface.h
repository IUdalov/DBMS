#include <string>
#include "database.h"

int touchDB(const std::string& path);
std::string getSQLiteVersion();

void createCarsTable();
void printCars();
