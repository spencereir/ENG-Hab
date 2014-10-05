#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>

#pragma comment(lib, "sqlapi.lib")		//Link to the SQL API
#pragma comment(lib, "alld.lib")		//Link to the Allegro graphics engine

#define NUM_MODULES 18
#define NUM_BUSES 3
#define EFFICIENCY 0.99
#define PRIMARY_BUS_WATTS 2500000		//The default number of watts on the primary bus
#define SECONDARY_BUS_WATTS 10000		//The default number of watts on the secondary bus
#define PRIMARY_HEAT_RATIO 100000		//Bigger this is = Smaller increments in which heat goes up
#define PRIMARY_COOLING_BASELINE 1		//Bigger this is = Larger temperature reductions every tick (1 tick = dTime milliseconds, assigned in the main function)
#define SECONDARY_HEAT_RATIO 50000
#define SECONDARY_COOLING_BASELINE 0.5
#define TERNARY_HEAT_RATIO 10000
#define TERNARY_COOLING_BASELINE 0.1
#define GENERAL_COOLING_BASELINE 1		//The amount that non-powersource module cool off every tick
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT (SCREEN_WIDTH * 0.75)

enum Status {					//Holds the status of a module
	normal,
	warning,
	destroyed
};

enum Mode {						//The viewmode
	electricity,
	temperature,
	cool
};

enum Coolant {					//All coolants
	leftCoolant,
	rightCoolant,
	bothCoolant
};

int findByName(std::string name);

#include <vector>
#include "module.h"
#include "bus.h"

#endif