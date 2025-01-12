#ifndef CELL_CPP
#define CELL_CPP

#include "cell.h"
#include "debug.h"
#include "face.h"
#include "point.h"
#include "velocity.h"
#include <array>
#include <iostream>
#include <cmath>
#include <vector>
cell::cell(): key { -1, -1, -1, -1, -1, 1}, id(-1) {
    
}


void cell::addNeighbour(int id_x){
    for (int i = 0;i < 6;i++) {
        if (key[i] == -1) {
            if(id_x==-1){
                key[i] = id;
                return;
            } else {
                key[i] = id_x;
            }
            break;
        }
    }
}

cell::cell(face face1,face face2,face face3,face face4,face face5,face face6){
                //THis is depricated as it is NEVER called!!!!

                center = point();
                
            };

void cell::setVolocity(velocity volocity){
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
        if (key[i] == -1) break;
        returnString += std::to_string(key[i]);
        returnString += ' ';
    }
    returnString += ')';
    return returnString;
}

std::string cell::printGradient(velocity gradient[3]){
    std::string returnString= "( ";
    for(int i=0;i<3;i++){
        returnString += std::to_string(gradient[i].x);
        returnString += " ";
        returnString += std::to_string(gradient[i].y);
        returnString += " ";
        returnString += std::to_string(gradient[i].z);
        returnString += " ";
    }
    returnString += ")\n";
    return returnString;
}
std::string cell::printQ(double q_crit){
    std::string returnString= "( ";
    returnString += std::to_string(q_crit);
    returnString += ")\n";
    return returnString;
}
double cell::trace_base(velocity gradient[3]){
    double return_value=0;
    return_value+=gradient[0].x;
    return_value+=gradient[1].y;
    return_value+=gradient[2].z;
    return return_value;
}
double cell::trace_other(velocity gradient[3]){
    double return_value=0;
    return_value+=gradient[0].x * gradient[0].x + gradient[0].y*gradient[1].x + gradient[0].z*gradient[2].x;
    return_value+=gradient[1].x*gradient[0].y + gradient[1].y*gradient[1].y + gradient[1].z*gradient[2].y;
    return_value+=gradient[2].x*gradient[0].z +  gradient[2].y*gradient[1].z + gradient[2].z*gradient[2].z;
    return return_value;
}

double manual_abs(double d) {
    return d > 0 ? d : -d;
    //return std::fabs(d);
}

void cell::math(std::vector<cell> cells, cell_result* result){
    int keys_x[2] = {id,id};
    int keys_y[2] = {id,id};
    int keys_z[2] = {id,id};
    
    for(int i=0;i<6;i++){
        if(cells[key[i]].center.x-center.x > 0){
            keys_x[0] = key[i];
        }
        else if(cells[key[i]].center.y-center.y > 0){
            keys_y[0] = key[i];
        }
        else if(cells[key[i]].center.z-center.z > 0){
            keys_z[0] = key[i];
        }
        else if(cells[key[i]].center.x-center.x < 0){
            keys_x[1] = key[i];
        }
        else if(cells[key[i]].center.y-center.y < 0){
            keys_y[1] = key[i];
        }
        else if(cells[key[i]].center.z-center.z < 0){
            keys_z[1] = key[i];
        }
    }
    double x_distance = manual_abs(center.x-cells[keys_x[1]].center.x);
    if (x_distance<manual_abs(center.x-cells[keys_x[0]].center.x)) {x_distance = manual_abs(center.x-cells[keys_x[0]].center.x);}

    result->gradient[0] = velocity( (cells[keys_x[0]].internalVolocity.x-cells[keys_x[1]].internalVolocity.x)/(2*x_distance),
                            (cells[keys_x[0]].internalVolocity.y-cells[keys_x[1]].internalVolocity.y)/(2*x_distance),
                            (cells[keys_x[0]].internalVolocity.z-cells[keys_x[1]].internalVolocity.z)/(2*x_distance)
                        );
    double y_distance = manual_abs(center.x-cells[keys_x[1]].center.x);
    if (y_distance<manual_abs(center.x-cells[keys_x[0]].center.x)) {y_distance = manual_abs(center.x-cells[keys_x[0]].center.x);}
    result->gradient[1] = velocity( (cells[keys_y[0]].internalVolocity.x-cells[keys_y[1]].internalVolocity.x)/(2*y_distance),
                            (cells[keys_y[0]].internalVolocity.y-cells[keys_y[1]].internalVolocity.y)/(2*y_distance),
                            (cells[keys_y[0]].internalVolocity.z-cells[keys_y[1]].internalVolocity.z)/(2*y_distance)
                        );
    double z_distance = manual_abs(center.x-cells[keys_x[1]].center.x);
    if (z_distance<manual_abs(center.x-cells[keys_x[0]].center.x)) {z_distance = manual_abs(center.x-cells[keys_x[0]].center.x);}
    result->gradient[2] = velocity( (cells[keys_z[0]].internalVolocity.x-cells[keys_z[1]].internalVolocity.x)/(2*z_distance),
                            (cells[keys_z[0]].internalVolocity.y-cells[keys_z[1]].internalVolocity.y)/(2*z_distance),
                            (cells[keys_z[0]].internalVolocity.z-cells[keys_z[1]].internalVolocity.z)/(2*z_distance)
                        );

    result->q_crit = (1./2.) * std::pow(trace_base(result->gradient), 2)
                   - (1./2.) * trace_other(result->gradient);
}
#endif