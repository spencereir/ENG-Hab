#include "module.h"

extern std::vector<Module> module;
extern std::vector<Bus> bus;
extern int radPC;
extern int rcon_lvl;

Module::Module() {
	name = "Default";
	truncName = "Default";
	wire = false;
	powered = false;
	id = 0;
	busNum = 0;
	temp = 0.0F;
	watts = 0;
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = 0;
	y = 25 * id;
}

Module::Module(int a, SDL_Renderer* context) {
	name = "Default";
	truncName = "Default";
	wire = false;
	powered = false;
	id = a;
	busNum = 0;
	temp = 0.0F;
	watts = 0;
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = 0;
	y = 25 * id;
	sprite[0] = load_bitmap("res/singleModuleNormal.bmp", context);
	sprite[1] = load_bitmap("res/singleModuleWarning.bmp", context);
	sprite[2] = load_bitmap("res/singleModuleDestroyed.bmp", context);
}

Module::Module(std::string a, std::string b, int c, float d, SDL_Renderer* context) {
	name = a; 
	truncName = b; 
	wire = false;
	powered = false;
	id = c; 
	busNum = 0;
	temp = 0.0F;
	watts = d; 
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = 0;
	y = 25 * id;
	sprite[0] = load_bitmap("res/singleModuleNormal.bmp", context);
	sprite[1] = load_bitmap("res/singleModuleWarning.bmp", context);
	sprite[2] = load_bitmap("res/singleModuleDestroyed.bmp", context);
}

Module::Module(std::string a, std::string b, int c, int d, float e, SDL_Renderer* context) {
	name = a; 
	truncName = b; 
	wire = false;
	powered = false;
	id = c; 
	busNum = d;
	temp = 0.0F;
	watts = e; 
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = 0;
	y = 25 * id;
	sprite[0] = load_bitmap("res/singleModuleNormal.bmp", context);
	sprite[1] = load_bitmap("res/singleModuleWarning.bmp", context);
	sprite[2] = load_bitmap("res/singleModuleDestroyed.bmp", context);
}

Module::Module(std::string a, std::string b, int c, int d, float e, int g, SDL_Renderer* context) {
	name = a; 
	truncName = b; 
	wire = false;
	powered = false;
	id = c; 
	busNum = d;
	temp = 0.0F;
	watts = e; 
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = 0;
	y = 25 * id;
	if(g == 1) {
		sprite[0] = load_bitmap("res/singleModuleNormal.bmp", context);
		sprite[1] = load_bitmap("res/singleModuleWarning.bmp", context);
		sprite[2] = load_bitmap("res/singleModuleDestroyed.bmp", context);
	}
	else if(g == 2) {
		//Double modules (RCON, RADSHIELD)
	}
	else if(g == 3) {
		//Special modules from this point out (Reactor, RCSP)
	}
}

Module::Module(std::string a, std::string b, int c, int d, float e, int g, int h, SDL_Renderer* context) {
	name = a; 
	truncName = b; 
	wire = false;
	powered = false;
	id = c; 
	busNum = d;
	temp = 0.0F;
	watts = e; 
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = g;
	y = h;
	sprite[0] = load_bitmap("res/singleModuleNormal.bmp", context);
	sprite[1] = load_bitmap("res/singleModuleWarning.bmp", context);
	sprite[2] = load_bitmap("res/singleModuleDestroyed.bmp", context);
}

Module::Module(std::string a, std::string b, int c, int d, float e, int g, int h, int k, SDL_Renderer* context) {
	name = a; 
	truncName = b; 
	wire = false;
	powered = false;
	id = c; 
	busNum = d;
	temp = 0.0F;
	watts = e; 
	thresh1 = 100.0F;
	thresh2 = 125.0F;
	status = normal;
	x = h;
	y = k;
	if(g == 1) {
		sprite[0] = load_bitmap("res/singleModuleNormal.bmp", context);
		sprite[1] = load_bitmap("res/singleModuleWarning.bmp", context);
		sprite[2] = load_bitmap("res/singleModuleDestroyed.bmp", context);
	}
	else if(g == 2) {
		//Double modules (RCON, RADSHIELD)
	}
	else if(g == 3) {
		//Special modules from this point out (Reactor, RCSP)
	}
}

float Module::heat() {
	if(powered && status != destroyed) {
		//Heat calculations here  
		if(truncName == "HabitatReactor" || truncName == "Battery" || truncName == "FuelCell") {
			if(truncName == "HabitatReactor") {
				temp += (PRIMARY_BUS_WATTS - bus[0].getWatts()) / PRIMARY_HEAT_RATIO - PRIMARY_COOLING_BASELINE;
				if(temp < 81) temp = 81;
			}
			else if(truncName == "FuelCell") {
				if(/*thingthatfindsifbus1&2areconnected*/true && !module[findByName("HabitatReactor")].isPowered()) {  //Check to see if bus 1 and 2 are connected, and see if fuelcell is powering them (Habitat Reactor will auto shutoff fuel cell
					temp += (PRIMARY_BUS_WATTS - bus[0].getWatts()) / SECONDARY_HEAT_RATIO - SECONDARY_COOLING_BASELINE;
				}
				temp += (SECONDARY_BUS_WATTS - bus[1].getWatts()) / SECONDARY_HEAT_RATIO - SECONDARY_COOLING_BASELINE;
			}
			else if(truncName == "Battery") {
				if(/*seeabove*/true && !module[findByName("HabitatReactor")].isPowered() && !module[findByName("HabitatReactor")].isPowered()) {
					temp += (PRIMARY_BUS_WATTS - bus[0].getWatts()) / TERNARY_HEAT_RATIO - TERNARY_COOLING_BASELINE;
				}								
				temp += (SECONDARY_BUS_WATTS - bus[0].getWatts()) / TERNARY_HEAT_RATIO - TERNARY_COOLING_BASELINE;
			}
		}
	}
	else if(status != destroyed) 
		temp -= GENERAL_COOLING_BASELINE;
	if(temp > thresh1) {
		if(temp > thresh2) {
			status = destroyed;
			power();
		}        
		else status = warning;
	}         
	else if(temp < 0) temp = 0;
	return temp;
}

void Module::power() {
	if(wire && status != destroyed) {
		if(bus[busNum].isPowered()) powered = true;                      
		else powered = false;       
	}
	else powered = false;
}
