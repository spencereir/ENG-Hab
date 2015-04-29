#ifndef MAIN_H_
#define MAIN_H_

#define NUM_MODULES 17
#define NUM_BUSES 3
#define EFFICIENCY 0.99
#define PRIMARY_BUS_WATTS 2500000		//The default number of watts on the primary bus
#define SECONDARY_BUS_WATTS 10000		//The default number of watts on the secondary bus
#define PRIMARY_HEAT_RATIO 100000		//Bigger this is = Smaller increments in which heat goes up
#define PRIMARY_COOLING_BASELINE 1		//Bigger this is = Larger temperature reductions every tick
#define SECONDARY_HEAT_RATIO 50000
#define SECONDARY_COOLING_BASELINE 0.5
#define TERNARY_HEAT_RATIO 10000
#define TERNARY_COOLING_BASELINE 0.1
#define GENERAL_COOLING_BASELINE 1		//The amount that non-powersource module cool off every tick
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT (SCREEN_WIDTH * 0.75)
#define FONT_SIZE 18
#define SDL_INIT_ERROR 1
#define SQL_INIT_ERROR 2

#include <string>
#include <vector>

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
	noCoolant,
	leftCoolant,
	rightCoolant,
	bothCoolant
};

int findByName(std::string name);

#endif