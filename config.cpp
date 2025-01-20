#ifndef CONFIG_CPP
#define CONFIG_CPP
#include <ostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <vector>
#include "config.h"
#include "debug.h"
enum pointer_type{
    PT_STRING,
    PT_INT,
    PT_BOOL,
    PT_DOUBLE,
    PT_DOUBLE_VECTOR,
};

enum pointer_flags {
    PF_NONE  = 0,      // no flags
    PF_MULTI = 1 << 0, // multiple entries allowed
    PF_OPT   = 1 << 1, // is optional
    PF_SET   = 1 << 2, // has been set
};

struct pointer_entity
{
    enum pointer_type type;
    void* pointer;
    void* pointer_2;
    int flags;
};


void load_config(Config* config){
    config->calculate_steps = std::vector<double>();
    std::string line;
    std::string line2;

    std::string filename = "config";
    std::ifstream config_file(filename);
    assert_file_valid(config_file, filename);
    /*
    
    Declaration of all inputs, the string is what is needed to be writen in config first followed by the declaration of the pointer entity(a struct used to later asign the values)
    
    */
    std::unordered_map <std::string, pointer_entity> map_pointers {};
    map_pointers["Name of Dataset"] =  pointer_entity {.type=PT_STRING,.pointer = &config->filePath, .flags = PF_NONE};
    map_pointers["Debug mode one"] =  pointer_entity {.type=PT_INT,.pointer = &config->debug_one, .flags = PF_NONE};
    map_pointers["Debug mode two"] =  pointer_entity {.type=PT_INT,.pointer = &config->debug_two, .flags = PF_NONE};
    map_pointers["Debug mode three"] =  pointer_entity {.type=PT_INT,.pointer = &config->debug_three, .flags = PF_NONE};
    map_pointers["Debug mode one stepping one"] = pointer_entity {.type=PT_INT,.pointer = &config->debug_one_step_one, .flags = PF_NONE};
    map_pointers["Debug mode one stepping two"] = pointer_entity {.type=PT_INT,.pointer = &config->debug_one_step_two, .flags = PF_NONE};
    map_pointers["Debug mode one stepping three"] = pointer_entity {.type=PT_INT,.pointer = &config->debug_one_step_three, .flags = PF_NONE};
    map_pointers["Use threads"] = pointer_entity {.type=PT_BOOL,.pointer = &config->use_threads, .flags = PF_NONE};
    map_pointers["Calc 1"] = pointer_entity {.type=PT_BOOL,.pointer = &config->time_step_one, .flags = PF_NONE};
    map_pointers["Calc 10"] = pointer_entity {.type=PT_BOOL,.pointer = &config->time_step_ten, .flags = PF_NONE};
    map_pointers["Thread count"] = pointer_entity {.type=PT_INT,.pointer = &config->thread_count, .flags = PF_NONE};
    map_pointers["Start time"] = pointer_entity {.type=PT_DOUBLE,.pointer = &config->start_time, .flags = PF_NONE};
    map_pointers["Increment for threads(above 25000 cells)"] = pointer_entity {.type=PT_DOUBLE,.pointer=&config->increment, .flags = PF_NONE};
    map_pointers["Calculate these time steps base"] = pointer_entity {.type=PT_DOUBLE_VECTOR,.pointer=&config->calculate_steps,.pointer_2=&config->numberCalculateSteps, .flags = PF_NONE};//given in the following format--> start_value \n end_value \n increments
    map_pointers["Calculate these time steps add"] = pointer_entity {.type=PT_DOUBLE_VECTOR,.pointer=&config->calculate_steps,.pointer_2=&config->numberCalculateSteps, .flags = PF_MULTI|PF_OPT};//given in the following format--> start_value \n end_value \n increments
    std::string* temp_string_pointer;
    int* temp_int8_t_pointer;
    double* temp_double_pointer;
    std::vector<double>* temp_double_vector_pointer;
    bool* temp_bool_pointer;
    double start;
    double end;
    double increm;
    /*
    
    beginning of readin the config file

    */
    while(std::getline(config_file,line)){
        if(line == "<<END>>"){
            /*
            
            Ends the config

            */
            break;
        }
        if(!std::getline(config_file,line2)){
            panic("Unexpected EOF while parsing config");
            /*
            
            Error handeling

            */
        }
        if (line.back() != ':') {
            panic("Expect key '" << line << "' to end with ':'");
            /*
            
            Error handeling

            */
        }
        line.pop_back();
        std::cout << LOC << "parsing cfg key '" << line << "'" << std::endl;
        if (map_pointers.count(line) == 0) { // key doesnt exist
            panic("Invalid config key '" << line << "'");
            /*
            
            Error handeling

            */
        }
        if ((map_pointers[line].flags & PF_SET) != 0 && (map_pointers[line].flags & PF_MULTI) == 0) {
            panic("Config key '" << line << "' is set already");
            /*
            
            Error handeling

            */
        }
        map_pointers[line].flags |= PF_SET;
        switch (map_pointers[line].type)
        {
        case PT_STRING: //saves string at the coresponding position
            temp_string_pointer = (std::string*) map_pointers[line].pointer;
            *temp_string_pointer = line2;
            break;
        case PT_INT:  //converts a string to a int and saves it at the coresponding position
            temp_int8_t_pointer = (int*) map_pointers[line].pointer;
            try {
                *temp_int8_t_pointer = std::stoi(line2);
            } catch(...) {
                panic("Invalid argument for '" << line << "': '" << line2 << "'.\nExpected <int>");
            }
            break;
        case PT_BOOL://converts a string to a bool and saves it at the coresponding position
            temp_bool_pointer = (bool*) map_pointers[line].pointer;
            if (line2 != "true" && line2 != "false") panic("Invalid argument for '" << line << "': '" << line2 << "'.\nExpected [true|false]");
            *temp_bool_pointer = line2 == "true";
            break;
        case PT_DOUBLE://converts a string to a double and saves it at the coresponding position
            temp_double_pointer = (double*) map_pointers[line].pointer;
            try {
                *temp_double_pointer = std::stod(line2);
            } catch(...) {
                panic("Invalid argument for '" << line << "': '" << line2 << "'.\nExpected <double>");
            }
            break;
        case PT_DOUBLE_VECTOR://converts a string to a double and saves it at the coresponding vector
            temp_double_vector_pointer = (std::vector<double>*) map_pointers[line].pointer;
            temp_int8_t_pointer = (int*) map_pointers[line].pointer_2;
            try {
                start = std::stod(line2);
                std::getline(config_file, line);
                end = std::stod(line);
                std::getline(config_file, line);
                increm = std::stod(line);
            } catch(...) {
                panic("Invalid argument for '" << line << "': '" << line2 << "'.\nExpected double array");
            }
            temp_double_vector_pointer->push_back(start);
            temp_double_vector_pointer->push_back(end);
            temp_double_vector_pointer->push_back(increm);
            *temp_int8_t_pointer += 1;
            break;
        default:
            panic("Unreachable");//something bad happened we should NOT reach this point
        }
    }
    for (const auto & [key, value] : map_pointers) {
        // value is mandarory but wasnt set?
        if ((value.flags & PF_SET) == 0 && (value.flags & PF_OPT) == 0) {
            panic("Config key '" << key << "' was never set");
        }
    }
    config_file.close();
};
#endif