#ifndef LOAD_VOLOCITY_CPP
#define LOAD_VOLOCITY_CPP
#include "cell.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "volocity.h"
/*
---------------------------------------------------------------------------

helper functions go down here
*/

bool toBeRemoved_LOCAL(char c)
{
    switch(c)
    {
    case '(':
    case ')':
        return true;
    default:
        return false;
    }
}

/*
---------------------------------------------------------------------------

end of helper functions
*/

int volocityLoad(std::unordered_map<int,cell> map,double timeStep,std::string filePath){
    std::string fileNumber = std::to_string(timeStep);
    std::string fileNumberExtracted = "";
    int a=0;
    bool is_integer=false;
    while(fileNumber[a]!='.'){
        fileNumberExtracted+=fileNumber[a];
        a++;
        if(a>fileNumber.length()){
            is_integer = true;
            break;
        }
    }
    if(is_integer){
        fileNumberExtracted=fileNumber;
    }else{
        fileNumberExtracted+=fileNumber[a];//(adds the dot)
        a++;
        while(fileNumber[a]!='0'){
            fileNumberExtracted+=fileNumber[a];
            a++;
        }
    }
    filePath += "/";
    filePath += fileNumberExtracted;
    filePath += "/U";
    std::cout<<filePath<<std::endl;
    std::ifstream volocity_file(filePath);
    std::string line;
    int number_of_entries=-1;
    for(int i=0;i<21;i++){
        std::getline(volocity_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(volocity_file,line);
    if(volocity_file.is_open()==false){
        std::cout<<"FILE FAILED TO OPEN"<<std::endl;
        std::cin>>line;
    }
    std::cout<<line<<std::endl;
    number_of_entries = std::stoi(line);
    std::getline(volocity_file,line);//gets rid of a unwanted line
    for(int i=0;i<number_of_entries;i++){
        std::getline(volocity_file,line);//get line
        //remmove brakets  ex: "(0 0 0)"" --> "0 0 0 "
        line.erase(std::remove_if(line.begin(),line.end(),&toBeRemoved_LOCAL),line.end());
        line += ' ';

        //seperate into bits ex: "0 0 0" -> 3 values in a array
        double values[3];
        int a = 0;
        std::string tempString = "";
        int debug;
        int j=0;
        while(j<2){
            while(a<line.size()){
                if(line[a]==' '){
                    values[j]=std::stod(tempString);
                    tempString="";
                    j++;
                    
                }else{
                    tempString += line[a];
                }
                a++;
            }
            a=0;
        }
        map[i].internalVolocity = volocity(values[0],values[1],values[2]);
    }
    volocity_file.close();
    std::cout << "Done loading volocities" <<std::endl;
    return 0;
};

#endif