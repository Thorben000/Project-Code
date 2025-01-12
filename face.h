#ifndef FACE_H
#define FACE_H
#include "point.h"
#include <string>
class face
{
public:
    int owners[2];
    std::string printCorners();
    face();
    face(int id);
    face(point corner1,point corner2,point corner3,point corner4);
};
#endif