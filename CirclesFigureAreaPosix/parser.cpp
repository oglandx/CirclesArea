/*
 * File:   parser.h
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:54 AM
 */

#ifndef PARSER_H
#define	PARSER_H

#include "parser.h"

std::vector< Circle* > *parseCirclesFromFile(const std::string &filename, int max_circles) throw(FileException)
{
    std::vector< Circle* > *circles = new std::vector< Circle* >();

    std::ifstream file;
    file.open(filename.c_str());

    if(!file.is_open())
    {
        std::stringstream reason;
        reason << "Cannot open file <" << filename << ">" << std::endl;
        throw FileException(reason.str().c_str());
    }

    for(unsigned long i = 0; i < max_circles && !file.eof(); ++i)
    {
        std::string line;
        std::getline(file, line);

        if(!file.bad())
        {
            std::istringstream line_to_parse(line);
            Circle* circle = new Circle(new Point(0, 0), 0);
            line_to_parse >> circle->center->x >> circle->center->y >> circle->radius;
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

#endif	/* PARSER_H */

