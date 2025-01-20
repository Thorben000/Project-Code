#ifndef POINT_CPP
#define POINT_CPP
#include "point.h"
/*

This file contains the constructors for the point class

*/
point::point(){
    x=0;
    y=0;
    z=0;
};
point::point(double xi,double yi,double zi){
    x=xi;
    y=yi;
    z=zi;
};
#endif