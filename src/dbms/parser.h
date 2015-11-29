#ifndef ARGHANDLER_H
#define ARGHANDLER_H

#include <string>
#include <vector>
#include <typeinfo>
#include "types.h"
#include <assert.h>

namespace dbt {
/*
 *
 * Variadic values parsing kernel
 *
 */
template<typename Handler, typename T>
void HandleArgs(Handler& c, T t)
{
    c.parse(t);
}

template<typename Handler, typename T, typename... Args>
void HandleArgs(Handler& c, T t, Args... args)
{
    c.parse(t);
    HandleArgs(c, args...);
}

/*
 *
 * Variadic types parsing kernel
 *
 */
template<typename Handler, typename T>
void HandleTypes(Handler& c)
{
    c.parse(typeid(T));
}

template<typename Handler, typename T1, typename T2, typename... Args>
void HandleTypes(Handler& c)
{
    c.parse(typeid(T1));
    HandleTypes<Handler, T2, Args...>(c);
}

/*
 *
 * PARSERS
 *
 */

class ValueToStringParser
{
public:
    std::vector<std::string> data;

    template<typename T>
    void parse(const T& val)
    {
        data.push_back(ToString(val));
    }
};

class StringToValueParser
{
public:
    std::vector<std::string> data;

    template<typename T>
    void parse(T* val)
    {
        if(val)
            FromString(*val, data.front());
        data.erase(data.begin());
    }
};

class TypeNameParser
{
public:
    std::vector<std::string> data;

    void parse(const std::type_info& id)
    {
        std::string name;
        assert(GetTypeName(id, name));
        data.push_back(name);
    }
};

}

#endif // ARGHANDLER_H
