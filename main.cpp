/*                       on error goto spencer;
          #####                                                              
         #     # #####    ##    ####  ######  ####  # #    #                 
         #       #    #  #  #  #    # #      #      # ##  ##                 
          #####  #    # #    # #      #####   ####  # # ## #                 
               # #####  ###### #      #           # # #    #                 
         #     # #      #    # #    # #      #    # # #    #                 
          #####  #      #    #  ####  ######  ####  # #    #                 
 #######                                                             
 #       #    #  ####  # #    # ###### ###### #####  # #    #  ####  
 #       ##   # #    # # ##   # #      #      #    # # ##   # #    # 
 #####   # #  # #      # # #  # #####  #####  #    # # # #  # #      
 #       #  # # #  ### # #  # # #      #      #####  # #  # # #  ### 
 #       #   ## #    # # #   ## #      #      #   #  # #   ## #    # 
 ####### #    #  ####  # #    # ###### ###### #    # # #    #  ####
							Hab Edition
                       By Spencer Whitehead

*/

#include <iostream>				//Standard I/O
#include <fstream>				//File I/O
#include <string>				//C++ std::string manipulations
#include <cstring>				//C char*/char[] manipulations
#include <iomanip>				//Formatting outstream
#include <map>					//Maps to handle keyboard input
#include <conio.h>				//Keyboard I/O (kbhit)
#include <Windows.h>			//Systemtime functions
#include <vector>				//Vectors > Arrays
#include <SQLAPI.h>				//Database I/O

#pragma comment(lib, "sqlapi.lib");

#define NUM_MODULES 18
#define NUM_BUSES 3

SYSTEMTIME t;

using namespace std;

bool busPowered(int busNum);
bool update(char keyIn);
int findByName(string fName);
int maxWidth();
char getInput();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  void skyrimStart();
inline void line();

enum statuses {
  normal,
  warning,
  destroyed
};

class Module {
  public:
    string name;
	string truncName;	//Truncated name, which will be used for receiving data.
    bool wire;
    bool powered;
    int id;
    int busNum;
    float temp;
    float thresh1;
    float thresh2;
    float amps;
    float volts;
    statuses status;
	bool oldWire;
	bool oldPowered;
	float oldTemp;
	statuses oldStatus;
    
    //Float functions
    float heat() {
      if(powered) {
        //Heat calculations here  
        if(temp > thresh1) {
          if(temp > thresh2) {
            status = destroyed;  
          }        
          status = warning;
        }         
	  }
	  return temp;
    }
    
    //Void functions 
    void power() {
      if(wire) {
        if(busPowered(busNum)) {
          powered = true;                       
        }                  
        else powered = false;       
      }
      else powered = false;
    }
    void toggleWire() {
      wire = !wire;   
    }     
    void setInitialValues(int i) {
      name = "Default";
	  truncName = "Default";
      wire = false;
      powered = false;
      id = i;
      busNum = 0;
      temp = 0.0F;
      amps = 0.0F;
      volts = 0.0F;
      status = normal;
    }
    void display() {
      cout << setfill('-') << setw(maxWidth()) << left << setprecision(3) << name << "--Wire: " << setfill(' ') << wire << " Powered: " << powered << " Temp: " << temp << " ID: " << id << " Status: " << status << endl;
    }
};

vector<Module> modules(NUM_MODULES);
void updateDatabase(Module mod);

class Bus {
  public:
    bool powered;   
    int busNum;
    float volts;
    float amps;
    void power() {
      switch(busNum) {
        case 0:
          if((modules[findByName("HabitatReactor")].powered && modules[findByName("HabitatReactor")].wire) || ((modules[findByName("FuelCell")].powered && modules[findByName("FuelCell")].wire) || (modules[findByName("Battery")].powered && modules[findByName("Battery")].wire) && true/*change this to check wires connecting bus 1/2*/)) {
            powered = true;       
            volts = 10000;
            amps = 2500;                   
          }
          else {
            powered = false;
            volts = 0;
            amps = 0;
          }
          break;
        case 1:
          if(((modules[findByName("HabitatReactor")].powered && modules[findByName("HabitatReactor")].wire) && true/*see above*/) || (modules[findByName("FuelCell")].powered && modules[findByName("FuelCell")].wire) || (modules[findByName("Battery")].powered && modules[findByName("Battery")].wire)) {
            powered = true;
            volts = 120;
            amps = 25;                          
          }            
          else {
            powered = false;
            volts = 0;
            amps = 0;
          }
          break;   
      }
    }
    void display() {
      line();
      string stats = "BUS ";
      stats += to_string(busNum + 1);
      stats += " STATISTICS";
      int width = (80 - stats.length()) / 2;
      for(int i = 0; i < width; i++) cout << " ";
      cout << stats << endl << endl;
      cout << "POWERED: " << powered << " VOLTS: " << volts << "V AMPS: " << amps << "A WATTS: " << volts * amps << "W" << " TIME: " << t.wMilliseconds + (t.wSecond * 1000) <<endl << endl;
      for(int i = 3; i < modules.size(); i++) {
        if(modules[i].busNum == busNum) {
          modules[i].display();                     
        }        
        else break;
      }
      line();
    }
};

