How to use the config file:
the config file works on a key -> value system, meaning in the first line the name/key is written and in the following lines the value for the key.

######################################################################

example:
Name of Dataset:
Simulation_Umstroemung_eines_Wuerfels

//In this example the key would be "Name of Dataset:" and the new value would be "Simulation_Umstroemung_eines_Wuerfels"
######################################################################

Each key has a assosiated datatype(listed bellow) and function in the code(listed bellow)
bool keys: (value is either a 0 or 1 or true or false)
Debug mode one: -> can be activated for additional information (recomended false)
Debug mode one stepping one:-> can be activated for additional information (recomended false)
Debug mode one stepping two:-> can be activated for additional information (recomended false)
Debug mode one stepping three:-> can be activated for additional information (recomended false)
Debug mode two:-> can be activated for additional information (recomended false)
Debug mode three:-> can be activated for additional information (recomended false)
Use threads:-> allowes the user to enable the use of threads to speed up the calculation

int keys:
Thread count:-> declares the number of threads the programm will try to create if threads are used

array double key:
Calculate these time steps base: -> takes 3 values first the start time step, followed in the next line by the end time and lastly in a third line the increment between the time steps

