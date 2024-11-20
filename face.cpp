#ifndef FACE_CPP
#define FACE_CPP
#include "face.h"
face::face(){/*only a default*/
    owners[0] = -1;
    owners[1] = -1;
};
face::face(point corner1,point corner2,point corner3,point corner4){
    corners[0] = corner1;
    corners[1] = corner2;
    corners[2] = corner3;
    corners[3] = corner4;
    owners[0] = -1;
    owners[1] = -1;
}
std::string face::printCorners(){
    std::string returnString= "";
    for(int b=0;b<4;b++){
            returnString += "Point id: ";
            returnString += std::to_string(corners[b].id);
            returnString += '[';
            returnString += std::to_string(corners[b].x);
            returnString += ' ';
            returnString += std::to_string(corners[b].y);
            returnString += ' ';
            returnString += std::to_string(corners[b].z);
            returnString += "] ";
            returnString += '\n';
    }
    returnString += ") ;";
    returnString += '\n';
    return returnString;
};
#endif