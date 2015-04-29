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
02/09/15 - Ok. So I'm going to do this. I'm going to implement SDL.
02/09/15 - Online mode is broken. Fairly convinced that its possessed. Should fix it. Not today. Can connect from remote hosts, is good enough.
02/11/15 - SDL pretty much implemented, lots of code readability changes. Online still doesn't work
*/

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_ttf.h>
#include <iostream>				//Standard I/O
#include <fstream>				//File I/O
#include <string>				//C++ std::string manipulations
#include <map>					//Maps to handle keyboard input
#include <SQLAPI.h>				//Database I/O
#include "main.h"
#include "module.h"
#include "bus.h"

#define OFFLINE 				//When this flag is set, the program is running in offline mode (Will not attempt to connect to Server)
//#define FULLSCREEN				//When this flag is set, the program will run in fullscreen mode. Otherwise, it will run in windowed mode. Fullscreen mode does not work on all computers

int radPC;				
int rcon_lvl;				
int oldrcon_lvl;						//The previous reactor containment level (Used for updating database)
int update_id = 0;
Mode mode;
std::map<char, int> key_map;
std::vector<Module> module(NUM_MODULES, Module());
std::vector<Bus> bus(NUM_BUSES, Bus());
SAConnection con;
SACommand cmd;

void draw(SDL_Window *window, TTF_Font *font);
void drawText(SDL_Window *window, TTF_Font *font, std::string message, int x, int y, int r, int g, int b);
bool update(SDL_Event);				//Update all required parts of the program
int findByName(std::string);		//Returns the ID of a module given its truncated name ("HabitatReactor" returns 0)
void updateDatabase(Module);		//Update the SQL database with all the information about the given module
void updateFromDatabase(int);		//Read in the database. Mode 0 reads in piloting information only, mode 1 reads the whole thing
int getUpdateID();					//Returns the current engineering update ID
Uint32 heatTimer(Uint32 interval, void* param);		//Callback function that regulates heating in update()

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);             
	TTF_Init();
	SDL_Window *window;                    
    SDL_Event event;
	TTF_Font *font;

    window = SDL_CreateWindow(
        "ENG Hab",                  
        SDL_WINDOWPOS_UNDEFINED,           
        SDL_WINDOWPOS_UNDEFINED,           
        SCREEN_WIDTH,                               
        SCREEN_HEIGHT,                               
		#ifdef FULLSCREEN
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
		#else
			SDL_WINDOW_OPENGL
		#endif
    );

	if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Initializaton Error", "Failed to create a SDL window. The program will now exit.", NULL);
        return SDL_INIT_ERROR;
    }

	font = TTF_OpenFont("Helvetica.ttf", FONT_SIZE);
	#ifndef OFFLINE
		std::ifstream inFile ("serverLocation.dat");
		std::string serverComputer;
		std::getline(inFile, serverComputer);
		inFile.close();
		serverComputer += "@spacesimServer";
		char *databaseName = (char*) serverComputer.c_str();
		try {
			con.Connect(databaseName, "spacesim", "isotope", SA_MySQL_Client);
		} catch(SAException &x) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SQL Initialization Error", "SQLAPI++ could not connect to database. Is the server computer on?", NULL);
			return SQL_INIT_ERROR;
		}
	#endif 

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

	
	#ifndef OFFLINE
	//TODO: Read the module information from database
	#else
		std::ifstream inFile ("defaultEngineeringValues.dat"); 
		for (int i = 0; i < NUM_MODULES; i++) {
			std::string a;
			std::string b;
			bool c; 
			bool d;
			int e;
			int f;
			float g; 
			float h; 
			float j; 
			float k; 
			int l; 
			int m;
			std::getline(inFile, a);
			inFile >> b >> c >> d >> e >> f >> g >> h >> j >> k >> l >> m;
			module[i].setValues(a, b, c, d, e, f, g, h, j, k, (Status) l, (Coolant) m);
			std::getline(inFile, a);
		}
	#endif
	
	bool start = false;

	do {
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					return 0;
				
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_h:
							module[findByName("HabitatReactor")].setPowered(true);
							module[findByName("HabitatReactor")].setTemp(81.0F);
							module[findByName("HabitatReactor")].setWire(true);
							rcon_lvl = 2000;
							start = true;
							break;

						case SDLK_c:
							start = true;
							break;
					}
					break;
			}
		}
		int w, h;
		TTF_SizeText(font, "Press H to hot start, or C to cold start", &w, &h);
		SDL_Surface *window_surface = SDL_GetWindowSurface(window);
		SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
		drawText(window, font, "Press H to hot start, or C to cold start", (SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2, 255, 255, 255);
		SDL_UpdateWindowSurface(window);
	} while (!start);

	SDL_TimerID timer;
	timer = SDL_AddTimer(500, heatTimer, NULL);

	do {
		//Update the software based on the input
		bool change = false;
		if (SDL_PollEvent(&event)) 
			change = update(event);
		
		//If something has changed, then draw the updated screens
		if(change) 
			draw(window, font);
	} while(true);

	SDL_Quit();
	TTF_Quit();
	return 0;
}

