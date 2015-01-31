/*                       
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
					  
If you change anything here or find anything wrong with your changes, please record everything in the Changelog and
the TODO. It just really makes everything easier for everyone. Additionally, you can message me (See above) to get
contributor privilages on git, but please please pleease dont push to master until you are 100% sure in all circumstances
that your fix works. I can create a branch for you while we delegate to QA (Is that a taskforce? That should be a taskforce)

TODO
Finished - I have marked inefficient code segments with disabled breakpoints, fix these.
Finished - Use class prototypes and declare the circularlry dependant functions after both classes are written
Finished - Add a source value for the second bus
Finished - Fix a bug that causes radiation shields to not drain power properly
Finished - Fix a bug that causes bus to still read as having power on it after the power source is destroyed
      NF - Totally in the wrong place, but set up some sort of active server program. It will serve as a placebo, as well as just checking what computers it can see every 10 seconds or so. To do this, give every computer a server (Put something in here to set it up) with a set of return codes. Have the computers update their local tables when they hit an error (Ex: set a local variable to 2 on fail to connect)
      NF - Fix a bug that causes the user to still be able to use bus 1 as if it is powered so long as they do not release a key (This is probably true for bus 2 also, cannot test yet)
      NF - Fix a bug that causes the bus watt values to fluctuate while modifying radiation shields (I have NO idea what causes this, kudos if you solve it)
      NF - Find out why sources other than Hab Reactor cant power primary bus
      NF - Maybe find a better way to set up bitmaps than passing x and y to all of them? Excepting this, make good X and Y values. The overeall layout of ENG should stay the same so there is as little confusion as possible
     WIP - Create images and make it draw things that aren't just text
Finished - Make a debouncer for the keys
      NF - Make coolants do stuff
Finished - Clean up class functions using seperate source files
     WIP - Use a unique ID to track when the database was last changed

CHANGELOG
8/28/14  - Did everything that isn't documented because I started a changelog way too late
8/28/14  - Added a Changelog
8/28/14  - Added offline mode (Enter offline by setting the OFFLINE flag to 1)				
8/29/14  - Removed 3 functions and instead used class prototypes to save 9 lines of code and much efficiency (As per earlier TODO)
9/4/14   - Made various changes for readibility on git
9/8/14   - Saved about 1000 cycles per tick at the cost of readability (Which is unnessecary, we only need to feed raw data to Allegro)
9/8/14   - Added support for destroyed module. When reported as destroyed, a module will be unable to be heated, cooled, or powered.
9/8/14   - Fixed multiple bugs surrounding destroyed power source module (See TODO)
9/9/14   - Fixed radiation shield power bug
9/9/14   - Added a source value to the secondary bus
9/9/14   - Against my better judgement, I have decided we will be using Allegro. SDL is too complicated, and everyone in Lisgar ICS alreeady knows Allegro, making this easier
9/12/14  - Allegro has been implemented
9/19/14  - Added framework required for displaying images with Allegro (No images yet, the code is commented ATM)
9/20/14  - Cleaned up the class structure and pre-main code a LOT
9/20/14  - Made program draw things rather than just print text
9/21/14  - Added a debouncer
9/21/14  - Put classes in their own headers
9/23/14  - Tried multiple ways to put functions with class headers, did not work, 3/10, would not reccomend to a friend
10/3/14  - Hey past me, you are dumb. I did classes for real now and its better
10/4/14  - After many many many many hours of work, I've finished the whole classes thing. It's now so much easier to read overall.
10/18/14 - Added ifndef guards to allow for machines without SQLAPI to run the offline mode
01/31/15 - Started working on this again. Cool idea: Have every module be a draggable borderless window that is connected to the (static) buses by a FF12 style arc
*/

#include <allegro.h>			//Graphics
#include <winalleg.h>			//SYSTEMTIME
#include <iostream>				//Standard I/O
#include <fstream>				//File I/O
#include <string>				//C++ std::string manipulations
#include <map>					//Maps to handle keyboard input
#include <SQLAPI.h>				//Database I/O
#include "main.h"

//#define OFFLINE 				//When this flag is set, the program is running in offline mode (Will not attempt to connect to Server)
//#define FULLSCREEN				//When this flag is set, the program will run in fullscreen mode. Otherwise, it will run in windowed mode. Fullscreen mode does not work on all computers

