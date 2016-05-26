//
// Created by oglandx on 3/16/16.
//

#ifndef CIRCLESFIGUREAREA_PARSER_H
#define CIRCLESFIGUREAREA_PARSER_H

#include <string>
#include <vector>
#include <exception>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>

#include "types.h"

class FileException : std::exception
{
private:
    char *reason;
public:
    FileException(const char *what)
    {
        this->reason = new char[std::strlen(what)];
        strcpy(this->reason, what);
    }

    const char* what() throw()
    {
        return this->reason;
    }
};

std::vector< Circle* > *parseCirclesFromFile(const std::string &filename, int max_circles) throw(FileException);

#endif //CIRCLESFIGUREAREA_PARSER_H
