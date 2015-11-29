#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <typeinfo>

namespace dbt {

typedef struct
{
    std::string name;
    const std::type_info& type;
} TypeDesc;

const TypeDesc TypesDefinition[] =
{
    {"INT", typeid(int)},
    {"TEXT", typeid(std::string)},
    {"REAL", typeid(double)}
};

bool GetTypeName(const std::type_info& info, std::string& name)
{
    for(auto& i: TypesDefinition)
    {
        if(i.type == info)
        {
            name = i.name;
            return true;
        }
    }
    return false;
}

template<typename T>
bool CheckTypeCons(const std::string& name)
{
    for(auto& i: TypesDefinition)
    {
        if(i.name == name && typeid(T) == i.type)
            return true;
    }
    return false;
}

std::string ToString(const int& val)
{
    return std::to_string(val);
}

std::string ToString(const double& val)
{
    return std::to_string(val);
}

std::string ToString(const std::string& val)
{
    return "'" + val + "'";
}

void FromString(int& val, const std::string& str)
{
    val = atoi(str.c_str());
}

void FromString(double& val, const std::string& str)
{
    val = atof(str.c_str());
}

void FromString(std::string& val, const std::string& str)
{
    val = str;
}

}

#endif // TYPES_H
