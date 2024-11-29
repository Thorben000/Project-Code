#ifndef CONFIG_CPP
#define CONFIG_CPP
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "config.h"
enum pointer_type{
    PT_STRING,
    PT_INT,
    PT_DOUBLE,
    PT_DOUBLE_ARRAY,
    PT_DOUBLE_ARRAY_ADD,
};

struct pointer_entitie
{
    enum pointer_type type;
    void* pointer;
    void* pointer_2;
};


int config(std::string* file_path,int* debug_one,int* debug_two,int* debug_three,double* start_time,int* debug_one_step_one,int* debug_one_step_two,int* debug_one_step_three,double* increment,double* calculate_steps,int* number_calculate_steps){
    try{
        std::string line;
        std::string line2;
        std::string filename = "config";
        std::ifstream config_file(filename);
        std::unordered_map <std::string,pointer_entitie> map_pointers;
        map_pointers["Name of Dataset:"] =  pointer_entitie {.type=PT_STRING,.pointer = file_path};
        map_pointers["Debug mode one:"] =  pointer_entitie {.type=PT_INT,.pointer = debug_one};
        map_pointers["Debug mode two:"] =  pointer_entitie {.type=PT_INT,.pointer = debug_two};
        map_pointers["Debug mode three:"] =  pointer_entitie {.type=PT_INT,.pointer = debug_three};
        map_pointers["Start time:"] = pointer_entitie {.type=PT_DOUBLE,.pointer = start_time};
        map_pointers["Debug mode one stepping one:"] = pointer_entitie {.type=PT_INT,.pointer = debug_one_step_one};
        map_pointers["Debug mode one stepping two:"] = pointer_entitie {.type=PT_INT,.pointer = debug_one_step_two};
        map_pointers["Debug mode one stepping three:"] = pointer_entitie {.type=PT_INT,.pointer = debug_one_step_three};
        map_pointers["Increment for threads(above 25000 cells):"] = pointer_entitie {.type=PT_DOUBLE,.pointer=increment};
        map_pointers["Calculate these time steps base:"] = pointer_entitie {.type=PT_DOUBLE_ARRAY,.pointer=calculate_steps,.pointer_2=number_calculate_steps};//given in the following format--> start_value \n end_value \n increments
        map_pointers["Calculate these time steps add:"] = pointer_entitie {.type=PT_DOUBLE_ARRAY_ADD,.pointer=calculate_steps,.pointer_2=number_calculate_steps};//given in the following format--> start_value \n end_value \n increments
        std::string* temp_string_pointer;
        int* temp_int8_t_pointer;
        double* temp_double_pointer;
        double start;
        double end;
        double increm;
        if (config_file.is_open()){
            while(std::getline(config_file,line)){
                if(line == "<<END>>"){
                    //std::cout<<"END of config"<<std::endl;
                    return 0;
                }
                if(std::getline(config_file,line2)){
                    //std::cout<<"value read:"<<line2 <<" Asigning to" << line << "At memory location " << map_pointers[line].pointer <<std::endl;
                    switch (map_pointers[line].type)
                    {
                    case PT_STRING:
                        temp_string_pointer = (std::string*) map_pointers[line].pointer;
                        *temp_string_pointer = line2;
                        break;
                    case PT_INT:  //converts a string to a int and saves it at the coresponding position
                        temp_int8_t_pointer = (int*) map_pointers[line].pointer;
                        *temp_int8_t_pointer = std::stoi(line2);
                        break;
                    case PT_DOUBLE:
                    temp_double_pointer = (double*) map_pointers[line].pointer;
                    *temp_double_pointer = std::stod(line2);
                    break;
                    case PT_DOUBLE_ARRAY:
                    temp_double_pointer = (double*) map_pointers[line].pointer;
                    temp_int8_t_pointer = (int*) map_pointers[line].pointer_2;
                    start = std::stod(line2);
                    std::getline(config_file,line);
                    end = std::stod(line);
                    std::getline(config_file,line);
                    increm = std::stod(line);
                    temp_double_pointer[0] = start;
                    temp_double_pointer[1] = end;
                    temp_double_pointer[2] = increm;
                    //std::cout<<temp_double_pointer[0]<<std::endl;
                    //std::cout<<temp_double_pointer[1]<<std::endl;
                    //std::cout<<temp_double_pointer[2]<<std::endl;
                    *temp_int8_t_pointer = 1;
                    break;
                    case PT_DOUBLE_ARRAY_ADD:
                    temp_double_pointer = (double*) map_pointers[line].pointer;
                    temp_int8_t_pointer = (int*) map_pointers[line].pointer_2;
                    start = std::stod(line2);
                    std::getline(config_file,line);
                    end = std::stod(line);
                    std::getline(config_file,line);
                    increm = std::stod(line);
                    double* temp_pointer;
                    temp_double_pointer[0+2*(*temp_int8_t_pointer)] = start;
                    temp_double_pointer[1+2*(*temp_int8_t_pointer)] = end;
                    temp_double_pointer[2+2*(*temp_int8_t_pointer)] = increm;
                    *temp_int8_t_pointer += 1;
                    break;
                    default:
                        break;
                    }
                }
                else{
                    throw 1;
                }
            }
            config_file.close();
            return 0;
        }
    }
    catch(...){
        return 1;
    }
    return 0;
};
#endif