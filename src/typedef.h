#ifndef TYPEDEF_H_
#define TYPEDEF_H_

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

#endif