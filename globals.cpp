#define WIN32_LEAN_AND_MEAN		//Ok, so I don't know what this does, but it doesn't work without it. Leave it there.
#include <iostream>				//Standard I/O
#include <string>				//C++ std::string manipulations
#include <cstring>				//C char*/char[] manipulations
#include <sstream>				//std::string stream for splitting strings into vectors
#include <vector>				//Vectors to parse the message received
#include <Windows.h>			//Windows.h is nessecary for winsock2.h and ws2tcpip.h
#include <WinSock2.h>			//Allows us to connect to the sockets
#include <WS2tcpip.h>			//Gives us access to most winsock2 functions

#define PORT "4950"				//The port data is sent/received over
#define MAXBUFLEN 511			//The maximum message size
#define SERVER_COMPUTER "GLaDOS"
#define ENG_HAB_COMPUTER "HAL-9000"
#pragma comment(lib, "ws2_32.lib")		//Links to ws2_32.lib, which is nessecary for winsock2
