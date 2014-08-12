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
 
                       By Spencer Whitehead

*/
#include "globals.cpp"
#include "listener.h"
#include "sender.h"
#include <iomanip>
#include <map>
#include <conio.h>
#include <thread>

#define NUM_MODULES 18
#define NUM_BUSES 3

SYSTEMTIME t;

using namespace std;

bool busPowered(int busNum);
bool update(char keyIn);
int findByName(string fName);
int maxWidth();
char getInput();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        void skyrimStart();
void line();
void lineParser(string pLine);
void listenerFunction();
void modify(string name, string mod, string val);
vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);

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
bool finished = false;
//memo: call power() on all modules when any wire is changed
  //for bonus efficiency, only call it on changed wire unless it is a power source, then check all affected buses
int main() {
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
    std::cout << "Error: Could not start WSA" << std::endl;
    system("pause");
  }

  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(SERVER_COMPUTER, PORT, &hints, &servinfo) != 0) {
    std::cout << "Error: Could not connect to target " << SERVER_COMPUTER << " on port " << PORT << "." << std::endl;
  }
  assert(servinfo != NULL);
  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      cout << "Error: Could not intialize socket" << endl;
      continue;
    }
    break;
  }
  
  char hostName[128];
  gethostname(hostName, sizeof hostName);
  thread listenerThread (listenerFunction);
  //Initialize all local variables
  char in = ' ';                                               //Holds user input
  dTime = 100;                                                   //How many milliseconds between updates?
  
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
    //hot start stuff             
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
      string sString = (static_cast<std::string>(hostName) + ";1|" + ENG_MC_COMPUTER + ":" + to_string(modules.size() * 3) + ";");
      for(int i = 0; i < modules.size(); i++) {
        sString += modules[i].truncName + "-temp~" + to_string(modules[i].temp) + ";";
        sString += modules[i].truncName + "-wire~" + to_string(modules[i].wire) + ";";
	sString += modules[i].truncName + "-status~" + to_string(modules[i].status) + ";";
      }
      //TODO: Optimise this statement a LOT. Find a way to record what values are changing, then make one big string, and send all the data at the same time. Multiple small strings will kill speed, one big one can be sent easy.
      //TODO: Investigate whether or not doing port bindings at the beginning of the code and just passing ports to the sender function can speed up the process
      sender(sString.c_str(), sockfd, p);
      system("cls");
      bus[0].display();
    }
  } while(in != 'q');
  finished = true;
  WSACleanup();
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
    if(modules[i].name.size() > max) max = modules[i].name.size();
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

void line() { 
  cout << setfill('-') << setw(80) << "-";
  cout << setfill(' '); 
}
void lineParser(string pLine) {
  vector<string> data = split(pLine, ';');
  for(int i = 1; i < atoi(data[0].c_str()) + 1; i++) {
    vector<string> subData = split(data[i], '-');
	vector<string> subSubData = split(subData[1], '~');
	modify(subData[0], subSubData[0], subSubData[1]);
  }
}
void listenerFunction() {
  string oldLis = "";
  do {
    string lis = listener();
	if(oldLis != lis) {
      oldLis = lis;
	  lineParser(lis);
	}
  } while(!finished);
}

void modify(string name, string mod, string val) {
  int id = findByName(name);
  if(mod == "temp") {
    modules[id].temp = atof(val.c_str());
  }
  else if(mod == "status") {
    modules[id].status = static_cast<statuses>(atoi(val.c_str()));
  }
  else if(mod == "wire") {
    modules[id].wire = atoi(val.c_str());
  }
}


vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
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
 