void draw(SDL_Window *window, TTF_Font *font) {
	SDL_Surface *window_surface = SDL_GetWindowSurface(window);
	SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
	for (int i = 0; i < NUM_MODULES; i++)
		drawText(window, font, module[i].getName() + ": " + std::to_string(module[i].getTemp()), 0, i * 20, 255, 255, 255);
	SDL_UpdateWindowSurface(window);
}

void drawText(SDL_Window *window, TTF_Font *font, std::string message, int x, int y, int r, int g, int b) {
	SDL_Surface *text, *window_surface;
	SDL_Color c = {r, g, b};
	window_surface = SDL_GetWindowSurface(window);
	text = TTF_RenderText_Solid(font, message.c_str(), c);
	SDL_Rect destRect = {x, y, 0, 0};
	SDL_BlitSurface(text, NULL, window_surface, &destRect);
	SDL_UpdateWindowSurface(window);
}

bool update(SDL_Event event) {         
	bool change = false;      
	switch (event.type) {
		case SDL_QUIT:
			exit(0);
				
		case SDL_KEYDOWN:
			if (key_map.find((char) event.key.keysym.sym) != key_map.end()) {
				int key_val = key_map[(char) event.key.keysym.sym];						//Assign an integer value based on the input
				if(key_val > 2) {														//If the key value is > 2 (Non-power source)
					bus[module[key_val].getBusNum()].power();                            //Check the power of the affected bus
					module[key_val].toggleWire();										//Toggle the wire
					module[key_val].power();												//Check the power of the module
				}
				else if(key_val <= 2) {													//Otherwise, (It is a power source)
					int last_val = 0;													
					module[key_val].toggleWire();										//Toggle the power source wire (Don't check power yet)
					module[key_val].power();											
					for(int i = 0; i < NUM_BUSES; i++) {								//For every bus
						bool old_power = bus[i].isPowered();							//Store their power state in a variable  
						bus[i].power();                                                 //Check to see if they are powered
						if(old_power != bus[i].isPowered()) {							//If their new powered value and the varaible are diferent
							for(int j = last_val; j != module.size() - 1; j++) {		//We need to update power states for everything on that bus
								if(module[j].getBusNum() == i)                          //This uses lastVal, so we don't make unnessecary comparisons
									module[key_val].power();
								else {
									last_val = j;                                        //Setting lastVal to j, so we know that the next bus has module starting at this number
									break;     
								}     
							}            
						}
					}  
				}
				change = true;
			}
			else {
				switch (event.key.keysym.sym) {
					case SDLK_x:
						if(module[findByName("HabitatReactor")].getTemp() >= 81) {
							module[findByName("HabitatReactor")].setPowered(true);
							bus[0].power();
							module[findByName("HabitatReactor")].power();
							change = true;
						}
						break;
					
					case SDLK_F11:
						if(mode == electricity)
							mode = temperature;
						else if(mode == temperature)
							mode = cool;
						else if(mode == cool)
							mode = electricity;
						change = true;
						break;

					case SDLK_LEFT:
						radPC -= 1;
						if(radPC < 0) 
						radPC = 0;
						#ifndef OFFLINE
							cmd.setConnection(&con);
							std::string com = "UPDATE engVars SET val = " + std::to_string(radPC) + " WHERE name = 'radPC'";
							cmd.setCommandText(com.c_str());
							cmd.Execute();
							con.Commit();
						#endif
						change = true;
						break;

					case SDLK_RIGHT:
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
						break;
					//im really not sure why this is here. when the code does stuff, try taking this out and see if stuff breaks
					default: 
						for(int i = 0; i < NUM_BUSES; i++) 
							bus[i].power();		
				}
			}
			break;

		case SDL_USEREVENT:
			switch (event.user.code) {
				case 0:
					for(int i = 0; i != module.size() - 1; i++) {
						module[i].heat();        
					}         
					if(!module[findByName("ReactorContainment1")].isPowered() && !module[findByName("ReactorContainment2")].isPowered()) {
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
					break;
			}
	}

	//what
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

Uint32 heatTimer(Uint32 interval, void* param) {
	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user.code = 0;
	event.user.data1 = 0;
	event.user.data2 = 0;
	SDL_PushEvent(&event);
	return interval;
}

int findByName(std::string fName) {
	for(int i = 0; i < NUM_MODULES; i++) 
		if(module[i].getTruncName() == fName) 
			return i;         
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Module not Found", ("Failed to find module with name " + fName + ".\nThe program will now exit.").c_str(), NULL);
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
