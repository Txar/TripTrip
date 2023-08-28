#pragma once
#include <iostream>

bool cst(std::string *&lst, std::string str) {
    for (int i = 0; i < (int)lst->length(); i++) {
        if (lst[i] == str) return true;
    }
    return false;
}