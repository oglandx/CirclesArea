/* 
 * File:   parser.h
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:54 AM
 */

#ifndef CMDLINE_H
#define	CMDLINE_H

#include <string>
#include <vector>
#include <exception>
#include <cstring>
#include <sstream>

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

    ~FileException()
    {
        delete reason;
    }

    const char* what() const
    {
        return this->reason;
    }
};

template<typename T>
std::vector< Circle<T> > &parseCirclesFromFile(const std::string filename, int max_circles);

#endif	/* CMDLINE_H */

