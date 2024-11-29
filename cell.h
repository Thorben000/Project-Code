
#ifndef CELL_H
#define CELL_H
#include "point.h"
#include "face.h"
#include "volocity.h"
#include <unordered_map>
class cell
{
private:
    
    volocity newVolocity;
    face faces[6];//WARNING IF THE DATA IS NOT WRITTEN THERE IS NO DEAFAULT SAFTY NET !!!!!!!!!!!!!!!
    
    int key[6];
    int declaredNighbours;
    int declaredFaces;
public:
    point center;
    volocity internalVolocity;
    bool exists;
    cell();
    int id;
    cell(face face1,face face2,face face3,face face4,face face5,face face6);
    void setVolocity(volocity volocity);
    void addFace(face face);
    void determineCenter();
    void determineNeighbours(std::unordered_map<int,cell> map);
    void math(std::unordered_map<int,cell>);
    void update();//sets the new volocity as internal volocity
    void addNighbour(int id_x);
    std::string printInternalVolocity();
    std::string printCenter();
    std::string printNeighbours();
    std::string printFaces();
    std::string printCorners();
};
#endif