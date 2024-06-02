#pragma once

#include <iostream>
#include <string>
#include "File.h"

using namespace std;

class DirectoryNotFoundException: exception
{
public:

    explicit DirectoryNotFoundException(const string& _messeage): messeage(_messeage) {

    }

    const char* what() const noexcept override {
        return concat_c("Директория не была найдена: ", messeage.c_str());
    }

private:
    string messeage;

};


