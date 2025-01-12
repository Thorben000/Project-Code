
#ifndef CELL_H
#define CELL_H
#include "point.h"
#include "face.h"
#include "velocity.h"
#include <unordered_map>
#include <vector>

class cell_result {
public:
    velocity gradient[3];
    double q_crit;
};

class cell
{
public:
    velocity internalVolocity;
    point center;
    int neighbors[6];
    int id;

    cell();
    cell(face face1,face face2,face face3,face face4,face face5,face face6);
    void setVolocity(velocity volocity);
    void addFace(face face);
    void determineCenter();
    void determineNeighbours(std::vector<cell> cells);
    void math(std::vector<cell> cells, cell_result* result);
    void update();//sets the new volocity as internal volocity
    void addNeighbour(int id_x);
    std::string printInternalVolocity();
    std::string printCenter();
    std::string printNeighbours();
    std::string printFaces();
    std::string printCorners();
    std::string printGradient(velocity gradient[3]);
    std::string printQ(double q_crit);
    double trace_base(velocity gradient[3]);
    double trace_other(velocity gradient[3]);
};

#endif