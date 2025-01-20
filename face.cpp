#ifndef FACE_CPP
#define FACE_CPP
#include "face.h"
/*

Constructors for the face class

*/
face::face(){/*only a default*/
    owners[0] = -1;
    owners[1] = -1;
};
face::face(int id){/*only a default*/
    owners[0] = -1;
    owners[1] = -1;
};
face::face(point corner1,point corner2,point corner3,point corner4){/*only a default*/
    owners[0] = -1;
    owners[1] = -1;
}
#endif