/* 
 * File:   randomizer.h
 * Author: oglandx
 *
 * Created on March 30, 2016, 1:10 AM
 */

#ifndef RANDOMIZER_H
#define	RANDOMIZER_H

#include <stdlib.h>
#include <random>

#include "types.h"


Point *random_simple(const Rect *rect, int density);
Point *random_uniform_real_distribution(const Rect *rect, int density);


#endif	/* RANDOMIZER_H */

