#ifndef RULES_H
#define RULES_H

#include <string>
#include <typeinfo>
#include <stdexcept>
#include "types.h"

namespace dbt {

typedef std::pair<std::string, std::string> Field;
typedef std::string Query;

Query operator!(const Query& op)
{
    return "NOT (" + op + ")";
}

Query operator|(const Query& left, const Query& right)
{
    return "(" + left + ") OR (" + right + ")";
}

Query operator&(const Query& left, const Query& right)
{
    return "(" + left + ") AND (" + right + ")";
}

#define FIELD_BIN_OPERATOR(native_op, string_op, native_type) \
    Query native_op (const Field& field, const native_type & value) \
    { \
        if(!CheckTypeCons< native_type >(field.second)) \
            throw std::runtime_error("Uncompatible type"); \
        return "(" + field.first + ") " + string_op + " (" + ToString(value) + ")"; \
    }

// INT
FIELD_BIN_OPERATOR(operator==, "=", int)
FIELD_BIN_OPERATOR(operator<, "<", int)
FIELD_BIN_OPERATOR(operator>, ">", int)
FIELD_BIN_OPERATOR(operator>=, ">=", int)
FIELD_BIN_OPERATOR(operator<=, "<=", int)
FIELD_BIN_OPERATOR(operator!=, "!=", int)

// REAL
FIELD_BIN_OPERATOR(operator==, "=", double)
FIELD_BIN_OPERATOR(operator<, "<", double)
FIELD_BIN_OPERATOR(operator>, ">", double)
FIELD_BIN_OPERATOR(operator>=, ">=", double)
FIELD_BIN_OPERATOR(operator<=, "<=", double)
FIELD_BIN_OPERATOR(operator!=, "!=", double)

// TEXT
FIELD_BIN_OPERATOR(operator==, "=", std::string)
FIELD_BIN_OPERATOR(operator%,  "like", std::string)

}

#endif // RULES_H
