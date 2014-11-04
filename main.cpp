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
                       Fixed by Will Xyen
					  
If you change anything here or find anything wrong with your changes, please record everything in the Changelog and
the TODO. It just really makes everything easier for everyone. Additionally, you can message me (See above) to get
contributor privilages on git, but please please pleease dont push to master until you are 100% sure in all circumstances
that your fix works. I can create a branch for you while we delegate to QA (Is that a taskforce? That should be a taskforce)
*/

#include "xSDL/xSDL_bitmap.h"	//Graphics
#include "xSDL/xSDL_allegro_wrapper.h"

// Event Handler
char processEvents(SDL_Window* window);

// Window
xSDL_Window *target_window;

// fix godamn overwrite
#define WIN32_LEAN_AND_MEAN
#define BITMAP WINDOWS_BITMAP
	#include <windows.h>
#define WINDOWS_RGB(r,g,b)  ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#undef BITMAP


#define OFFLINE 				//When this flag is set, the program is running in offline mode (Will not attempt to connect to Server)
//#define FULLSCREEN				//When this flag is set, the program will run in fullscreen mode. Otherwise, it will run in windowed mode. Fullscreen mode does not work on all computers

#ifndef OFFLINE
	#include <SQLAPI.h>				//Database I/O
	SAConnection con;
	SACommand cmd;
#endif

#include "main.h"
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
std::string startType = "";
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

int main(int argc, char *argv[]){
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
		int radPC = 0;
		int rcon_lvl = 0;
	#endif 

	//Initialize all local variables
	char in = ' ';                                              //Holds user input
	d_time = 500;												//Number of miliseconds between updates
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

	
	target_window = new xSDL_Window("ENG-hab", SCREEN_WIDTH, SCREEN_HEIGHT,
	#ifdef FULLSCREEN
		true
	#else
		false
	#endif
	);
	
	// install_keyboard();
	// buffer = create_bitmap(SCREEN_W, SCREEN_H);

	//Set up all the module information
	module[0] = Module("Habitat Reactor", "HabitatReactor", 0, 0, 10000, 100, 100, target_window->renderer);
	module[1] = Module("Fuel Cell", "FuelCell", 1, 10000, target_window->renderer);
	module[2] = Module("Battery", "Battery", 2, 10000, target_window->renderer);
	module[3] = Module("Radiation Shield 1", "RadiationShield1", 3, 10000, target_window->renderer);
	module[4] = Module("Radiation Shield 2", "RadiationShield2", 4, 10000, target_window->renderer);
	module[5] = Module("Artificial Gravity", "ArtificialGravity", 5, 10000, target_window->renderer);
	module[6] = Module("Reactor Containment 1", "ReactorContainment1", 6, 5000, target_window->renderer);
	module[7] = Module("Reactor Containment 2", "ReactorContainment2", 7, 5000, target_window->renderer);
	module[8] = Module("RCS Pressure System", "RCSPressureSystem", 8, 5000, target_window->renderer);
	module[9] = Module("That Other One", "ThatOtherOne", 9, 10000, target_window->renderer);
	module[10] = Module("Engine Accelerator 1", "EngineAccelerator1", 10, 10000, target_window->renderer);
	module[11] = Module("IonEngine1", "IonEngine1", 11, 10000, target_window->renderer);
	module[12] = Module("Engine Accelerator 2", "EngineAccelerator2", 12, 10000, target_window->renderer);
	module[13] = Module("IonEngine2", "IonEngine2", 13, 10000, target_window->renderer);
	module[14] = Module("Engine Accelerator 3", "EngineAccelerator3", 14, 10000, target_window->renderer);
	module[15] = Module("IonEngine3", "IonEngine3", 15, 10000, target_window->renderer);
	module[16] = Module("Engine Accelerator 4", "EngineAccelerator4", 16, 10000, target_window->renderer);
	module[17] = Module("IonEngine4", "IonEngine4", 17, 10000, target_window->renderer);

	clear_to_color(target_window->renderer, makecol(0, 0, 0));
	textprintf_ex(target_window->renderer, NULL, SCREEN_WIDTH / 2 - (text_length(NULL, "Press H to hot start or C to cold start") / 2), SCREEN_HEIGHT / 2 - (text_height(NULL) / 2), makecol(255, 255, 255), NULL, "Press H to hot start or C to cold start");
	SDL_RenderPresent(target_window->renderer);
	std::cout << "a4";
	while(startType == "") {
		SDL_PumpEvents();
		if(target_window->keyboardState[SDL_SCANCODE_H])
			startType = "h";
		else if(target_window->keyboardState[SDL_SCANCODE_C])
			startType = "c";
	}

	if(startType == "h") {																																																																																																																																																																																																																																																																																																																																																												
		module[findByName("HabitatReactor")].setPowered(true);
		module[findByName("HabitatReactor")].setTemp(81.0F);
		module[findByName("HabitatReactor")].setWire(true);
		rcon_lvl = 2000;
	}
	else if(startType == "c") {
		//If cold starting requires anything special, put that here
	}
	else {
		std::cout << "Invalid start type." << std::endl;
		system("PAUSE");
		exit(1);     
	}

	GetLocalTime(&t);
	time1 = t.wMilliseconds + (t.wSecond * 1000);
	do {
		in = processEvents(target_window->window);
		input += in;
		
		bool change = update(in);
		if(change)
			draw();
	} while(true);
	return 0;
}

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
	masked_blit(m.getSprite(m.getStatus()), target_window->renderer, 0, 0, m.getX(), m.getY(), m.getSprite(m.getStatus())->w, m.getSprite(m.getStatus())->h);
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
	textprintf_ex(target_window->renderer, NULL, m.getX() + ((m.getSprite(m.getStatus())->w - text_length(NULL, stats.c_str())) / 2), m.getY() + ((m.getSprite(m.getStatus())->h - text_height(NULL)) / 2), makecol(255, 255, 255), NULL, stats.c_str());
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
	clear_to_color(target_window->renderer, makecol(0, 0, 0));
	displayBus(bus[0]);
	SDL_RenderPresent(target_window->renderer);
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
	#ifndef OFFLINE
		cmd.setConnection(&con);
		cmd.setCommandText(cmd_text.c_str());
		cmd.Execute();
		con.Commit();
	#endif
}

void updateFromDatabase(int update_mode) {
	if(update_mode) {
		for(int i = 0; i < module.size(); ++i) {
			std::string cmd_text = "SELECT name, truncName, thresh1, thresh2, temp, wire, powered, status FROM eng WHERE id = " + module[i].getName();
			#ifndef OFFLINE
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
			#endif
		}
	}
}
