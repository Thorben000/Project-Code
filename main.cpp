#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>

#include "point.h"
#include "face.h"//this is a surface ^^
#include <fstream>
#include <algorithm>
#include <vector>
#include "config.h"
#include "velocity.h"
#include "cell.h"
#include "debug.h"


/*
---------------------------------------------------------------------------

helper functions go down here
*/
uint64_t getTimeMS(){
    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return ms;
};
class cellMapSaver{
    public:
    std::vector<cell> cells;
    cellMapSaver(int cell_count){
        
    };
};
void printValues(std::vector<cell> cells, int cellamount){
    std::cout<<LOC<<"Amount cells: "<<cellamount<<std::endl;
    for(int i=0;i<cellamount;i++){
        std::cout<<LOC<<"#####################################"<<std::endl<<"Cell ID: "<<i<<std::endl<<"Cell center: "<<cells[i].printCenter()<<std::endl<<"Cell internal volocity: "<<cells[i].printInternalVolocity()<<std::endl;
        std::cout<<LOC<<"Neighbours: "<<cells[i].printNeighbours()<<std::endl/*<<"Faces: "<<cells[i].printFaces()<<std::endl/*<<"Corners: "<<cells[i].printCorners()<<std::endl/**/;
    }
}
void printToFile(std::vector<cell> cells, std::vector<cell_result> cell_results, std::string extracted_number,std::string base_file_path,int cell_amount){//for now only does the gradient!!!
    std::string file_path_grad_u = base_file_path + "/" + extracted_number + "/grad(U)";
    std::ofstream grad_u_file(file_path_grad_u);
    std::ifstream grad_u_template_file("templates/tensorfield.templ");
    assert_file_valid(grad_u_template_file, "templates/tensorfield.templ");
    std::string line_grad_u;
    std::string line_template_grad_u;
    //add hadder
    for(int i=0;i<13;i++){
        std::getline(grad_u_template_file,line_template_grad_u);
        line_template_grad_u+='\n';
        grad_u_file << line_template_grad_u;
    }
    std::getline(grad_u_template_file,line_template_grad_u);
    line_template_grad_u += extracted_number;
    line_template_grad_u += '\n';
    grad_u_file << line_template_grad_u;
    for(int i=0;i<8;i++){
        std::getline(grad_u_template_file,line_template_grad_u);
        if(i<7)line_template_grad_u+='\n';
        grad_u_file << line_template_grad_u;
    }
    line_grad_u = std::to_string(cell_amount)+"\n(\n";
    grad_u_file << line_grad_u;
    for(int i=0;i<cell_amount;i++){
        grad_u_file << cells[i].printGradient(cell_results[i].gradient);
    }
    line_grad_u = ")\n;\n";
    grad_u_file << line_grad_u;
    for(int i=0;i<27;i++){
        std::getline(grad_u_template_file,line_template_grad_u);
        line_template_grad_u+='\n';
        grad_u_file << line_template_grad_u;
    }
    return;
}
void printToFileQ(std::vector<cell> cells, std::vector<cell_result> celL_results, std::string extracted_number,std::string base_file_path,int cell_amount){//for now only does the Q!!!
    std::string file_path_grad_u = base_file_path + "/" + extracted_number + "/Q";
    std::ofstream grad_u_file(file_path_grad_u);
    std::ifstream grad_u_template_file("templates/QCriterium.templ");
    assert_file_valid(grad_u_template_file, "templates/QCriterium.templ");
    std::string line_grad_u;
    std::string line_template_grad_u;
    //add hadder
    for(int i=0;i<13;i++){
        std::getline(grad_u_template_file,line_template_grad_u);
        line_template_grad_u+='\n';
        grad_u_file << line_template_grad_u;
    }
    std::getline(grad_u_template_file,line_template_grad_u);
    line_template_grad_u += extracted_number;
    line_template_grad_u += '\n';
    grad_u_file << line_template_grad_u;
    for(int i=0;i<8;i++){
        std::getline(grad_u_template_file,line_template_grad_u);
        if(i<7)line_template_grad_u+='\n';
        grad_u_file << line_template_grad_u;
    }
    line_grad_u = std::to_string(cell_amount)+"\n(\n";
    grad_u_file << line_grad_u;
    for(int i=0;i<cell_amount;i++){
        grad_u_file << cells[i].printQ(celL_results[i].q_crit);
    }
    line_grad_u = ")\n;\n";
    grad_u_file << line_grad_u;
    for(int i=0;i<27;i++){
        std::getline(grad_u_template_file,line_template_grad_u);
        line_template_grad_u+='\n';
        grad_u_file << line_template_grad_u;
    }
    return;
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

void threadableMath(int cell_start_thread, int cell_end_thread,int threadNR, uint64_t start_time, std::vector<cell> cells, std::vector<cell_result>* results){
    int portion_of_then = 1;
    for(int i=cell_start_thread;i<cell_end_thread;i++){
        if(cells[i].id != -1){
            //cellMap->cells[i].determineCenter();
            //cellMap->cells[i].determineNeighbours(cellMap->cells);
            cells[i].math(cells, &(*results)[i]);
            if((float)(i-cell_start_thread)/(cell_end_thread-cell_start_thread) >= 0.001*portion_of_then){
                float t = (getTimeMS()-start_time) / 1000.0;
                float eta = (t / portion_of_then) * (1000 - portion_of_then);
                std::cout<<LOC<<"Thread "<<threadNR<<": "<<portion_of_then * 0.1 << "% in " << t <<"s (ETA in " << eta << "s)"<<std::endl;
                portion_of_then += 1;
            }
        }
   }
   std::cout<<LOC<<"Thread " <<threadNR<<" completed"<<std::endl;
}

/*
---------------------------------------------------------------------------

end of helper functions
*/

int main(int, char**){
    double startTime;

    int cell_amount=0;
    int face_amount=0;

    Config config {};
    load_config(&config);

    std::cout <<LOC << "Config loaded successfully with path '" << config.filePath << "'" << std::endl;

    std::cout << LOC << "Calc number entries is: " << config.numberCalculateSteps<<" content:"<<std::endl;
    for(int i=0;i<config.numberCalculateSteps*3;i++){
        std::cout<<LOC<<"  "<<(int)i/3+1<<". "<<config.calculate_steps[i]<<std::endl;
    }
    

    /*

    First we read the points into the system
    
    */
    std::unordered_map<int,point> point_map;
    std::string point_file_path = config.filePath+"/constant/polyMesh/points";
    std::ifstream point_file(point_file_path);
    assert_file_valid(point_file, point_file_path);
    std::string line;
    int number_of_entries=-1;
    for(int i=0;i<19;i++){
        std::getline(point_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(point_file,line);
    std::cout <<LOC<< config.filePath<<std::endl;
    number_of_entries = std::stoi(line);
    std::getline(point_file,line);//gets rid of a unwanted line
    for(int i=0;i<number_of_entries;i++){
        std::getline(point_file,line);//get line
        //remmove brakets  ex: "(0 0 0)"" --> "0 0 0 "
        line.erase(std::remove_if(line.begin(),line.end(),&toBeRemoved),line.end());
        line += ' ';
        if(config.debug_one == 1){
            std::cout<<LOC<<"Line after processing (points) "<< line << std::endl;
        }
        //seperate into bits ex: "0 0 0" -> 3 values in a array
        double values[3];
        int a = 0;
        std::string tempString = "";
        int debug;
        int j=0;
        while(j<2){
            while(a<line.size()){
                if(config.debug_one==1){
                    std::cout <<LOC<<"current character:"<< line[a]<<std::endl<<"Current value of a: " << a<<std::endl;
                }
                if(line[a]==' '){
                    if(config.debug_one==1){
                        std::cout <<LOC<<"trying to convert: "<< tempString << std::endl;
                        std::cout <<LOC << " current value[" << j <<"] = "<< values[j] <<std::endl;
                    }
                    values[j]=std::stod(tempString);
                    
                    
                    if(config.debug_one==1){
                        std::cout <<LOC<<"the conversion: "<< std::stod(tempString) << std::endl << " new value[" << j <<"] = "<< values[j] <<std::endl<<" the temp string was reste to: |"<<tempString<<std::endl;
                    }
                    tempString="";
                    j++;
                    
                }else{
                    tempString += line[a];
                    if(config.debug_one==1){
                        std::cout <<LOC<<"new temp sting: |"<< tempString<< std::endl;
                    }

                }
                a++;
            }
            a=0;
        }
        //create point 
        point_map[i] = point(values[0],values[1],values[2]);
        if(config.debug_one == 1){
            std::cout<<LOC<<"Point object: x: "<< point_map[i].x << " y: "<< point_map[i].y << " z: "<< point_map[i].z << std::endl;
        }
    }
    point_file.close();
    std::cout <<LOC<< "Done with the points" <<std::endl;
    /*
    
    Second we create the faces

    */
    std::string face_file_path = config.filePath+"/constant/polyMesh/faces";
    std::ifstream face_file(face_file_path);
    assert_file_valid(face_file, face_file_path);
    number_of_entries=-1;
    for(int i=0;i<19;i++){
        std::getline(face_file,line);//skips the first 19 lines of the file unimportant
    }
    std::getline(face_file,line);
    number_of_entries = std::stoi(line);
    face_amount = number_of_entries;
    
    std::vector<cell> cells = std::vector(number_of_entries, cell());
    std::vector<cell_result> results = std::vector(number_of_entries, cell_result {});
    std::vector<face> faces = std::vector(face_amount, face { });

    std::getline(face_file,line);//gets rid of a unwanted line
    for(int i=0;i<number_of_entries;i++){
        std::getline(face_file,line);//get line
        //remmove brakets  ex: "4(0 0 0 0)"" --> "0 0 0 0 "
        line[0]='(';//replaces the 4 in the dataset with a ' ' to fix a issue
        line.erase(std::remove_if(line.begin(),line.end(),&toBeRemoved),line.end());
        line += ' ';
        if(config.debug_one == 1){
            std::cout<<LOC<<"Line after processing (face) "<< line << std::endl;
        }
        //seperate into bits ex: "0 0 0 0" -> 4 values in a array
        double values[4];
        int a = 0;
        std::string tempString = "";
        int debug;
        int j=0;
        while(j<4){
            while(a<line.size()&&j<4){
                if(config.debug_one==1){
                    std::cout <<LOC<<"current character:"<< line[a]<<std::endl<<"Current value of a: " << a<<std::endl;
                }
                if(line[a]==' '){
                    if(config.debug_one==1){
                        std::cout <<LOC<<"trying to convert: "<< tempString << std::endl;
                        std::cout <<LOC<< std::endl << " current value[" << j <<"] = "<< values[j] <<std::endl;
                    }
                    if(tempString==" "){
                        panic("Invalid content of file");
                    }
                    values[j]=std::stoi(tempString);
                    
                    
                    if(config.debug_one==1){
                        std::cout <<LOC<<"the conversion: "<< std::stoi(tempString) << std::endl << " new value[" << j <<"] = "<< values[j] <<std::endl<<" the temp string was reste to: |"<<tempString<<std::endl;
                    }
                    tempString="";
                    j++;
                    
                }else{
                    tempString += line[a];
                    if(config.debug_one==1){
                        std::cout <<LOC<<"new temp sting: |"<< tempString<< std::endl;
                    }

                }
                a++;
            }
            a=0;
        }
        //create point 
        faces[i] = face(point_map[values[0]],point_map[values[1]],point_map[values[2]],point_map[values[3]]);
        if(config.debug_one == 1){
            std::cout<<LOC<<"face corner IDs: "<< values[0] << " , "<< values[1] << " , "<< values[2] << " , "<< values[3]<< std::endl;
            
        }
    }
    /*
    for(int i=0;i<number_of_entries;i++){
        std::cout<<LOC<<face_map[i].printCorners()<<std::endl;
    }*/
    face_file.close();
    std::cout << LOC<<"Done with the faces" <<std::endl;
    /*
    
    then we read the face ownerships

    */
    std::string face_file_o_path = config.filePath+"/constant/polyMesh/owner";
    std::ifstream face_file_o(face_file_o_path);
    assert_file_valid(face_file_o, face_file_o_path);
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
        if(config.debug_one==1){
            std::cout <<LOC<<"Atempting to convert to int:"<< line <<std::endl;
        }
        value = std::stoi(line);
        
        
        faces[i].owners[0]= value;
        if(config.debug_one==1){
            std::cout<<LOC<<"Set cell id "<< value <<" as owner of face id"<< i <<std::endl;
        }
    }
    face_file_o.close();
    std::cout <<LOC<< "Done with the face ownership" <<std::endl;
    /*
    
    then we read the face nighbours
    
    */

    std::string face_file_n_path = config.filePath+"/constant/polyMesh/neighbour";
    std::ifstream face_file_n(face_file_n_path);
    assert_file_valid(face_file_n, face_file_n_path);
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
        if(config.debug_one==1){
            std::cout <<LOC<<"Atempting to convert to int:"<< line <<std::endl;
        }
        value_i = std::stoi(line);
        line = "-1";
        
        faces[i].owners[1]= value_i;
        if(config.debug_one==1){
            std::cout<<LOC<<"Set cell id "<< value_i <<" as owner of face id"<< i <<std::endl;
        }
    }
    face_file_n.close();
    std::cout <<LOC<< "Done with the face neighbours" <<std::endl;
    /*
    
    now make the cells 
    
    */
    for(int i=0;i<face_amount;i++){
        bool tempbool=false;
        if(config.debug_one){
            std::cout<<LOC<<std::endl<<"#####################################"<<std::endl<<"Showing face ID:"<<i<<std::endl<<"Owner 0: "<<faces[i].owners[0]<<std::endl;
            std::cout<<LOC<<"Owner 1: "<<faces[i].owners[1]<<std::endl;
        }
        if(cells[faces[i].owners[0]].id != -1 && faces[i].owners[0]!= -1){
            cells[faces[i].owners[0]].addNeighbour(faces[i].owners[1]);
            if(config.debug_one){
                std::cout<<LOC<<"Declared additional face for Cell:"<<faces[i].owners[0]<<std::endl;
            }
        }else{
            if(faces[i].owners[0]!= -1){
                cells[faces[i].owners[0]] = cell();
                cells[faces[i].owners[0]].id = faces[i].owners[0];
                cells[faces[i].owners[0]].addNeighbour(faces[i].owners[1]);
                tempbool=true;
                cell_amount++;
                if(config.debug_one){
                    std::cout<<LOC<<"Declared Cell:"<<faces[i].owners[0]<<std::endl;
                }
            }
        }
        if ( cells[faces[i].owners[1]].id != -1 && faces[i].owners[1]!= -1)
        {
           cells[faces[i].owners[1]].addNeighbour(faces[i].owners[0]);
           if(config.debug_one){
                std::cout<<LOC<<"Declared additional face for Cell:"<<faces[i].owners[1]<<std::endl;
            }
        }else{
            if(faces[i].owners[1]!= -1){
                cells[faces[i].owners[1]] = cell();
                cells[faces[i].owners[1]].id = faces[i].owners[1];
                cells[faces[i].owners[1]].addNeighbour(faces[i].owners[0]);
                tempbool=true;
                cell_amount++;
                if(config.debug_one){
                    std::cout<<LOC<<"Declared Cell:"<<faces[i].owners[1]<<std::endl;
                }
            }    
        }
    }

    std::cout <<LOC<<"Done with cell creation"<<std::endl;
    std::cout<<LOC<<"cell amount: "<<cell_amount<<std::endl;

    std::cout <<LOC<<"Done with cell computations"<<std::endl;

    double start;
    double end;
    double increment_time;
    std::string filePath_velocities;
    std::string filePath_centers;
    std::string C_line;
    for(int i=0;i<config.numberCalculateSteps*3;i+=3){
        start = config.calculate_steps[i];
        end = config.calculate_steps[i+1];
        increment_time = config.calculate_steps[i+2];
        std::cout << LOC << "start: " << start << " end: " << end << " increment " << increment_time << std::endl;
        for(double n=start;n<=end;n+=increment_time){
            std::string fileNumber = std::to_string(n);
            while (fileNumber.length() > 1) {
                char c = fileNumber.back();
                if (c != '0' && c != '.') {
                    break;
                }
                fileNumber.pop_back();
            }
            filePath_velocities = config.filePath;
            filePath_velocities += "/";
            filePath_velocities += fileNumber;
            filePath_centers = filePath_velocities;
            filePath_centers += "/C";
            filePath_velocities += "/U";
            std::ifstream velocity_file(filePath_velocities);
            std::ifstream center_file(filePath_centers);
            assert_file_valid(velocity_file, filePath_velocities);
            assert_file_valid(center_file, filePath_centers);
            line="";
            C_line="";
            number_of_entries=-1;
            for(int i=0;i<21;i++){
                std::getline(velocity_file,line);//skips the first 19 lines of the file unimportant
                std::getline(center_file,C_line);
            }
            std::getline(velocity_file,line);
            std::getline(center_file,C_line);
            
            number_of_entries = std::stoi(line);
            std::cout << LOC << "entries: " << number_of_entries << std::endl;
            std::getline(velocity_file,line);//gets rid of a unwanted line
            std::getline(center_file,C_line);
            for(int i=0;i<number_of_entries;i++){
                std::getline(velocity_file,line);//get line
                std::getline(center_file,C_line);
                //remmove brakets  ex: "(0 0 0)"" --> "0 0 0 "
                line.erase(std::remove_if(line.begin(),line.end(),toBeRemoved),line.end());
                line += ' ';
                C_line.erase(std::remove_if(C_line.begin(),C_line.end(),toBeRemoved),C_line.end());
                C_line += ' ';

                //seperate into bits ex: "0 0 0" -> 3 values in a array
                double values[3];
                double c_value[3];
                int a = 0;
                std::string tempString = "";
                std::string C_tempString = "";
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
                j=0;
                while(j<2){
                    while(a<C_line.size()){
                        if(C_line[a]==' '){
                            c_value[j]=std::stod(C_tempString);
                            C_tempString="";
                            j++;
                            
                        }else{
                            C_tempString += C_line[a];
                        }
                        a++;
                    }
                    a=0;
                }
                cells[i].internalVolocity = velocity(values[0],values[1],values[2]);
                cells[i].center = point(c_value[0],c_value[1],c_value[2]);
            }
            velocity_file.close();
            std::cout << LOC << "Done loading velocities" <<std::endl;
            if(config.debug_two){
                printValues(cells,cell_amount);
            }
            uint64_t start_time;
            start_time = getTimeMS();

            int portions_of_then = 1;
            if(config.use_threads){
                if (config.thread_count < 1) panic("Must use at least one thread, but a config value of " << config.thread_count << " was provided");
                int num_threads = config.thread_count;
                num_threads += 1; // one residual thread
                int thread_load = cell_amount / (num_threads-1);
                std::cout<<LOC<<"trying to create Threads number:"<<num_threads<<std::endl;
                std::thread* threads[num_threads];

                for(int i=0;i<num_threads;i++){
                    std::cout<<LOC<<"Thread created"<<std::endl;
                    threads[i] = new std::thread(threadableMath,thread_load*i, std::min(thread_load*(i+1), cell_amount),i,start_time, cells, &results);
                }
                std::cout<<LOC<<"All threads created"<<std::endl;
                for(int i=0;i<num_threads;i++){
                    threads[i]->join();
                }
                std::cout<<LOC<<"All threads done"<<std::endl;
            }
            else{
                for(int i=0;i<cell_amount;i++){
                    cells[i].math(cells, &results[i]);
                    if((float)i/cell_amount >= 0.001*portions_of_then){
                        float t = (getTimeMS()-start_time) / 1000.0;
                        float eta = (t / portions_of_then) * (1000 - portions_of_then);
                        std::cout<<LOC<<"completed "<<portions_of_then / 10.0 << "% in " << t <<"s (ETA in " << eta << "s)"<<std::endl;
                        portions_of_then += 1;
                    }
                }
            }
            std::cout<<LOC<<"The math took: "<<(getTimeMS()-start_time) / 1000.0 <<"s"<<std::endl;
            printToFile(cells, results, fileNumber, config.filePath, cell_amount);
            printToFileQ(cells, results, fileNumber, config.filePath, cell_amount);
        }
    }
    return 0;
}



