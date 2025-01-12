#ifndef CONFIG_CPP
#define CONFIG_CPP
#include <ostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "config.h"
#include "debug.h"
enum pointer_type{
    PT_STRING,
    PT_INT,
    PT_BOOL,
    PT_DOUBLE,
    PT_DOUBLE_ARRAY,
    PT_DOUBLE_ARRAY_ADD,
};

struct pointer_entity
{
    enum pointer_type type;
    void* pointer;
    void* pointer_2;
    bool set;
    bool optional;
};


void load_config(Config* config){
    std::string line;
    std::string line2;

    std::string filename = "config";
    std::ifstream config_file(filename);
    assert_file_valid(config_file, filename);

    std::unordered_map <std::string, pointer_entity> map_pointers {};
    map_pointers["Name of Dataset"] =  pointer_entity {.type=PT_STRING,.pointer = &config->filePath, .set=false, .optional=false};
    map_pointers["Debug mode one"] =  pointer_entity {.type=PT_INT,.pointer = &config->debug_one, .set=false, .optional=false};
    map_pointers["Debug mode two"] =  pointer_entity {.type=PT_INT,.pointer = &config->debug_two, .set=false, .optional=false};
    map_pointers["Debug mode three"] =  pointer_entity {.type=PT_INT,.pointer = &config->debug_three, .set=false, .optional=false};
    map_pointers["Debug mode one stepping one"] = pointer_entity {.type=PT_INT,.pointer = &config->debug_one_step_one, .set=false, .optional=false};
    map_pointers["Debug mode one stepping two"] = pointer_entity {.type=PT_INT,.pointer = &config->debug_one_step_two, .set=false, .optional=false};
    map_pointers["Debug mode one stepping three"] = pointer_entity {.type=PT_INT,.pointer = &config->debug_one_step_three, .set=false, .optional=false};
    map_pointers["Use threads"] = pointer_entity {.type=PT_BOOL,.pointer = &config->use_threads, .set=false, .optional=false};
    map_pointers["Thread count"] = pointer_entity {.type=PT_INT,.pointer = &config->thread_count, .set=false, .optional=false};
    map_pointers["Start time"] = pointer_entity {.type=PT_DOUBLE,.pointer = &config->start_time, .set=false, .optional=false};
    map_pointers["Increment for threads(above 25000 cells)"] = pointer_entity {.type=PT_DOUBLE,.pointer=&config->increment, .set=false, .optional=false};
    map_pointers["Calculate these time steps base"] = pointer_entity {.type=PT_DOUBLE_ARRAY,.pointer=&config->calculate_steps,.pointer_2=&config->numberCalculateSteps, .set=false, .optional=false};//given in the following format--> start_value \n end_value \n increments
    map_pointers["Calculate these time steps add"] = pointer_entity {.type=PT_DOUBLE_ARRAY_ADD,.pointer=&config->calculate_steps,.pointer_2=&config->numberCalculateSteps, .set=false, .optional=false};//given in the following format--> start_value \n end_value \n increments
    std::string* temp_string_pointer;
    int* temp_int8_t_pointer;
    double* temp_double_pointer;
    bool* temp_bool_pointer;
    double start;
    double end;
    double increm;
    
    while(std::getline(config_file,line)){
        std::cout << LOC << "parsing cfg key '" << line << "'" << std::endl;
        if(line == "<<END>>"){
            return;
        }
        if(!std::getline(config_file,line2)){
            panic("Unexpected EOF while parsing config");
        }
        if (line.back() != ':') {
            panic("Expect key '" << line << "' to end with ':'");
        }
        line.pop_back();
        if (map_pointers.count(line) == 0) { // key doesnt exist
            panic("Invalid config key '" << line << "'");
        }
        if (map_pointers[line].set) {
            panic("Config key '" << line << "' is set already");
        }
        map_pointers[line].set = true;
        switch (map_pointers[line].type)
        {
        case PT_STRING:
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
        case PT_BOOL:
            temp_bool_pointer = (bool*) map_pointers[line].pointer;
            if (line2 != "true" && line2 != "false") panic("Invalid argument for '" << line << "': '" << line2 << "'.\nExpected [true|false]");
            *temp_bool_pointer = line2 == "true";
            break;
        case PT_DOUBLE:
            temp_double_pointer = (double*) map_pointers[line].pointer;
            try {
                *temp_double_pointer = std::stod(line2);
            } catch(...) {
                panic("Invalid argument for '" << line << "': '" << line2 << "'.\nExpected <double>");
            }
            break;
        case PT_DOUBLE_ARRAY:
            temp_double_pointer = *(double**) map_pointers[line].pointer;
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
            temp_double_pointer[0] = start;
            temp_double_pointer[1] = end;
            temp_double_pointer[2] = increm;
            *temp_int8_t_pointer = 1;
            break;
        case PT_DOUBLE_ARRAY_ADD:
            temp_double_pointer = *(double**) map_pointers[line].pointer;
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
            double* temp_pointer;
            temp_double_pointer[0+2*(*temp_int8_t_pointer)] = start;
            temp_double_pointer[1+2*(*temp_int8_t_pointer)] = end;
            temp_double_pointer[2+2*(*temp_int8_t_pointer)] = increm;
            *temp_int8_t_pointer += 1;
            break;
        default:
            panic("Unreachable");
        }
    }
    for (const auto & [key, value] : map_pointers) {
        // value is mandarory but wasnt set?
        if (!value.set && !value.optional) {
            panic("Config key '" << key << "' was never set");
        }
    }
    config_file.close();
};
#endif