vector<Bus> bus(3);

map<char, int> keyMap;
double time1;
double time2;
double dTime;
string startType = "";
SAConnection con;
SACommand cmd;

int main() {
  ifstream inFile ("serverLocation.txt");
  string serverComputer;
  getline(inFile, serverComputer);
  inFile.close();
  serverComputer + "@spacesimServer";
  char *databaseName = (char*) serverComputer.c_str();
  try {
	con.Connect("HAL-9000@spacesimServer", "root", "isotope", SA_MySQL_Client);
  } catch(SAException &x) {
	cout << "Error: " << x.ErrText() << endl;
	system("pause");
	exit(1);
  }
  //Initialize all local variables
  char in = ' ';                                               //Holds user input
  dTime = 500;                                                   //How many milliseconds between updates?
  
  //Initialize the Key Input map
  keyMap['z'] = 0;
  keyMap['n'] = 1;
  keyMap['p'] = 2;
  keyMap['a'] = 3;
  keyMap['b'] = 4;
  keyMap['c'] = 5;
  keyMap['d'] = 6;
  keyMap['e'] = 7;
  keyMap['f'] = 8;
  keyMap['g'] = 9;
  keyMap['h'] = 10;
  keyMap['i'] = 11;
  keyMap['j'] = 12;
  keyMap['k'] = 13;
  keyMap['l'] = 14;
  keyMap['m'] = 15;
  keyMap['r'] = 16;
  keyMap['o'] = 17;
  
  //Set all values to zero 
  for(int i = 0; i < modules.size(); i++) {
    modules[i].setInitialValues(i);
  }
  //Set up all the module information
  modules[0].name = "Habitat Reactor";
  modules[0].truncName = "HabitatReactor";
  modules[1].name = "Fuel Cell";
  modules[1].truncName = "FuelCell";
  modules[2].name = "Battery";
  modules[2].truncName = "Battery";
  modules[3].name = "Radiation Shield 1";
  modules[3].truncName = "RadiationShield1";
  modules[4].name = "Radiation Shield 2";
  modules[4].truncName = "RadiationShield2";
  modules[5].name = "Artificial Gravity";
  modules[5].truncName = "ArtificialGravity";
  modules[6].name = "Reactor Containment 1";
  modules[6].truncName = "ReactorContainment1";
  modules[7].name = "Reactor Containment 2";
  modules[7].truncName = "ReactorContainment2";
  modules[8].name = "RCS Engines";
  modules[8].truncName = "RCSEngines";
  modules[9].name = "That other one";
  modules[9].truncName = "ThatOtherOne";
  modules[10].name = "Engine Accelerator 1";
  modules[10].truncName = "EngineAccelerator1";
  modules[11].name = "Ion Engine 1";
  modules[11].truncName = "IonEngine1";
  modules[12].name = "Engine Accelerator 2";
  modules[12].truncName = "EngineAccelerator2";
  modules[13].name = "Ion Engine 2";
  modules[13].truncName = "IonEngine2";
  modules[14].name = "Engine Accelerator 3";
  modules[14].truncName = "EngineAccelerator3";
  modules[15].name = "Ion Engine 3";
  modules[15].truncName = "IonEngine3";
  modules[16].name = "Engine Accelerator 4";
  modules[16].truncName = "EngineAccelerator4";
  modules[17].name = "Ion Engine 4";
  modules[17].truncName = "IonEngine4";
  
  if(startType == "") {
    cout << "Enter h to hot start or c to cold start: ";
    cin >> startType;
  }
  if(startType == "h") {																																																																																																																																																																																																																																																																																																																																																												
    modules[findByName("HabitatReactor")].powered = true;             
  }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     else if(startType == "skyrim") { cout << "What? There is no such thing as a skyrim start, I assure you. Enter any character for the program to exit"; cin >> startType; if(startType == "skyrim") { cout << endl << endl << "Fine, you win"; skyrimStart(); } else exit(2); }
  else if(startType == "c") {
       
  }
  else {
    cout << "Invalid start type." << endl;
    system("PAUSE");
    exit(1);     
  }
  GetLocalTime(&t);
  time1 = t.wMilliseconds + (t.wSecond * 1000);
  do {
    in = getInput();
    bool change = update(in);
    if(change) {
	  //write SQL stuff here
	  system("cls");
      bus[0].display();
    }
  } while(in != 'q');
  system("PAUSE");
}

