#ifndef MODULE_H_
#define MODULE_H_

#include "xSDL/xSDL_bitmap.h"
#include <string>
#include "main.h"

class Module {
	private:    
		std::string name;		//Full name of a module
		std::string truncName;	//Truncated name, which will be used for receiving data.
		bool wire;			//The status of the wire (True = Connected, False = Disconnected)
		bool powered;		//Whether or not the module is powered (Depends on wire and its bus' power)
		int id;				//A unique ID that refers to this module alone
		int busNum;			//The bus it belongs to (0 = Primary, 1 = Secondary, 2 = Ternary)
		int x;
		int y;
		float temp;			//The current temperature of the module
		float thresh1;		//Crossing this temperature means that the module goes into a "warning" state
		float thresh2;		//Crossing this temperature means that the module goes into the "destroyed" state
		float watts;		//The assumption here is that the reactor puts out a total of "enough" volts, and we designed it with the idea that it can pull whatever current we want. Therefore, watts are really the only important thing.
		Status status;		// ^ Trust me, I know someone who knows electronics, and he sort of agreed with me
		Coolant coolant;
		bool oldWire;
		bool oldPowered;
		float oldTemp;
		Status oldStatus;	
		Coolant oldCoolant;	//These are the things sent over SQL
		BITMAP *sprite[3];		//The bitmap to be drawn for this
		
	public:
		//Constructors
		Module();
		Module(int a, SDL_Renderer* context);
		Module(std::string, std::string, int, float, SDL_Renderer* context);
		Module(std::string, std::string, int, int, float, SDL_Renderer* context);
		Module(std::string, std::string, int, int, float, int, SDL_Renderer* context);
		Module(std::string, std::string, int, int, float, int, int, SDL_Renderer* context);
		Module(std::string, std::string, int, int, float, int, int, int, SDL_Renderer* context);
		
		//Accessors
		std::string getName() { return name; }
		std::string getTruncName() { return truncName; }
		bool getWire() { return wire; }
		bool getOldWire() { return oldWire; }
		bool isPowered() { return powered; }
		bool getOldPowered() { return oldPowered; }
		int getID() { return id; }
		int getBusNum() { return busNum; }
		int getX() { return x; }
		int getY() { return y; }
		float getTemp() { return temp; }
		float getOldTemp() { return oldTemp; }
		float getWatts() { return watts; }
		float getThreshold(int a) { if(a) return thresh2; return thresh1; }
		Coolant getCoolant() { return coolant; }
		Coolant getOldCoolant() { return oldCoolant; }
		Status getStatus() { return status; }
		Status getOldStatus() { return oldStatus; }
		BITMAP *getSprite(int a) { return sprite[a]; }

		//Mutators
		float heat();
		void power();
		void toggleWire() { wire = !wire; }
		void setWire(bool a) { wire = a; }
		void setOldWire(bool a) { oldWire = a; } 
		void setTemp(float a) { temp = a; }
		void setOldTemp(float a) { oldTemp = a; }
		void setThresh1(float a) { thresh1 = a; }
		void setThresh2(float a) { thresh2 = a; }
		void setPowered(bool a) { powered = a; }
		void setOldPowered(bool a) { oldPowered = a; }
		void setName(std::string a) { name = a; }
		void setTruncName(std::string a) { truncName = a; }
		void setStatus(Status a) { status = a; }
		void setOldStatus(Status a) { oldStatus = a; }
};

#endif
