#ifndef CELL_CPP
#define CELL_CPP

#include "cell.h"
#include "face.h"
#include "point.h"
#include <unordered_map>
#include <iostream>
cell::cell(){exists = false;declaredFaces=0;};

void cell::addFace(face face){
    faces[declaredFaces] = face;
    std::cout<<face.id<<std::endl;
    declaredFaces+=1;
    exists = true;
    std::cout<<"Cell id:"<<id<<std::endl<<"current faces:"<<printFaces()<<std::endl;
    return;
}

cell::cell(face face1,face face2,face face3,face face4,face face5,face face6){
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
    std::cout<<"Doing cellID: "<<id<<std::endl;
    std::cout<<"owned faces: "<<printFaces()<<std::endl;
    for(int i=0;i<6;i++){
        //std::cout<<" nighbouris option 1 is:"<<map[faces[i].owners[0]].id<<std::endl;
        //std::cout<<" nighbouris option 2 is:"<<map[faces[i].owners[1]].id<<std::endl;
        //std::cout<<" self is:"<<id<<std::endl;
        if(map[faces[i].owners[0]].id!=faces[i].owners[0] && faces[i].owners[0]!=-1){std::cout<<"ERROR Cell ID"<<map[faces[i].owners[0]].id<<" at "<<faces[i].owners[0]<<std::endl;}
        if(map[faces[i].owners[1]].id!=faces[i].owners[1] && faces[i].owners[1]!=-1){std::cout<<"ERROR Cell ID"<<map[faces[i].owners[1]].id<<" at "<<faces[i].owners[1]<<std::endl;}
        if(id != (map[faces[i].owners[0]]).id){
            key[i] = map[faces[i].owners[0]].id;//sets the first owner as nighbour if it is not self
            //std::cout<<" chosen was: "<<map[faces[i].owners[0]].id<<std::endl;
        }else if (faces[i].owners[1]==-1)
        {
            key[i] = id;//sets self as nighbour if the second owner does not exist
            //std::cout<<"Self nighbour"<<std::endl;
        }else{
            key[i] = map[faces[i].owners[1]].id;//sets second owner as nighbour
            //std::cout<<" chosen was:  "<<map[faces[i].owners[1]].id<<std::endl;
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
            if(lowerPoint[0]==-1 || lowerPoint[0]> faces[i].corners[b].x){
                lowerPoint[0]=faces[i].corners[b].x;
            }
            if (higherPoint[0]==-1 || higherPoint[0] < faces[i].corners[b].x){
                higherPoint[0]=faces[i].corners[b].x;
            }
            //y
            if(lowerPoint[1]==-1 || lowerPoint[1]> faces[i].corners[b].y){
                lowerPoint[1]=faces[i].corners[b].y;
            }
            if (higherPoint[1]==-1 || higherPoint[1] < faces[i].corners[b].y){
                higherPoint[1]=faces[i].corners[b].y;
            }
            //z
            if(lowerPoint[2]==-1 || lowerPoint[2] > faces[i].corners[b].z){
                lowerPoint[2]=faces[i].corners[b].z;
            }
            if (higherPoint[2]==-1 || higherPoint[2] < faces[i].corners[b].z){
                higherPoint[2]=faces[i].corners[b].z;
            }
        }
    }
    center.x = higherPoint[0] - (higherPoint[0] - lowerPoint[0])/2.0;
    center.y = higherPoint[1] - (higherPoint[1] - lowerPoint[1])/2.0;
    center.z = higherPoint[2] - (higherPoint[2] - lowerPoint[2])/2.0;
    return;
}

void cell::setVolocity(volocity volocity){
    internalVolocity = volocity;
    return;
}

std::string cell::printInternalVolocity(){
    std::string returnString = "(";
    returnString += std::to_string(internalVolocity.x);
    returnString += ' ';
    returnString += std::to_string(internalVolocity.y);
    returnString += ' ';
    returnString += std::to_string(internalVolocity.z);
    returnString += ' ';
    returnString += ')';
    return returnString;
}
std::string cell::printCenter(){
    std::string returnString = "(";
    returnString += std::to_string(center.x);
    returnString += ' ';
    returnString += std::to_string(center.y);
    returnString += ' ';
    returnString += std::to_string(center.z);
    returnString += ' ';
    returnString += ')';
    return returnString;
}

std::string cell::printNeighbours(){
    std::string returnString= "(";;
    for(int i=0;i<6;i++){
        returnString += std::to_string(key[i]);
        returnString += ' ';
    }
    returnString += ')';
    return returnString;
}

std::string cell::printFaces(){
    std::string returnString= "(";;
    for(int i=0;i<declaredFaces;i++){
        returnString += std::to_string(faces[i].id);
        returnString += ' ';
    }
    returnString += ')';
    return returnString;
}
std::string cell::printCorners(){
    std::string returnString= "(";
    for(int i=0;i<6;i++){
        returnString += " (";
        for(int b=0;b<4;b++){
            returnString += "Point id: ";
            returnString += std::to_string(faces[i].corners[b].id);
            returnString += '[';
            returnString += std::to_string(faces[i].corners[b].x);
            returnString += ' ';
            returnString += std::to_string(faces[i].corners[b].y);
            returnString += ' ';
            returnString += std::to_string(faces[i].corners[b].z);
            returnString += "] ";
            returnString += '\n';
        }
        returnString += ") ;";
        returnString += '\n';
    }
    returnString += ')';
    return returnString;
}
void cell::math(std::unordered_map<int,cell>){
    for(int i=0;i<3;i++){
        
    }
}
#endif