std::vector<Module> module(NUM_MODULES, Module());
std::vector<Bus> bus(NUM_BUSES, Bus());
int radPC;				
int rcon_lvl;				
int update_id = 0;
Mode mode;
SYSTEMTIME t;						//The program's local time, used when updating
int oldrcon_lvl;						//The previous reactor containment level (Used for updating database)
std::map<char, int> key_map;
double time1;
double time2;
double d_time;
char startType = "";
SAConnection con;
SACommand cmd;
BITMAP *buffer;

void displayModule(Module);
void displayBus(Bus);
bool update(char);					//Update all required parts of the program based on the key input trapped by getInput()
void draw();
int findByName(std::string);				//Returns the ID of a module given its truncated name ("HabitatReactor" returns 0)
void updateDatabase(Module);		//Update the SQL database with all the information about the given module
void updateFromDatabase(int);		//Read in the database. Mode 0 reads in piloting information only, mode 1 reads the whole thing
int getUpdateID();					//Returns the current engineering update ID
void comment();

int main() {
	#ifndef OFFLINE
		std::ifstream inFile ("serverLocation.txt");
		std::string serverComputer;
		std::getline(inFile, serverComputer);
		inFile.close();
		serverComputer += "@spacesimServer";
		char *databaseName = (char*) serverComputer.c_str();
		try {
			con.Connect(databaseName, "root", "isotope", SA_MySQL_Client);
		} catch(SAException &x) {
			std::cout << "Error: " << x.ErrText() << std::endl;
			system("pause");
			exit(1);
		}
	#endif 

	//Initialize all local variables
	char in = ' ';                                              //Holds user input
	d_time = 500;												//Number of miliseconds between updates
	int radPC = 0;
	int rcon_lvl = 0;
	std::string input = "";

	//Initialize the Key Input map
	key_map['z'] = 0;																			//Z = Habitat Reactor
	key_map['n'] = 1;																			//N = Fuel Cell
	key_map['p'] = 2;																			//P = Battery
	key_map['a'] = 3;																			//A = Radiation Shield 1
	key_map['b'] = 4;																			//B = Radiation Shield 2
	key_map['c'] = 5;																			//C = Artificial Gravity
	key_map['d'] = 6;																			//D = Reactor Containment 1
	key_map['e'] = 7;																			//E = Reactor Containment 2
	key_map['f'] = 8;																			//F = RCS Engines
	key_map['g'] = 9;																			//G = I ACTUALLY HAVE NO IDEA
	key_map['h'] = 10;																			//H = Engine Accelerator 1
	key_map['i'] = 11;																			//I = Ion Engine 1
	key_map['j'] = 12;																			//J = Engine Accelerator 2
	key_map['k'] = 13;																			//K = Ion Engine 2
	key_map['l'] = 14;																			//L = Engine Accelerator 3
	key_map['m'] = 15;																			//M = Ion Engine 3
	key_map['n'] = 16;																			//N = Engine Accelerator 4
	key_map['o'] = 17;																			//O = Ion Engine 4

	allegro_init();
	set_color_depth(32);
	
	#ifdef 
		set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	#else
		set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	#endif
	
	install_keyboard();
	buffer = create_bitmap(SCREEN_W, SCREEN_H);

	//Set up all the module information
	//TODO: Maybe make this read from the database?
	module[0] = Module("Habitat Reactor", "HabitatReactor", 0, 0, 10000, 100, 100);
	module[1] = Module("Fuel Cell", "FuelCell", 1, 10000);
	module[2] = Module("Battery", "Battery", 2, 10000);
	module[3] = Module("Radiation Shield 1", "RadiationShield1", 3, 10000);
	module[4] = Module("Radiation Shield 2", "RadiationShield2", 4, 10000);
	module[5] = Module("Artificial Gravity", "ArtificialGravity", 5, 10000);
	module[6] = Module("Reactor Containment 1", "ReactorContainment1", 6, 5000);
	module[7] = Module("Reactor Containment 2", "ReactorContainment2", 7, 5000);
	module[8] = Module("RCS Pressure System", "RCSPressureSystem", 8, 5000);
	module[9] = Module("Engine Accelerator 1", "EngineAccelerator1", 9, 10000);
	module[10] = Module("IonEngine1", "IonEngine1", 10, 10000);
	module[11] = Module("Engine Accelerator 2", "EngineAccelerator2", 11, 10000);
	module[12] = Module("IonEngine2", "IonEngine2", 12, 10000);
	module[13] = Module("Engine Accelerator 3", "EngineAccelerator3", 13, 10000);
	module[14] = Module("IonEngine3", "IonEngine3", 14, 10000);
	module[15] = Module("Engine Accelerator 4", "EngineAccelerator4", 15, 10000);
	module[16] = Module("IonEngine4", "IonEngine4", 16, 10000);

	textprintf_ex(screen, font, SCREEN_W / 2 - (text_length(font, "Press H to hot start or C to cold start") / 2), SCREEN_H / 2 - (text_height(font) / 2), makecol(255, 255, 255), makecol(0, 0, 0), "Press H to hot start or C to cold start");

	while (startType == '') {
		if (key[KEY_H])
			startType = 'h';
		else if (key[KEY_C])
			startType = 'c';
	}

	if (startType == 'h') {																																																																																																																																																																																																																																																																																																																																																												
		module[findByName("HabitatReactor")].setPowered(true);
		module[findByName("HabitatReactor")].setTemp(81.0F);
		module[findByName("HabitatReactor")].setWire(true);
		rcon_lvl = 2000;
	}
	else if (startType != 'c') {
		std::cout << "Invalid start type." << std::endl;
		system("PAUSE");
		exit(1);     
	}
	
	//Why. Just why.
	GetLocalTime(&t);
	time1 = t.wMilliseconds + (t.wSecond * 1000);
	//Please remove this god-forsaken windows-dependant terrible code.
	
	do {
		if(keypressed()) {
			if (key[KEY_TAB]) 
				in = '!';
			else if (key[KEY_PGUP])
				in = '@';
			else if (key[KEY_PGDN])
				in = '#';
			else if (key[KEY_LEFT])
				in = '$';
			else if (key[KEY_RIGHT])
				in = '%';
			else if (key[KEY_Q] || key[KEY_ESC])
				exit(EXIT_SUCCESS);
			else
				in = readkey();
			input += in;
			
			//This is the worst debouncer i have ever seen
			clear_keybuf();
			rest(10);
		}
		else 
			in = ' ';
		
		//Update the software based on the input
		bool change = update(in);
		
		//If something has changed, then draw the updated screens
		if(change) 
			draw();
	} while(true);
	return 0;
}
END_OF_MAIN()

