#include <iostream>
#include <unordered_map>
#include "point.h"
#include "face.h"//this is a surface ^^
#include <fstream>
#include <algorithm>
#include "config.h"
#include "volocity.h"
#include "cell.h"
#include <thread>

/*
---------------------------------------------------------------------------

helper functions go down here
*/

void printValues(std::unordered_map<int,cell> map, int cellamount){
    std::cout<<"Amount cells: "<<cellamount<<std::endl;
    for(int i=0;i<cellamount;i++){
        std::cout<<"#####################################"<<std::endl<<"Cell ID: "<<i<<std::endl<<"Cell center: "<<map[i].printCenter()<<std::endl<<"Cell internal volocity: "<<map[i].printInternalVolocity()<<std::endl;
        std::cout<<"Neighbours: "<<map[i].printNeighbours()<<std::endl/*<<"Faces: "<<map[i].printFaces()<<std::endl<<"Corners: "<<map[i].printCorners()<<std::endl*/;
    }
}

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
void threadableMath(int cell_amount_thread,int cell_start_thread,int threadNR,int debug_one,std::unordered_map<int,cell> map){
    for(int i=cell_start_thread;i<cell_amount_thread;i++){
        if(map[i].exists){
            map[i].determineCenter();
            map[i].determineNeighbours(map);
            if(debug_one){
                std::cout<<"Run: "<<i<<"Of Thread NR."<<threadNR<<std::endl;
            }  
        }
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
    int debug_one_step_one;//this is unaccessable via config as I don't see a reason for it to be accessable as I only used it as step by step debuging!!!
    int debug_one_step_two;
    int debug_one_step_three;
    int debug_two;//this exists so we can enable debug mode for the second milestone
    int debug_three;//this exists so we can enable debug mode for the third milestone

    double startTime;

    int cell_amount=0;
    int face_amount=0;

    double increment = 0.01;

    std::string dummy;
    if(config(&filePath,&debug_one,&debug_two,&debug_three,&startTime,&debug_one_step_one,&debug_one_step_two,&debug_one_step_three,&increment) == 1){
        std::cout << "There was a error while loading the config file." << std::endl << "Enter anything to close the program." << std::endl;
        std::cin;
        return 1;
    }
    std::cout << "Config loaded successfully" << std::endl;
    /*

    First we read the points into the system
    
    */
    std::unordered_map<int,point> point_map;
    std::ifstream point_file(filePath+"/constant/polyMesh/points");
    std::string line="ERROR";
    int number_of_entries=-1;
    for(int i=0;i<19;i++){
        std::getline(point_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(point_file,line);
    std::cout << filePath<<std::endl;
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
                            std::cin>>dummy;
                        }
                    }
                    values[j]=std::stod(tempString);
                    
                    
                    if(debug_one==1){
                        std::cout <<"the conversion: "<< std::stod(tempString) << std::endl << " new value[" << j <<"] = "<< values[j] <<std::endl<<" the temp string was reste to: |"<<tempString<<std::endl;
                        if(debug_one_step_one==1){
                            std::cin>>dummy;
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
        point_map[i].id=i;
        if(debug_two == 1){
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
        double values[4];
        int a = 0;
        std::string tempString = "";
        int debug;
        int j=0;
        while(j<4){
            while(a<line.size()&&j<4){
                if(debug_one==1){
                    std::cout <<"current character:"<< line[a]<<std::endl<<"Current value of a: " << a<<std::endl;
                }
                if(line[a]==' '){
                    if(debug_one==1){
                        std::cout <<"trying to convert: "<< tempString << std::endl;
                        std::cout << std::endl << " current value[" << j <<"] = "<< values[j] <<std::endl;
                        if(debug_one_step_two==1){
                            std::cin>>dummy;
                        }
                    }
                    if(tempString==" "){
                        std::cout<<"SOMETHING MESSED UP"<<std::endl;
                        std::cin >> dummy;
                        return 1;
                    }
                    values[j]=std::stoi(tempString);
                    
                    
                    if(debug_one==1){
                        std::cout <<"the conversion: "<< std::stoi(tempString) << std::endl << " new value[" << j <<"] = "<< values[j] <<std::endl<<" the temp string was reste to: |"<<tempString<<std::endl;
                        if(debug_one_step_two==1){
                            std::cin>>dummy;
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
        face_map[i].id = i;
        if(debug_one == 1){
            std::cout<<"face corner IDs: "<< values[0] << " , "<< values[1] << " , "<< values[2] << " , "<< values[3]<< std::endl;
            
        }
    }
    /*
    for(int i=0;i<number_of_entries;i++){
        std::cout<<face_map[i].printCorners()<<std::endl;
    }*/
    face_file.close();
    std::cout << "Done with the faces" <<std::endl;
    /*
    
    then we read the face ownerships

    */
    std::ifstream face_file_o(filePath+"/constant/polyMesh/owner");
    number_of_entries=-1;
    for(int i=0;i<20;i++){
        std::getline(face_file_o,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(face_file_o,line);
    number_of_entries = std::stoi(line);
    std::getline(face_file_o,line);//gets rid of a unwanted line
    int value = 0;
    for(int i=0;i<number_of_entries;i++){
        std::getline(face_file_o,line);
        if(debug_one==1){
            std::cout <<"Atempting to convert to int:"<< line <<std::endl;
            if(debug_one_step_three==1){
                int a;
                std::cin>>dummy;
            }
        }
        value = std::stoi(line);
        
        
        face_map[i].owners[0]= value;
        if(debug_one==1){
            std::cout<< std::endl <<"Set cell id "<< value <<" as owner of face id"<< i <<std::endl;
            if(debug_one_step_three==1){
                int a;
                std::cin>>a;
            }
        }
    }
    face_file_o.close();
    std::cout << "Done with the face ownership" <<std::endl;
    /*
    
    then we read the face nighbours
    
    */

    std::ifstream face_file_n(filePath+"/constant/polyMesh/neighbour");
    number_of_entries=-1;
    for(int i=0;i<20;i++){
        std::getline(face_file_n,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(face_file_n,line);
    number_of_entries = std::stoi(line);
    std::getline(face_file_n,line);//gets rid of a unwanted line
    int value_i = 0;
    for(int i=0;i<number_of_entries;i++){
        std::getline(face_file_n,line);
        if(debug_one==1){
            std::cout <<"Atempting to convert to int:"<< line <<std::endl;
            if(debug_one_step_three==1){
                std::cin>>dummy;
            }
        }
        value_i = std::stoi(line);
        line = "-1";
        
        face_map[i].owners[1]= value_i;
        if(debug_one==1){
            std::cout<< std::endl <<"Set cell id "<< value_i <<" as owner of face id"<< i <<std::endl;
            if(debug_one_step_three==1){
                std::cin>>dummy;
            }
        }
    }
    face_file_n.close();
    std::cout << "Done with the face neighbours" <<std::endl;
    /*
    
    now make the cells 
    
    */
   std::unordered_map<int,cell> cell_map;
    for(int i=0;i<face_amount;i++){
        bool tempbool=false;
        if(debug_one){
            std::cout<<std::endl<<"#####################################"<<std::endl<<"Showing face ID:"<<i<<std::endl<<"Owner 0: "<<face_map[i].owners[0]<<std::endl;
            std::cout<<"Owner 1: "<<face_map[i].owners[1]<<std::endl;
        }
        if(cell_map[face_map[i].owners[0]].exists && face_map[i].owners[0]!= -1){
            cell_map[face_map[i].owners[0]].addFace(face_map[i]);
            if(debug_one){
                std::cout<<"Declared additional face for Cell:"<<face_map[i].owners[0]<<std::endl;
            }
        }else{
            if(face_map[i].owners[0]!= -1){
                cell_map[face_map[i].owners[0]] = cell();
                cell_map[face_map[i].owners[0]].id = cell_amount;
                cell_map[face_map[i].owners[0]].addFace(face_map[i]);
                tempbool=true;
                cell_amount++;
                if(debug_one){
                std::cout<<"Declared Cell:"<<face_map[i].owners[0]<<std::endl;
                }
            }
        }
        if ( cell_map[face_map[i].owners[1]].exists && face_map[i].owners[1]!= -1)
        {
           cell_map[face_map[i].owners[1]].addFace(face_map[i]);
           if(debug_one){
                std::cout<<"Declared additional face for Cell:"<<face_map[i].owners[1]<<std::endl;
            }
        }else{
            if(face_map[i].owners[1]!= -1){
                cell_map[face_map[i].owners[1]] = cell();
                cell_map[face_map[i].owners[1]].id = cell_amount;
                cell_map[face_map[i].owners[1]].addFace(face_map[i]);
                tempbool=true;
                cell_amount++;
                if(debug_one){
                    std::cout<<"Declared Cell:"<<face_map[i].owners[1]<<std::endl;
                }
            }    
        }
    }
    std::cout <<"Done with cell creation"<<std::endl;
    std::cout<<"cell amount:"<<cell_amount<<std::endl;
   /*
   
   now make the cells determine there nighbours and cell centers

   we are using threading as is speeds up the loading
   first we spread the load based on the increments
   
   */
  if(cell_amount>25000){
    
    int thread_load=cell_amount*increment;//int rounding is intended
    int num_threads = cell_amount/thread_load;
    int completed_cells=0;
    if(cell_amount%thread_load>0){
        num_threads++;
    }
    std::cout<<"trying to create Threads number:"<<num_threads<<std::endl;
    std::thread* threads[num_threads];

    for(int i=0;i<num_threads;i++){
        threads[i] = new std::thread(threadableMath,completed_cells,i*thread_load,i,debug_one,cell_map);
    }
    std::cout<<"Threads created"<<std::endl;
    for(int i=0;i<num_threads;i++){
        threads[i]->join();
    }
  }else{
    double passed_increments = 0.0;
   for(int i=0;i<cell_amount;i++){
        cell_map[i].determineCenter();
        cell_map[i].determineNeighbours(cell_map);
        if((i*1.0)/cell_amount-increment-passed_increments>0.0){
            passed_increments+=increment;
            std::cout<<"Cell math done to "<<(i*1.0)/cell_amount<<std::endl;
        }
        if(debug_one){
            std::cout<<"Run: "<<i<<std::endl;
        }  
   }
  }
   std::cout <<"Done with cell computations"<<std::endl;
   /*
   
   Now load volocities and begin simulation maths
   
   */
    //volocityLoad(&cell_map,startTime,filePath);
    std::string fileNumber = std::to_string(startTime);
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
    std::cin>>dummy;
    std::ifstream volocity_file(filePath);
    line="";
    number_of_entries=-1;
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
        line.erase(std::remove_if(line.begin(),line.end(),toBeRemoved),line.end());
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
        cell_map[i].internalVolocity = volocity(values[0],values[1],values[2]);
    }
    volocity_file.close();
    std::cout << "Done loading volocities" <<std::endl;

    if(debug_two){
        printValues(cell_map,cell_amount);
    }
   
    
    std::cin>>dummy;
    return 0;
}



