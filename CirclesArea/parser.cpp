#include "parser.h"

#include <iostream>
#include <fstream>

template<typename T>
std::vector< Circle<T> > &parseCirclesFromFile(const std::string filename, int max_circles)
{
    std::vector< Circle<T> > circles;

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
                circles.push_back(circle);
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