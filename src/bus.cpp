#include "bus.h"
#include "module.h"

extern std::vector<Module> module;
extern std::vector<Bus> bus;
extern int radPC;
extern int rcon_lvl;

Bus::Bus() {
	busNum = 0;
	powered = false;
	source = -1;
	watts = 0;
}

Bus::Bus(int a) {
	busNum = a;
	powered = false;
	source = -1;
	watts = 0;
}

//Determine whether or not the bus is powered, update variables powered and source accordingly
void Bus::power() {
	switch(busNum) {
		case 0:
			if((module[findByName("HabitatReactor")].isPowered() && module[findByName("HabitatReactor")].getWire()) || (((module[findByName("FuelCell")].isPowered() && module[findByName("FuelCell")].getWire()) || (module[findByName("Battery")].isPowered() && module[findByName("Battery")].getWire())) && true/*change this to check wires connecting bus 1/2*/)) {
				powered = true;       
				watts = PRIMARY_BUS_WATTS;
				if(module[findByName("HabitatReactor")].isPowered()) 
					source = 0;
				else if(module[findByName("FuelCell")].isPowered()) 
					source = 1;
				else if(module[findByName("Battery")].isPowered()) 
					source = 2;
			}
			else {
				powered = false;
				watts = 0;
				source = -1;
			}
			break;

		case 1:
			if(((module[findByName("HabitatReactor")].isPowered() && module[findByName("HabitatReactor")].getWire()) && true/*see above*/) || (module[findByName("FuelCell")].isPowered() && module[findByName("FuelCell")].getWire()) || (module[findByName("Battery")].isPowered() && module[findByName("Battery")].getWire())) {
				powered = true;
				watts = SECONDARY_BUS_WATTS;         
				if(module[findByName("HabitatReactor")].isPowered() && module[findByName("HabitatReactor")].getWire() && true /*wire connecty thingy */) 
					source = 0;
				else if(module[findByName("FuelCell")].isPowered()) 
					source = 1;
				else if(module[findByName("Battery")].isPowered()) 
					source = 2;
			}       	    
			else {
				powered = false;
				watts = 0;
				source = -1;
			}
			break;   
	}
}