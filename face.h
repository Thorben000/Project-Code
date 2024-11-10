#ifndef FACE_H
#define FACE_H
#include "point.h"
#include <string>
class face
{
public:
    point* corners[4];//WARNING IF THE DATA IS NOT WRITTEN THERE IS NO DEAFAULT SAFTY NET !!!!!!!!!!!!!!!
    int owners[2];
    face();
    face(point corner1,point corner2,point corner3,point corner4);
};
#endif