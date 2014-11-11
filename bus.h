#ifndef BUS_H_
	#define BUS_H_

	#include "main.h"

	class Bus {
		private:
			bool powered;   
			int busNum;
			int source;
			float watts;
		
		public:
			//Constructors
			Bus();
			Bus(int);

			//Accessors
			bool isPowered() { return powered; }
			int getBusNum() { return busNum; }
			int getSource() { return source; }
			float getWatts() { return watts; }

			//Mutators
			void setBusNum(int a) { busNum = a; }
			void setSource(int a) { source = a; }
			void setWatts(float a) { watts = a; }
			void power();    		
	};
#endif
