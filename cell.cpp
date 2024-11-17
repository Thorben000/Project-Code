#ifndef CELL_CPP
#define CELL_CPP

#include "cell.h"
#include "face.h"
#include "point.h"
#include <unordered_map>
#include <iostream>
cell::cell(){exists = false;declaredFaces=0;};

void cell::addFace(face* face){
    faces[declaredFaces] = face;
    declaredFaces++;
    exists = true;
    return;
}

cell::cell(face* face1,face* face2,face* face3,face* face4,face* face5,face* face6){
                //THis is depricated as it is NEVER called!!!!
                faces[0] = face1;
                faces[1] = face2;
                faces[2] = face3;
                faces[3] = face4;
                faces[4] = face5;
                faces[5] = face6;
                center = point();
                determineCenter();
                exists = true;
                declaredFaces = 6;
                //
            };


void cell::determineNeighbours(std::unordered_map<int,cell> map){
    for(int i=0;i<6;i++){
        if(this != &map[faces[i]->owners[0]]){
            neighbours[i] = &map[faces[i]->owners[0]];//sets the first owner as nighbour if it is not self
        }else if (faces[i]->owners[1]==-1)
        {
            neighbours[i] = this;//sets self as nighbour if the second owner does not exist
        }else{
            neighbours[i] = &map[faces[i]->owners[1]];//sets second owner as nighbour
        }
        
    }
    return;
}

void cell::determineCenter(){
    double lowerPoint[3];// index:representation; 0:x 1:y 2:z
    double higherPoint[3];
    if(declaredFaces!=6){
        int dummy;
        std::cout<<"SOMETHING MESSED UP IN CELL LOADING THIS ONE ONLY HAS:"<<declaredFaces<<" FACES DECLARED"<<std::endl;
        std::cin>>dummy;
        return;
    }
    for(int i=0;i<3;i++){
        lowerPoint[i] = -1;
        higherPoint[i] = -1;
    }
    for(int i=0;i<6;i++){
        for(int b=0;b<3;b++){
            //x
            if(lowerPoint[0]==-1 || lowerPoint[0]> faces[i]->corners[b]->x){
                lowerPoint[0]=faces[i]->corners[b]->x;
            }
            if (higherPoint[0]==-1 || higherPoint[0] < faces[i]->corners[b]->x){
                higherPoint[0]=faces[i]->corners[b]->x;
            }
            //y
            if(lowerPoint[1]==-1 || lowerPoint[1]> faces[i]->corners[b]->y){
                lowerPoint[1]=faces[i]->corners[b]->y;
            }
            if (higherPoint[1]==-1 || higherPoint[0] < faces[i]->corners[b]->y){
                higherPoint[1]=faces[i]->corners[b]->y;
            }
            //z
            if(lowerPoint[2]==-1 || lowerPoint[2]> faces[i]->corners[b]->z){
                lowerPoint[2]=faces[i]->corners[b]->z;
            }
            if (higherPoint[2]==-1 || higherPoint[0] < faces[i]->corners[b]->z){
                higherPoint[2]=faces[i]->corners[b]->z;
            }
        }
    }
    center.x = higherPoint[0] - (higherPoint[0] -lowerPoint[0])/2.0;
    center.y = higherPoint[1] - (higherPoint[1] -lowerPoint[1])/2.0;
    center.z = higherPoint[2] - (higherPoint[2] -lowerPoint[2])/2.0;
    return;
}

void cell::setVolocity(volocity volocity){
    internalVolocity = volocity;
    return;
}

#endif