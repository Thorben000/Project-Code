#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <vector>

struct Config {
    std::string filePath;
    int debug_one;
    int debug_two;
    int debug_three;
    int debug_one_step_one;
    int debug_one_step_two;
    int debug_one_step_three;
    bool use_threads;
    int thread_count;
    bool time_step_one;
    bool time_step_ten;
    double start_time;
    double increment;
    std::vector<double> calculate_steps;
    int numberCalculateSteps;
    /*
    
    class used as a wrapper class(a thing that just saves a lot of data) for all config options

    */
};

void load_config(Config* config);

#endif