bool update(char key_in) {         
	bool change = false;      
	GetLocalTime(&t);
	time2 = t.wMilliseconds + (t.wSecond * 1000);  
	if(time2 - time1 >= d_time || time2 - time1 < 0) {
		time1 = time2;
		for(int i = 0; i != module.size() - 1; i++) {
			module[i].heat();        
		}         
		if(module[findByName("ReactorContainment1")].isPowered() == false && module[findByName("ReactorContainment2")].isPowered() == false) {
			rcon_lvl -= 8; 
		}
		else if(!module[findByName("ReactorContainment1")].isPowered() != !module[findByName("ReactorContainment1")].isPowered()) {		//Protip: (!A != !B) is a really easy way to do A XOR B. The ! before A and B converts it to a bool if it wasnt already, and doesn't affect the answer at all
			rcon_lvl += 8;
		}
		else {
			rcon_lvl += 16;
		}
		if(rcon_lvl > 2000) rcon_lvl = 2000;
		else if(rcon_lvl < 0) rcon_lvl = 0;
	
		#ifndef OFFLINE
			cmd.setConnection(&con);
			std::string com = "UPDATE engVars SET val = " + std::to_string(rcon_lvl) + " WHERE name = 'rcon_lvl'";
			cmd.setCommandText(com.c_str());
			cmd.Execute();
			con.Commit();
		#endif
		change = true;
	}
  	if(key_in == 'x') {                                                  //Put here key inputs not specific to a module
		if(module[findByName("HabitatReactor")].getTemp() >= 81) {
			module[findByName("HabitatReactor")].setPowered(true);
			bus[0].power();
			module[findByName("HabitatReactor")].power();
			change = true;
		}
	}
	else if(key_in == '!') {
		if(mode == electricity)
			mode = temperature;
		else if(mode == temperature)
			mode = cool;
		else if(mode == cool)
			mode = electricity;
	}
	else if(key_in == '$') {
		radPC -= 1;
		if(radPC < 0) radPC = 0;
		#ifndef OFFLINE
			cmd.setConnection(&con);
			std::string com = "UPDATE engVars SET val = " + std::to_string(radPC) + " WHERE name = 'radPC'";
			cmd.setCommandText(com.c_str());
			cmd.Execute();
			con.Commit();
		#endif
		change = true;
	}
	else if(key_in == '%') {
		radPC += 1;
		if(radPC > 100) radPC = 100;
		#ifndef OFFLINE
			cmd.setConnection(&con);
			std::string com = "UPDATE engVars SET val = " + std::to_string(radPC) + " WHERE name = 'radPC'";
			cmd.setCommandText(com.c_str());
			cmd.Execute();
			con.Commit();
		#endif
		change = true;
	}
	else if(key_map.find(key_in) != key_map.end()) {								//If the input is part of the keymap
		int key_val = key_map[key_in];												//Assign an integer value based on the input
		if(key_val > 2) {														//If the key value is > 2 (Non-power source)
			bus[module[key_val].getBusNum()].power();                            //Check the power of the affected bus
			module[key_val].toggleWire();										//Toggle the wire
			module[key_val].power();												//Check the power of the module
		}
		else if(key_val <= 2) {													//Otherwise, (It is a power source)
			int last_val = 0;													//Initialize a variable called lastVal
			module[key_val].toggleWire();										//Toggle the power source wire (Don't check power yet)
			module[key_val].power();												//jk check power cause yeah (no idea what i was thinking here)
			for(int i = 0; i < NUM_BUSES; i++) {								//For every bus
				bool old_power = bus[i].isPowered();								//Store their power state in a variable  
				bus[i].power();                                                 //Check to see if they are powered
				if(old_power != bus[i].isPowered()) {							//If their new powered value and the varaible are diferent
					for(int j = last_val; j != module.size() - 1; j++) {			//We need to update power states for everything on that bus
						if(module[j].getBusNum() == i) {                        //This uses lastVal, so we don't make unnessecary comparisons
							module[key_val].power();
						}
						else {
							last_val = j;                                        //Setting lastVal to j, so we know that the next bus has module starting at this number
							break;     
						}     
					}            
				}
			}  
		}
		change = true;                                                      //Record that a change has been made
	}
	else 
		for(int i = 0; i < NUM_BUSES; i++) 
			bus[i].power();		
	
	for(int i = 3; i != module.size() - 1; i++) {                             //Calculate new power values for the buses (Should only need to do this if change is true, investigate later)
		if(module[i].isPowered()) {
			if(module[i].getTruncName() == "RadiationShield1" || module[i].getTruncName() == "RadiationShield2") 
				bus[module[i].getBusNum()].setWatts(bus[module[i].getBusNum()].getWatts() - (module[i].getWatts() * radPC * (1/EFFICIENCY)));
			else 
				bus[module[i].getBusNum()].setWatts(bus[module[i].getBusNum()].getWatts() - (module[i].getWatts() * (1/EFFICIENCY)));
			
			if(bus[module[i].getBusNum()].getWatts() < 0) {																//If the bus cannot provide enough wattage for a module
				module[i].setPowered(false);																					//Power off that module
				if(module[i].getName() == "RadiationShield1" || module[i].getName() == "RadiationShield2") 
					bus[module[i].getBusNum()].setWatts(bus[module[i].getBusNum()].getWatts() + (module[i].getWatts() * radPC * (1/EFFICIENCY)));
				else 
					bus[module[i].getBusNum()].setWatts(bus[module[i].getBusNum()].getWatts() + (module[i].getWatts() * (1/EFFICIENCY)));  //Return the spent watts
			}
		}
		#ifndef OFFLINE
			if((module[i].getTemp() != module[i].getOldTemp() || module[i].getWire() != module[i].getOldWire() || module[i].isPowered() != module[i].getOldPowered() || module[i].getStatus() != module[i].getOldStatus())) {
				module[i].setOldTemp(module[i].getTemp());
				module[i].setOldWire(module[i].getWire());
				module[i].setOldPowered(module[i].isPowered());
				module[i].setOldStatus(module[i].getStatus());
				updateDatabase(module[i]);
			}
		#endif
	}
	return change;
}

