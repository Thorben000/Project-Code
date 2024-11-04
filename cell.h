
#ifndef CELL_H
#define CELL_H
#include "point.h"
#include "volocity.h"
class cell
{
private:
    point center;
    point* corners[7];//WARNING IF THE DATA IS NOT WRITTEN THERE IS NO DEAFAULT SAFTY NET !!!!!!!!!!!!!!!
    volocity internalVolocity;
public:
    cell(point* corner1,point* corner2,point* corner3,point* corner4,point* corner5,point* corner6,point* corner7,point* corner8,
            volocity volocity);
};
#endif