#ifndef FACE_CPP
#define FACE_CPP
#include "face.h"
face::face(){/*only a default*/};
face::face(point corner1,point corner2,point corner3,point corner4){
    corners[0] = &corner1;
    corners[1] = &corner2;
    corners[2] = &corner3;
    corners[3] = &corner4;
}
#endif