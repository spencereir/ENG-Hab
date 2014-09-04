/*                       
											on error try {
												goto spencer;
											} catch(SpencerIsBusyOrHasGraduatedException e) {
												int telephoneNum = 6138837874;
												Phone p = new Phone(telephoneNum);
												try {
													p.call();
												} catch(NoAnswerException e) {
													std::string email = "spencer.whitehead@rogers.com";
													std::string subject = "[SPACESIM] Engineering Software";
													std::string content = "Hi Spencer, I recieved error number " + to_string(errno) + " while running ENG Hab. Any idea how to fix this? Thanks."
													MailHandler m = new MailHandler(email, subject, content);
													try {
													  m.send();
													} catch(MailWontSendOrIHaventResponedIn30DaysException e) {
														std::string url = "http://facebook.com/spencer.whitehead1337/"
														FacebookAPI f = new FacebookAPI(url);
														try {
														  f.message(content);
														} catch(FacebookHasFinallyClosedException e) {
															//Thank god
															MSNObj m = new MSNObj;
															try {
																m.message(email, content);
															} catch(NobodyUsesMSNException e) {
																COORD loc = NSA::GetCoordsByTelephoneNumber(telephoneNum);
																SmokeAPI s = new SmokeAPI;
																try {
																	s.sendSignal(content);
																} catch(SpencerHasNoReasonToKnowSmokeSignalsException e) {
																  SQLCommand c;																													//If you guys are using this in 2048, get some new software. seriously
																	c.setCommandText("SELECT * FROM PEOPLE WHERE name = 'Spencer Whitehead' AND age > 14 AND age < 50 AND highSchool = 'Lisgar Collegiate Institute");
																	c.FetchFirst();
																	c.setCommandText("UPDATE people WHERE p_uuid = " + c.field("p_uuid") + " SET x_location = " + this.x + ", y_location = " + this.y");
																	try {
																	  c.Execute();
																	} catch(SQLCantBeUsedToManipulateSpaceTimeException e) {
																		try {
																			Install("get.adobe.com/reader/"); //Adobe reader will fix all your problems.
																		} catch(AdobeReaderDidNotFixAllYourProblemsException e) {				//Should never execute, Adobe reader never fails
																			SpaceTimeContinuum s = this.getSpaceTimeContinuum();
																			s.mass++;
																			if(physics.exists()) {			//Should never execute, we have already broken physics on the last line
																				foreach(atom : atoms) {
																					atom.formula = "DEsU";		//Good ol' Dilithium Monoeinsteinium monouranide
																				}												
																				if(physics.exists()) {			//Should never execute, we have already made every single atom in the universe DEsU
																					exit(1);								//Probably a server issue
																				}
																			}
																		}
																	}
																}
															}
														}
													}
												}
											}
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

					If you are looking at this
					code, then CONGRATULATIONS!
					My extreme laziness and 
					tendancy to shortcut have
					given you a project that
					will waste away valuable
					hours of your life. At
					least you have readable
					source to go off of.
					Also, if you break this
					so horribly that it has
					no hope of working again,
					have no fear! Just go to
				https://github.com/spencereir/ENG-Hab
				    where you can download a
					stable build. If you
					have an issue you just
					can't fix, make an issue
					report on github, and I
					will (probably) get to 
					it within the year.
*/