bool busPowered(int busNum) {
  if(bus[busNum].powered) {
    return true;                        
  }     
  else return false;
}
bool update(char keyIn) {         
  bool change = false;      
  GetLocalTime(&t);
  time2 = t.wMilliseconds + (t.wSecond * 1000);  
  if(time2 - time1 >= dTime || time2 - time1 < 0) {
    time1 = time2;
    for(int i = 0; i < modules.size(); i++) {
      modules[i].heat();        
    }         
    change = true;
  }
  if(keyIn == 'x') {                                                  //Put here key inputs not specific to a module
    if(modules[findByName("HabitatReactor")].temp >= 80) {
      modules[findByName("HabitatReactor")].powered = true;
      change = true;
    }
  }
  else if(keyMap.find(keyIn) != keyMap.end()) {                     //If the input is part of the keymap
    int keyVal = keyMap[keyIn];                                       //Assign an integer value based on the input
    if(keyVal > 2) {                                                  //If the key value is > 2 (Non-power source)
      bus[modules[keyVal].busNum].power();                              //Check the power of the affected bus
      modules[keyVal].toggleWire();                                     //Toggle the wire
      modules[keyVal].power();                                          //Check the power of the module
    }
    else if(keyVal <= 2) {                                            //Otherwise, (It is a power source)
      int lastVal = 0;                                                //Initialize a variable called lastVal
      modules[keyVal].toggleWire();                                   //Toggle the power source wire (Don't check power yet)
      for(int i = 0; i < NUM_BUSES; i++) {                            //For every bus
        bool oldPower = bus[i].powered;                                 //Store their power state in a variable  
        bus[i].power();                                                 //Check to see if they are powered
        if(oldPower != bus[i].powered) {                                //If their new powered value and the varaible are diferent
          for(int j = lastVal; j < modules.size(); j++) {                 //We need to update power states for everything on that bus
            if(modules[j].busNum == i) {                                  //This uses lastVal, so we don't make unnessecary comparisons
              modules[keyVal].power();
            }
            else {
              lastVal = j;                                                //Setting lastVal to j, so we know that the next bus has modules starting at this number
              break;     
            }     
          }            
        }
      }  
    }
    change = true;                                                      //Record that a change has been made
  }
  for(int i = 3; i < modules.size(); i++) {                             //Calculate new power values for the buses
    if(modules[i].powered) {
      bus[modules[i].busNum].volts = 0;					//Add math
	  bus[modules[i].busNum].amps = 0;
    }               
	if(modules[i].temp != modules[i].oldTemp || modules[i].wire != modules[i].oldWire || modules[i].powered != modules[i].oldPowered || modules[i].status != modules[i].oldStatus) {
	  modules[i].oldTemp = modules[i].temp;
	  modules[i].oldWire = modules[i].wire;
	  modules[i].oldPowered = modules[i].powered;
	  modules[i].oldStatus = modules[i].status;
	  updateDatabase(modules[i]);
	}
  }
  return change;
}

int findByName(string fName) {
  for(int i = 0; i < NUM_MODULES; i++) {
    if(modules[i].truncName == fName) {
      return i;                   
    }      
  }
  cout << "Cannot find module with name " << fName << endl;
  system("pause");
}
int maxWidth() {
  int max = 0;
  for(int i = 0; i < modules.size(); i++) {
    if(modules[i].name.size() > max) max = modules[i].name.size();			//Possible speed liability (Shouldn't need to check every time... this is literally being called more than 32 times a second)
  }    
  return max;
}

char getInput() {
  char in = ' ';
  if(_kbhit()) {
    in = _getch();            
  }
  return in;
}

inline void line() {          //I made some witty comments on this one in habMC 
  cout << setfill('-') << setw(80) << "-" << setfill(' ');		//"HabMC"
}						//Probably a sign that i've done enough today

void updateDatabase(Module mod) {
  string cmdText = "UPDATE eng SET temp = " + to_string(mod.temp) + ", wire = " + to_string(mod.wire) + ", powered = " + to_string(mod.powered) + ", status = " + to_string(mod.status) + " WHERE name = '" + mod.truncName + "'";
  cout << cmdText << endl;
  cmd.setConnection(&con);
  cmd.setCommandText(cmdText.c_str());
  cmd.Execute();
  con.Commit();
}

































































































































																																																																																																																																																																																		//This function doesnt exist. Pretend you saw nothing
																																																																																																																																																																																		void skyrimStart() {
																																																																																																																																																																																		  system("dir");
																																																																																																																																																																																		  system("cd \"data/moreData/evenMoreData/will/the/real/eng/data/please/stand/up/The Elder Scrolls V Skyrim/\"");
																																																																																																																																																																																		  system("FTYPE SkyrimSave=TESV.exe");
																																																																																																																																																																																		  system("assoc .ess=SkyrimSave");  
																																																																																																																																																																																		  system("cd..");
																																																																																																																																																																																		  system("cd please/stand/up/please/stand/up/will/the/real/eng/data/please/");
																																																																																																																																																																																		  system("dir");
																																																																																																																																																																																		  system("engineer.ess");
																																																																																																																																																																																		  Sleep(100000);
																																																																																																																																																																																		  system("cls");
																																																																																																																																																																																		  cout << "Congratulations. We've been sitting here waiting for you to get all systems going, and you decide to play Skyrim. Nice one. Now, kindly get back to work." << endl;
																																																																																																																																																																																		  system("pause");
																																																																																																																																																																																		  startType = "h";
																																																																																																																																																																																		  main();
																																																																																																																																																																																		}
 
