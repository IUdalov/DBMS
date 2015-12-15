#include <iostream>

#define BENCHMARK
#ifndef BENCHMARK

//#define DBT_DEBUG
//#define POSTGRESQL

#include <sqlite_impl.h>
#include <postgres_impl.h>
#include "datapack.h"

using namespace dbt;

struct Person
{
    std::string fname;
    std::string mname;
    std::string lname;
    int         age;
    double      height;
};

const Person sample_data[] =
{
    {"fname1", "mname1", "lname1", 22, 180.1},
    {"fname2", "mname2", "lname2", 43, 190.7},
    {"fname3", "mname3", "lname3", 16, 167.3},
    {"fname4", "mname4", "lname4", 51, 171.4},
    {"fname5", "mname5", "lname5", 32, 183.8},
    {"fname6", "mname6", "lname6", 14, 160.7},
    {"fname7", "mname7", "lname7", 10, 143.6},
    {"fname8", "mname8", "lname8", 27, 191.2},
};

template<typename T>
void Print(T& pack)
{
    /*
     *
     * Data mining
     *
     */
    std::cout << "Results:" << std::endl;
    Person p;
    while(pack.Get(&p.fname, &p.mname, &p.lname, &p.age, &p.height))
        std::cout << "\tPerson:"  << p.fname << ", " << p.mname << ", " <<
                     p.lname << ", " << p.age << ", "<< p.height << std::endl;
}

void printUsage() {
   std::cout << "Usage:" << std::endl;
   std::cout << "./<binary name>" << " <sqlite|postgres> <connection string> "<< std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printUsage();
        return 1; // 'user=iudalov dbname=dbms host=localhost'
    }


    std::shared_ptr<DBWrap> db = nullptr;
    if (std::string(argv[1]) == std::string("sqlite")) {
        db = std::make_shared<SQLiteWrap>();
    } else if (std::string(argv[1]) == std::string("postgres")) {
        db = std::make_shared<PostgreSQLWrap>();
    } else {
        printUsage();
        return 1;
    }
    db->touch(argv[2]);



    /*
     *
     * Container creating
     *
     */
    Datapack<std::string, std::string, std::string, int, double> pack("People", *db);

    /*
     *
     * Data filling
     *
     */
    for(auto& i: sample_data)
    {
        pack.Push(i.fname, i.mname, i.lname, i.age, i.height);
    }

    /*
     *
     * Request compiling
     *
     */
    pack.Request((pack[3] >= 20) & (pack[3] < 50) & (pack[4] != 180.1) & !(pack[4] < 30.));
    Print(pack);

    pack.Request(pack[0] % "f%1");
    Print(pack);

    /*
     *
     * Remove data
     *
     */
    pack.Remove((pack[4] >= 190.) & (pack[3] == 27));
    pack.Request();
    Print(pack);

    pack.Remove();
    pack.Request();
    Print(pack);
    return 0;
}

#endif // BENCHMARK
