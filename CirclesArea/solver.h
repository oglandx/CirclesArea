/* 
 * File:   solver.h
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:54 AM
 */

#ifndef SOLVER_H
#define	SOLVER_H

#include "types.h"
#include <functional>
#include <vector>

template<typename T>
T solve(const std::vector< Circle<T> > circles, int density, std::function<T ()> randomizer);

#endif	/* SOLVER_H */

