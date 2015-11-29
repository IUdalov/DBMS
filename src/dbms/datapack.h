#ifndef DATAPACK_H
#define DATAPACK_H

#include <string>
#include <vector>
#include <database.h>
#include "rules.h"
#include "parser.h"

namespace dbt {
    template<typename... Args>
    class Datapack
    {
    public:
        Datapack(std::string _name, DBWrap& _db) : m_name(_name), m_db(_db)
        {
            TypeNameParser parser;
            HandleTypes<TypeNameParser, Args...>(parser);
            for(auto f = parser.data.begin(); f != parser.data.end(); f++)
                m_fields.push_back(std::make_pair(
                                     "column" + std::to_string(f - parser.data.begin()),
                                     *f
                                     )
                                 );
            std::string query = \
                    "DROP TABLE IF EXISTS " + m_name +
                    ";CREATE TABLE " + m_name + "(";
            for(auto f : m_fields)
                query += f.first + " " + f.second + ",";
            query.pop_back();
            query += ");";
#ifdef DBT_DEBUG
            std::cout << query << std::endl;
#endif
            m_db.execute(query);
        }
        void Push(const Args&...args)
        {
            std::string query = \
                    "INSERT INTO " + m_name +
                    " VALUES(";
            ValueToStringParser parser;
            HandleArgs(parser, args...);
            for(auto f: parser.data)
                query += f + ",";
            query.pop_back();
            query += ");";
#ifdef DBT_DEBUG
            std::cout << query << std::endl;
#endif
            m_db.execute(query);
        }
        Field operator[](const int& ind)
        {
            return m_fields[ind];
        }

        void Request(const Query& rule = "")
        {
            auto q = CompileSelectQuery(rule);
#ifdef DBT_DEBUG
            std::cout << q << std::endl;
#endif
            m_data = m_db.execute(q).data;
        }

        void Remove(const Query& rule = "")
        {
            auto q = CompileRemoveQuery(rule);
#ifdef DBT_DEBUG
            std::cout << q << std::endl;
#endif
            m_data = m_db.execute(q).data;
        }

        bool Get(Args*...args)
        {
            if(m_data.empty())
                return false;
            StringToValueParser parser;
            parser.data = m_data.front();
            m_data.erase(m_data.begin());
            HandleArgs(parser, args...);
            return true;
        }

    private:
        std::string CompileSelectQuery(const Query& rule)
        {
            std::string str = "SELECT ";
            for(auto f: m_fields)
                str += f.first + ",";
            str.pop_back();
            str += " FROM " + m_name;
            if(rule != "")
                str += " WHERE " + rule;
            return str;
        }

        std::string CompileRemoveQuery(const Query& rule)
        {
            std::string str = "DELETE ";
            str += " FROM " + m_name;
            if(rule != "")
                str += " WHERE " + rule;
            return str;
        }

        std::string m_name;
        std::vector<Field> m_fields;
        std::vector<std::vector<std::string>> m_data;
        DBWrap& m_db;
    };
}

#endif // DATAPACK_H
