#ifndef FACE_H
#define FACE_H
#include "point.h"
#include <string>
class face
{
private:
    point* corners[3];//WARNING IF THE DATA IS NOT WRITTEN THERE IS NO DEAFAULT SAFTY NET !!!!!!!!!!!!!!!
public:
    face();
    face(point corner1,point corner2,point corner3,point corner4);
};
#endif