/*
TODO
(No = I have decided not to do this)
(NF = Not finished, probably do this)
(Finished = I have finished this idea)
      NF - I have marked inefficient code segments with disabled breakpoints, fix these.
      No - The Module and Bus classes are circularly dependant. By seperating them into their own files, multiple function calls will be saved (Amounting to something like 200 cycles/second)
      No - ^ ALTERNATE: MAKE A VECTOR MEMBER OF BUS THAT HOLDS VECTORS! BAM PROBLEM SOLVED GO REWRITE 500 LINES OF CODE MAKE A BRANCH FOR THIS ON GITHUB?
Finished - Use class prototypes and declare the circularlry dependant functions after both classes are written
CHANGELOG
8/28/14 - Did everything that isn't documented because I started a changelog way too late
8/28/14 - Added a Changelog
8/28/14 - Added offline mode (Enter offline by setting the OFFLINE flag to 1)				
8/29/14 - Removed 3 functions and instead used class prototypes to save 9 lines of code and much efficiency (As per earlier TODO)
9/4/14  - Made various changes for readibility on git
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

#pragma comment(lib, "sqlapi.lib")		//Lets get SQLty

#define NUM_MODULES 18
#define NUM_BUSES 3
#define PRIMARY_BUS_WATTS 2500000
#define SECONDARY_BUS_WATTS 10000
#define PRIMARY_HEAT_RATIO 100000				//Bigger this is = Smaller increments in which heat goes up
#define PRIMARY_COOLING_BASELINE 1			//Bigger this is = Larger temperature reductions every tick (1 tick = dTime milliseconds, assigned in the main function)
#define SECONDARY_HEAT_RATIO 50000
#define SECONDARY_COOLING_BASELINE 0.5
#define TERNARY_HEAT_RATIO 10000
#define TERNARY_COOLING_BASELINE 0.1
#define GENERAL_COOLING_BASELINE 1		//The amount that non-powersource modules cool off every tick
#define EFFICIENCY 0.99								//No system is 100% efficient
#define OFFLINE 0

SYSTEMTIME t;
int radPC;
int rconLvl;
int oldRconLvl;

using namespace std;

bool update(char keyIn);			//NAH. NEXT PERSON TO LOOK AT THIS CODE PLEASE DO THIS
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
	    	float thresh1;				//Crossing this temperature means that the module goes into a "warning" state
	    	float thresh2;				//Crossing this temperature means that the module goes into the "destroyed" state
		float watts;				//The assumption here is that the reactor puts out a total of "enough" volts, and we designed it with the idea that it can pull whatever current we want. Therefore, watts are really the only important thing.
	    	statuses status;			// ^ Trust me, I know someone who knows electronics, and he sort of agreed with me
		bool oldWire;
		bool oldPowered;
		float oldTemp;
		statuses oldStatus;				//These are the things sent over SQL
	//Float functions
	float heat();			//Just a prototype, the function is declared later
    
    	//Void functions 
	void power();
    	
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
      		watts = 0;
		thresh1 = 95.0F;
		thresh2 = 120.0F;
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
		int source;
		float watts;
       		void power() {
      			switch(busNum) {
        			case 0:
          				if((modules[findByName("HabitatReactor")].powered && modules[findByName("HabitatReactor")].wire) || ((modules[findByName("FuelCell")].powered && modules[findByName("FuelCell")].wire) || (modules[findByName("Battery")].powered && modules[findByName("Battery")].wire) && true/*change this to check wires connecting bus 1/2*/)) {
            					powered = true;       
						watts = PRIMARY_BUS_WATTS;
						if(modules[findByName("HabitatReactor")].powered) source = 0;
						else if(modules[findByName("FuelCell")].powered) source = 1;	//We dont have to perform checks on the connecting						
						else if(modules[findByName("Battery")].powered) source = 2;	//wire here because of reasons just accept my logic
							
          				}
          				else powered = false;
          				break;
        			case 1:
				        if(((modules[findByName("HabitatReactor")].powered && modules[findByName("HabitatReactor")].wire) && true/*see above*/) || (modules[findByName("FuelCell")].powered && modules[findByName("FuelCell")].wire) || (modules[findByName("Battery")].powered && modules[findByName("Battery")].wire)) {
				          	powered = true;
				          	watts = SECONDARY_BUS_WATTS;         
				        }       					//Add source to this...?    
				        else powered = false;
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
      			cout << stats << endl;
			if(busNum == 0) {
				cout << "RADIATION SHIELD PERCENTAGE: " << radPC << " REACTOR CONTAINMENT LEVELS: " << rconLvl << endl;
			}
      			cout << "POWERED: " << powered << " WATTS: " << watts << "W" << " POWER SOURCE: " << modules[source].name << " TIME: " << t.wMilliseconds + (t.wSecond * 1000) << endl << endl;
			cout << "POWER SOURCE INFO:" << endl;
			modules[source].display();
			cout << endl;
      			for(int i = 3; i < modules.size(); i++) {
        			if(modules[i].busNum == busNum) modules[i].display();                     
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

float Module::heat() {							//Declare some more functions
	if(powered) {
    		//Heat calculations here  
		if(truncName == "HabitatReactor" || truncName == "Battery" || truncName == "FuelCell") {
			if(truncName == "HabitatReactor") {
				temp += (PRIMARY_BUS_WATTS - bus[0].watts) / PRIMARY_HEAT_RATIO - PRIMARY_COOLING_BASELINE;
				if(temp < 81) temp = 81;
			}
			else if(truncName == "FuelCell") {
				if(/*thingthatfindsifbus1&2areconnected*/true && !modules[findByName("HabitatReactor")].powered) {  //Check to see if bus 1 and 2 are connected, and see if fuelcell is powering them (Habitat Reactor will auto shutoff fuel cell
					temp += (PRIMARY_BUS_WATTS - bus[0].watts) / SECONDARY_HEAT_RATIO - SECONDARY_COOLING_BASELINE;
				}
				temp += (SECONDARY_BUS_WATTS - bus[1].watts) / SECONDARY_HEAT_RATIO - SECONDARY_COOLING_BASELINE;
			}
			else if(truncName == "Battery") {
				if(/*seeabove*/true && !modules[findByName("HabitatReactor")].powered && !modules[findByName("HabitatReactor")].powered) {
					temp += (PRIMARY_BUS_WATTS - bus[0].watts) / TERNARY_HEAT_RATIO - TERNARY_COOLING_BASELINE;
				}
				temp += (SECONDARY_BUS_WATTS - bus[0].watts) / TERNARY_HEAT_RATIO - TERNARY_COOLING_BASELINE;
			}
		}
	}
	else temp -= GENERAL_COOLING_BASELINE;
		if(temp > thresh1) {
			if(temp > thresh2) {
		  status = destroyed;  
		}        
	  status = warning;
	}         
	else if(temp < 0) temp = 0;
  	return temp;
}

void Module::power() {
	if(wire) {
		if(bus[busNum].powered) powered = true;                      
		else powered = false;       
  	}
	else powered = false;
}

int main() {
	if(!OFFLINE) {
		ifstream inFile ("serverLocation.txt");
		string serverComputer;
		getline(inFile, serverComputer);
		inFile.close();
		serverComputer += "@spacesimServer";
		cout << serverComputer << endl;
		system("pause");
		char *databaseName = (char*) serverComputer.c_str();
		try {
			con.Connect("spacesimServer", "root", "isotope", SA_MySQL_Client);
		} catch(SAException &x) {
			cout << "Error: " << x.ErrText() << endl;
			system("pause");
			exit(1);
		}
	}
	//Initialize all nesseccary variables
	char in = ' ';                                               //Holds user input
	dTime = 500;                                                   //How many milliseconds between updates?
	int radPC = 0;
	int rconLvl = 0;
	  
	//Initialize the Key Input map
	keyMap['z'] = 0;																			//Z = Habitat Reactor
	keyMap['n'] = 1;																			//N = Fuel Cell
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
	modules[5].watts = 1000;
	modules[6].name = "Reactor Containment 1";
	modules[6].truncName = "ReactorContainment1";
	modules[6].watts = 50000;
	modules[7].name = "Reactor Containment 2";
	modules[7].truncName = "ReactorContainment2";
	modules[7].watts = 50000;
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
		modules[findByName("HabitatReactor")].temp = 81.0F;
		modules[findByName("HabitatReactor")].wire = true;
		rconLvl = 2000;
	}
	else if(startType == "c") {
	    //cold start stuff
		  //so nothing, right?
			//^ yeah pretty much.
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
			system("cls");
	      		bus[0].display();
		}
	} while(in != 'q');
	system("PAUSE");
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
		if(modules[findByName("ReactorContainment1")].powered == false && modules[findByName("ReactorContainment2")].powered == false) {
	      		rconLvl -= 1; 
	    	}
		else if(!modules[findByName("ReactorContainment1")].powered != !modules[findByName("ReactorContainment1")].powered) {		//Protip: (!A != !B) is a really easy way to do A XOR B. The ! before A and B converts it to a bool if it wasnt already, and doesn't affect the answer at all
			rconLvl += 1;
		}
		else {
			rconLvl += 2;
		}
		if(rconLvl > 2000) rconLvl = 2000;
		else if(rconLvl < 0) rconLvl = 0;
		if(!OFFLINE) {
			cmd.setConnection(&con);
			string com = "UPDATE engVars SET val = " + to_string(rconLvl) + " WHERE name = 'rconLvl'";
			cmd.setCommandText(com.c_str());
			cmd.Execute();
			con.Commit();
		}
	    	change = true;
	}
  	if(keyIn == 'x') {                                                  //Put here key inputs not specific to a module
		if(modules[findByName("HabitatReactor")].temp >= 81) {
			modules[findByName("HabitatReactor")].powered = true;
			bus[0].power();
			modules[findByName("HabitatReactor")].power();
			change = true;
		}
	}
	else if(keyIn == /*Right key*/'.') {
		radPC += 1;
		if(radPC > 100) radPC = 100;
		if(!OFFLINE) {
			cmd.setConnection(&con);
			string com = "UPDATE engVars SET val = " + to_string(radPC) + " WHERE name = 'radPC'";
			cmd.setCommandText(com.c_str());
			cmd.Execute();
			con.Commit();
		}
		change = true;
	}
	else if(keyIn == /*Left key*/',') {
		radPC -= 1;
		if(radPC < 0) radPC = 0;
		if(!OFFLINE) {
			cmd.setConnection(&con);
			string com = "UPDATE engVars SET val = " + to_string(radPC) + " WHERE name = 'radPC'";
			cmd.setCommandText(com.c_str());
			cmd.Execute();
			con.Commit();
		}
		change = true;
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
	//for(int i = 0; i < NUM_BUSES; i++) bus[i].power();		//HORRIBLY INEFFICIENT, FIX THE ABOVE CODE ASAP
	for(int i = 3; i < modules.size(); i++) {                             //Calculate new power values for the buses (Should only need to do this if change is true, investigate later)
		if(modules[i].powered) {
			bus[modules[i].busNum].watts -= modules[i].watts * (1/EFFICIENCY);					
			if(bus[modules[i].busNum].watts < 0) {																//If the bus cannot provide enough wattage for a module
				modules[i].powered = false;																					//Power off that module
				bus[modules[i].busNum].watts += modules[i].watts * (1/EFFICIENCY);  //Return the spent watts
			}
		}
		if(!OFFLINE && (modules[i].temp != modules[i].oldTemp || modules[i].wire != modules[i].oldWire || modules[i].powered != modules[i].oldPowered || modules[i].status != modules[i].oldStatus)) {
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
  	for(int i = 0; i < NUM_MODULES; i++) if(modules[i].truncName == fName) return i;         
  	cout << "Cannot find module with name " << fName << endl;
  	system("pause");
}
int maxWidth() {
  	int max = 0;
  	for(int i = 0; i < modules.size(); i++) {
    		if(modules[i].name.size() > max) max = modules[i].name.size();			//Possible speed liability (Shouldn't need to check every time... this is literally being called more than 32 times a second, so about 640 wasted cycles atm)
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
	cmd.setConnection(&con);
	cmd.setCommandText(cmdText.c_str());
	cmd.Execute();
	con.Commit();
}
