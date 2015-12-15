#include "database.h"

ExecResult::ExecResult() {
}

ExecResult::~ExecResult() {
}

std::vector<std::string> ExecResult::operator[](size_t row) {
    return data[row];
}

size_t ExecResult::cols() {
    return title.size();
}

size_t ExecResult::rows() {
    return data.size();
}

void ExecResult::log() {
    for(auto colName = title.begin(); colName != title.end(); colName++) {
        std::cout << *colName << "|";
    }
    std::cout << std::endl;
    for(auto row = data.begin(); row != data.end(); row++) {
        for(auto value = (*row).begin(); value != (*row).end(); value++) {
            std::cout << *value << "|";
        }
        std::cout << std::endl;
    }
}
