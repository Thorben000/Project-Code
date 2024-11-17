
#ifndef CELL_H
#define CELL_H
#include "point.h"
#include "face.h"
#include "volocity.h"
#include <unordered_map>
class cell
{
private:
    point center;
    face* faces[6];//WARNING IF THE DATA IS NOT WRITTEN THERE IS NO DEAFAULT SAFTY NET !!!!!!!!!!!!!!!
    
    cell* neighbours[6];
    
    int declaredFaces;
public:
    volocity internalVolocity;
    bool exists;
    cell();
    cell(face* face1,face* face2,face* face3,face* face4,face* face5,face* face6);
    void setVolocity(volocity volocity);
    void addFace(face* face);
    void determineCenter();
    void determineNeighbours(std::unordered_map<int,cell> map);
};
#endif