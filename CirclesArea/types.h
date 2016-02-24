/* 
 * File:   types.h
 * Author: oglandx
 *
 * Created on February 17, 2016, 1:59 AM
 */

#ifndef TYPES_H
#define	TYPES_H

template <typename T>
struct Coords2{
    T x;
    T y;
};

template <typename T>
struct Circle {
    Coords2<T> center;
    T radius;
};

typedef Circle<float> Circlef;
typedef Circle<int> Circlei;


template <typename T>
struct Rect {
    Coords2<T> lt;
    Coords2<T> br;
};

#endif	/* TYPES_H */

