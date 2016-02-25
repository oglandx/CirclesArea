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

    ~FileException()
    {
        delete reason;
    }

    const char* what() throw()
    {
        return this->reason;
    }
};

template<typename T>
std::vector< Circle<T> > *parseCirclesFromFile(const std::string &filename, int max_circles)
{
    std::vector< Circle<T> > *circles = new std::vector< Circle<T> >();

    std::ifstream file;
    file.open(filename.c_str());

    if(!file.is_open())
    {
        std::stringstream reason;
        reason << "Cannot open file <" << filename << ">" << std::endl;
        throw FileException(reason.str().c_str());
    }

    for(int i = 0; i < max_circles, !file.eof(); ++i)
    {
        std::string line;
        std::getline(file, line);

        if(!file.bad())
        {
            std::istringstream line_to_parse(line);
            Circle<T> circle = {0};
            line_to_parse >> circle.center.x >> circle.center.y >> circle.radius;
            if(!line_to_parse.bad())
            {
                circles->push_back(circle);
            }
            else
            {
                std::stringstream reason;
                reason << "Error in line " << i + 1 << ": not enough symbols" << std::endl;
                throw FileException(reason.str().c_str());
            }
        }
        else
        {
            break;
        }
    }

    file.close();

    return circles;
}

#endif	/* CMDLINE_H */

