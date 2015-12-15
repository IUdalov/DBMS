#include <random>
#include <time.h>
#include <fstream>
#include <unistd.h>
#include <memory>

//#define DBT_DEBUG
//#define POSTGRESQL
#include "datapack.h"
#include "sqlite_impl.h"
#ifdef POSTGRESQL
    #include "postgres_impl.h"
#endif

using namespace dbt;

int GenerateInt()
{
    return rand();
}

std::string GenerateString()
{
    std::string res;
    for (int i = 0; i < 200; i++)
        res += std::to_string(rand() % 10);
    return res;
}

double GenerateDouble()
{
    return (rand() % 30000) / 1000.;
}

#define DATA_SIZE 5000

class Timer
{
public:
    std::string m_name;
    std::vector<long> m_stamps;
    long m_total;
    long m_lastStamp;

    Timer(std::string name) : m_name(name), m_total(0), m_lastStamp(0) { }

    void TimeStampStop()
    {
        m_stamps.push_back(CurrTime() - m_lastStamp);
    }

    Timer operator+=(const Timer& timer)
    {
        m_stamps.insert(m_stamps.end(), timer.m_stamps.begin(), timer.m_stamps.end());
        return *this;
    }

    void TimeStampStart()
    {
        m_lastStamp = CurrTime();
    }

    void TimerFinish()
    {
        for(auto& i: m_stamps)
            m_total += i;
    }

    long CurrTime(void)
    {
        struct timespec spec;
        clock_gettime(CLOCK_REALTIME, &spec);
        return spec.tv_sec * 1000000 + spec.tv_nsec / 1000;
    }

    void Save(void)
    {
        #define MSEC(sec) ((double)sec / 1000000.)
        if (m_stamps.size() > 1)
        {
            std::ofstream streamTotal(m_name + ".total.mark");
            streamTotal << MSEC(m_total) << std::endl;

            std::ofstream streamStamps(m_name + ".stamps.mark");
            int num = 1;
            for (auto& i: m_stamps)
                streamStamps << num << ", " << MSEC(i) << std::endl;

            std::ofstream streamAvg(m_name + ".avg.mark");
            streamAvg << MSEC(((double)m_total / (double)m_stamps.size())) << std::endl;
        }
        else
        {
            std::ofstream streamTotal(m_name + ".mark");
            streamTotal << MSEC(m_total) << std::endl;
        }
    }
};

std::vector<std::pair<Query, std::string>> GenerateRequests(Datapack<std::string, int, double>& pack)
{
    return
    {
            {(pack[1] > 0), "query1"}, // All entries
            {(pack[2] > 15.), "query2"}, // Half of entries
            {(pack[2] > 10.) & (pack[2] < 20.) & (pack[1] != 100) & (pack[1] != 200) & (pack[1] != 300) & (pack[1] != 400) \
                | ((pack[2] < 10.) | (pack[2] > 20.)) & (pack[1] != 100503) & (pack[1] != 100502) & (pack[1] != 100501) & (pack[1] != 100500), "query3"}, // Complex rule
            {(pack[0] % "%0%0%"), "query4"}, // String operation 'like'
    };
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
        #ifdef POSTGRESQL
            db = std::make_shared<PostgreSQLWrap>();
        #else
            std::cout << "PostgreSQL is not available. Define POSTGRESQL!" << std::endl;
            return 1; // 'user=iudalov dbname=dbms host=localhost'
        #endif
    } else {
        printUsage();
        return 1;
    }
    db->touch(argv[2]);

    srand(0);

    Datapack<std::string, int, double> pack("BenchMarkData", *db);

    Timer timerUploading("uploading");
    for(int i = 0; i < DATA_SIZE; i++)
    {
        timerUploading.TimeStampStart();
        pack.Push(GenerateString(), GenerateInt(), GenerateDouble());
        timerUploading.TimeStampStop();
    }
    timerUploading.TimerFinish();
    timerUploading.Save();

    Timer timerTotalReqs("totalrequests");
    auto requests = GenerateRequests(pack);
    for (auto& r: requests)
    {
        Timer timer(r.second);
        timer.TimeStampStart();
        pack.Request(r.first);
        timer.TimeStampStop();
        timer.TimerFinish();
        timer.Save();
        timerTotalReqs += timer;
    }

    //pack.Remove((pack[4] >= 190.) & (pack[3] == 27));
    //pack.Request();
    return 0;
}

