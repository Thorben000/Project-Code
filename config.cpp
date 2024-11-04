#ifndef CONFIG_CPP
#define CONFIG_CPP
#include <string>
#include <unordered_map>
#include <fstream>
#include "config.h"
enum pointer_type{
    PT_STRING,
    PT_INT,
};

struct pointer_entitie
{
    enum pointer_type type;
    void* pointer;
};


int config(std::string* file_path,int* debug_one,int* debug_two,int* debug_three){
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
        std::string* temp_string_pointer;
        int* temp_int8_t_pointer;
        if (config_file.is_open()){
            while(std::getline(config_file,line)){
                if(line == "<<END>>"){
                    return 0;
                }
                if(std::getline(config_file,line2)){
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