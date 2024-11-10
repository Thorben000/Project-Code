#include <iostream>
#include <unordered_map>
#include "point.h"
#include "face.h"//this is a surface ^^
#include <fstream>
#include <algorithm>
#include "config.h"
#include "volocity.h"
#include "cell.h"

/*
---------------------------------------------------------------------------

helper functions go down here
*/

bool toBeRemoved(char c)
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


/*
Exit codes: 
---------------------------------------------------------------------------
x --> single digit numbers are for loading time errors
xx -> double digit numbers are for run time errors
---------------------------------------------------------------------------
1 --> Error during config loading
*/

int main(int, char**){
    std::string filePath;
    int debug_one;//this exists so we can enable debug mode for the first milestone
    int debug_one_step_one = 0;//this is unaccessable via config as I don't see a reason for it to be accessable as I only used it as step by step debuging!!!
    int debug_one_step_two = 0;
    int debug_one_step_three = 0;
    int debug_two;//this exists so we can enable debug mode for the second milestone
    int debug_three;//this exists so we can enable debug mode for the third milestone
    int cell_amount=-1;
    int face_amount=-1;
    std::string dummy;
    if(config(&filePath,&debug_one,&debug_two,&debug_three) == 1){
        std::cout << "There was a error while loading the config file." << std::endl << "Enter anything to close the program." << std::endl;
        std::cin;
        return 1;
    }
    std::cout << "Config loaded successfully" << std::endl;
    /*First we read the points into the system*/
    std::unordered_map<int,point> point_map;
    std::ifstream point_file(filePath+"/constant/polyMesh/points");
    std::string line;
    int number_of_entries=-1;
    for(int i=0;i<19;i++){
        std::getline(point_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(point_file,line);
    number_of_entries = std::stoi(line);
    std::getline(point_file,line);//gets rid of a unwanted line
    for(int i=0;i<number_of_entries;i++){
        std::getline(point_file,line);//get line
        //remmove brakets  ex: "(0 0 0)"" --> "0 0 0 "
        line.erase(std::remove_if(line.begin(),line.end(),&toBeRemoved),line.end());
        line += ' ';
        if(debug_one == 1){
            std::cout<<"Line after processing (points) "<< line << std::endl;
        }
        //seperate into bits ex: "0 0 0" -> 3 values in a array
        double values[3];
        int a = 0;
        std::string tempString = "";
        int debug;
        int j=0;
        while(j<2){
            while(a<line.size()){
                if(debug_one==1){
                    std::cout <<"current character:"<< line[a]<<std::endl<<"Current value of a: " << a<<std::endl;
                }
                if(line[a]==' '){
                    if(debug_one==1){
                        std::cout <<"trying to convert: "<< tempString << std::endl;
                        std::cout << std::endl << " current value[" << j <<"] = "<< values[j] <<std::endl;
                        if(debug_one_step_one==1){
                            std::cin>>a;
                        }
                    }
                    values[j]=std::stod(tempString);
                    
                    
                    if(debug_one==1){
                        std::cout <<"the conversion: "<< std::stod(tempString) << std::endl << " new value[" << j <<"] = "<< values[j] <<std::endl<<" the temp string was reste to: |"<<tempString<<std::endl;
                        if(debug_one_step_one==1){
                            std::cin>>a;
                        }
                    }
                    tempString="";
                    j++;
                    
                }else{
                    tempString += line[a];
                    if(debug_one==1){
                        std::cout <<"new temp sting: |"<< tempString<< std::endl;
                    }

                }
                a++;
            }
            a=0;
        }
        //create point 
        point_map[i] = point(values[0],values[1],values[2]);
        if(debug_one == 1){
            std::cout<<"Point object: x: "<< point_map[i].x << " y: "<< point_map[i].y << " z: "<< point_map[i].z << std::endl;
        }
    }
    point_file.close();
    std::cout << "Done with the points" <<std::endl;
    /*
    
    Second we create the faces

    */
    std::unordered_map<int,face> face_map;
    std::ifstream face_file(filePath+"/constant/polyMesh/faces");
    number_of_entries=-1;
    for(int i=0;i<19;i++){
        std::getline(face_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(face_file,line);
    number_of_entries = std::stoi(line);
    face_amount = number_of_entries;
    std::getline(face_file,line);//gets rid of a unwanted line
    for(int i=0;i<number_of_entries;i++){
        std::getline(face_file,line);//get line
        //remmove brakets  ex: "4(0 0 0 0)"" --> "0 0 0 0 "
        line[0]='(';//replaces the 4 in the dataset with a ' ' to fix a issue
        line.erase(std::remove_if(line.begin(),line.end(),&toBeRemoved),line.end());
        line += ' ';
        if(debug_one == 1){
            std::cout<<"Line after processing (face) "<< line << std::endl;
        }
        //seperate into bits ex: "0 0 0 0" -> 4 values in a array
        double values[3];
        int a = 0;
        std::string tempString = "";
        int debug;
        int j=0;
        while(j<3){
            while(a<line.size()){
                if(debug_one==1){
                    std::cout <<"current character:"<< line[a]<<std::endl<<"Current value of a: " << a<<std::endl;
                }
                if(line[a]==' '){
                    if(debug_one==1){
                        std::cout <<"trying to convert: "<< tempString << std::endl;
                        std::cout << std::endl << " current value[" << j <<"] = "<< values[j] <<std::endl;
                        if(debug_one_step_two==1){
                            std::cin>>a;
                        }
                    }
                    values[j]=std::stoi(tempString);
                    
                    
                    if(debug_one==1){
                        std::cout <<"the conversion: "<< std::stoi(tempString) << std::endl << " new value[" << j <<"] = "<< values[j] <<std::endl<<" the temp string was reste to: |"<<tempString<<std::endl;
                        if(debug_one_step_two==1){
                            std::cin>>a;
                        }
                    }
                    tempString="";
                    j++;
                    
                }else{
                    tempString += line[a];
                    if(debug_one==1){
                        std::cout <<"new temp sting: |"<< tempString<< std::endl;
                    }

                }
                a++;
            }
            a=0;
        }
        //create point 
        face_map[i] = face(point_map[values[0]],point_map[values[1]],point_map[values[2]],point_map[values[3]]);
        if(debug_one == 1){
            std::cout<<"face corner IDs: "<< values[0] << " , "<< values[1] << " , "<< values[2] << " , "<< values[3]<< std::endl;
        }
    }
    face_file.close();
    std::cout << "Done with the faces" <<std::endl;
    /*
    
    then we read the face ownerships
    
    */
    std::ifstream face_file(filePath+"/constant/polyMesh/owner");
    number_of_entries=-1;
    for(int i=0;i<20;i++){
        std::getline(face_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(face_file,line);
    number_of_entries = std::stoi(line);
    std::getline(face_file,line);//gets rid of a unwanted line
    int value = 0;
    for(int i=0;i<number_of_entries;i++){
        std::getline(face_file,line);
        if(debug_one==1){
            std::cout <<"Atempting to convert to int:"<< line <<std::endl;
            if(debug_one_step_three==1){
                int a;
                std::cin>>a;
            }
        }
        value = std::stoi(line);
        
        
        face_map[i].owners[0]= value;
        if(debug_one==1){
            std::cout <<"Set cell id "<< value <<" as owner of face" <<std::endl;
            if(debug_one_step_three==1){
                int a;
                std::cin>>a;
            }
        }
    }
    face_file.close();
    std::cout << "Done with the face ownership" <<std::endl;
    /*
    
    then we read the face nighbours
    
    */
   std::ifstream face_file(filePath+"/constant/polyMesh/neighour");
    number_of_entries=-1;
    for(int i=0;i<20;i++){
        std::getline(face_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(face_file,line);
    number_of_entries = std::stoi(line);
    std::getline(face_file,line);//gets rid of a unwanted line
    int value = 0;
    for(int i=0;i<number_of_entries;i++){
        std::getline(face_file,line);
        if(debug_one==1){
            std::cout <<"Atempting to convert to int:"<< line <<std::endl;
            if(debug_one_step_three==1){
                int a;
                std::cin>>a;
            }
        }
        value = std::stoi(line);
        
        
        face_map[i].owners[1]= value;
        if(debug_one==1){
            std::cout <<"Set cell id "<< value <<" as owner of face" <<std::endl;
            if(debug_one_step_three==1){
                int a;
                std::cin>>a;
            }
        }
    }
    face_file.close();
    std::cout << "Done with the face neighbours" <<std::endl;
    /*
    
    now make the cells 
    
    */
   std::unordered_map<int,cell> cell_map;
    for(int i=0;i<face_amount;i++){
        if(cell_map[face_map[i].owners[0]].exists || cell_map[face_map[i].owners[1]].exists){
            cell_map[face_map[i].owners[0]].addFace(&face_map[i]);
        }else{
            cell_map[face_map[i].owners[0]].addFace(&face_map[i]);
            cell_amount++;
        }
    }
   /*
   
   now make the cells determine there nighbours and cell centers
   
   */
   for(int i=0;i<cell_amount;i++){
        cell_map[i].determineCenter();
        cell_map[i].determineNeighbours(cell_map);
   }
   /*
   
   Now load volocities and begin simulation maths
   
   */
   
    
    std::cin>>dummy;
    return 0;
}



