#ifndef CELL_CPP
#define CELL_CPP

#include "cell.h"
#include "point.h"
cell::cell(point* corner1,point* corner2,point* corner3,point* corner4,point* corner5,point* corner6,point* corner7,point* corner8,
            volocity volocity){
                corners[0] = corner1;
                corners[1] = corner2;
                corners[2] = corner3;
                corners[3] = corner4;
                corners[4] = corner5;
                corners[5] = corner6;
                corners[6] = corner7;
                corners[7] = corner8;
                internalVolocity = volocity;
            };


#endif