void displayModule(Module m) {
	masked_blit(m.getSprite(m.getStatus()), buffer, 0, 0, m.getX(), m.getY(), m.getSprite(m.getStatus())->w, m.getSprite(m.getStatus())->h);
	std::string stats;
	switch(mode) {
		case electricity:
			if(m.getTruncName() == "RadiationShield1" || m.getTruncName() == "RadiationShield2")
				stats = std::to_string(int(m.getWatts() * m.isPowered() * radPC)) + "W";
			else 
				stats = std::to_string(int(m.getWatts() * m.isPowered())) + "W";
			break;
		case temperature:
			stats = std::to_string(m.getTemp());
			break;
		case cool:
			//stats = name + " Wire: " + std::to_string(wire) + " Powered: " + std::to_string(powered) + " Temp: " + std::to_string(temp) + " ID: " + std::to_string(id) + " Status: " + std::to_string((int)status);
			break;
	}
	textprintf_ex(buffer, font, m.getX() + ((m.getSprite(m.getStatus())->w - text_length(font, stats.c_str())) / 2), m.getY() + ((m.getSprite(m.getStatus())->h - text_height(font)) / 2), makecol(255, 255, 255), -1, stats.c_str());
}

void displayBus(Bus b) {
	//		string stats = "BUS ";
	//		stats += std::to_string(busNum + 1);
	//		stats += " STATISTICS";
	//		int width = (80 - stats.length()) / 2;
	//		for(int i = 0; i < width; i++) //cout << " ";
	//		cout << stats << endl;
	//		if(busNum == 0) cout << "RADIATION SHIELD PERCENTAGE: " << radPC << " REACTOR CONTAINMENT LEVELS: " << rcon_lvl << endl;
	//		cout << "POWERED: " << powered << " WATTS: " << watts << "W";
	//		if(source != -1) cout << " POWER SOURCE: " << module[source].name;
	//		else cout << " POWER SOURCE: NOT POWERED";
	//		cout << " TIME: " << t.wMilliseconds + (t.wSecond * 1000) << endl << endl;
	//		if(source != -1) {
	//			cout << "POWER SOURCE INFO: " << endl;
	//			module[source].display();
	//			cout << endl << endl;
	//		}
	//		else cout << "NOT POWERED" << endl << endl;
	//		textprintf_ex(screen, font, 100, 100, makecol(255, 255, 255), makecol(0, 0, 0), stats.c_str());
	//		stats = "RADIATION SHIELD PERCENTAGE: " + std::to_string(radPC) + " REACTOR CONTAINMENT LEVELS: " + std::to_string(rcon_lvl);
	//		textprintf_ex(screen, font, 100, 110, makecol(255, 255, 255), makecol(0, 0, 0), stats.c_str());
	//		if(source != -1) stats = "POWER SOURCE: " + module[source].name;
	//		else stats = "POWER SOURCE: NOT POWERED";
	///		textprintf_ex(screen, font, 100, 120, makecol(255, 255, 255), makecol(0, 0, 0), stats.c_str());
	//		if(source != -1); //show it not being powered
	for(int i = 3; i != module.size() - 1; i++) {
		if(module[i].getBusNum() == b.getBusNum()) 
			displayModule(module[i]);
		else 
			break;
	}
	if(b.getSource() != -1) 
		displayModule(module[b.getSource()]);
}

