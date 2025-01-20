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
    /*
    
    This file is here for legacy reasons as it WILL break the code if I change or remove this,

    its main use it to allow me to determine the neighours of cells saved in:
    int owners[2]
    
    */
};
#endif