void draw() {
	clear_to_color(buffer, makecol(0, 0, 0));
	displayBus(bus[0]);
	blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
}

int findByName(std::string fName) {
	for(int i = 0; i < NUM_MODULES; i++) 
		if(module[i].getTruncName() == fName) 
			return i;         
	std::cout << "Cannot find module with name " << fName << ", please contact your local adminispencer" << std::endl;
	system("pause");
	exit(1);
}

void updateDatabase(Module m) {
	std::string cmd_text = "UPDATE eng SET temp = " + std::to_string(m.getTemp()) + ", wire = " + std::to_string(m.getWire()) + ", powered = " + std::to_string(m.isPowered()) + ", status = " + std::to_string(m.getStatus()) + ", watts = " + std::to_string(m.getWatts()) + " WHERE name = '" + m.getTruncName() + "'";
	cmd.setConnection(&con);
	cmd.setCommandText(cmd_text.c_str());
	cmd.Execute();
	con.Commit();
}

void updateFromDatabase(int update_mode) {
	if(update_mode) {
		for(int i = 0; i < module.size(); ++i) {
			std::string cmd_text = "SELECT name, truncName, thresh1, thresh2, temp, wire, powered, status FROM eng WHERE id = " + module[i].getName();
			cmd.setConnection(&con);
			cmd.setCommandText(cmd_text.c_str());
			cmd.Execute();
			con.Commit();
			while(cmd.FetchNext()) {
				module[i].setName((std::string)cmd.Field("name").asString());
				module[i].setTruncName((std::string)cmd.Field("truncName").asString());
				module[i].setThresh1((float)cmd.Field("thresh1").asDouble());
				module[i].setThresh2((float)cmd.Field("thresh2").asDouble());
				module[i].setTemp((float)cmd.Field("temp").asDouble());
				module[i].setWire(cmd.Field("wire").asBool());
				module[i].setPowered(cmd.Field("powered").asBool());
				module[i].setStatus((Status)(int)cmd.Field("status").asLong());
			}
		}
	}
}
