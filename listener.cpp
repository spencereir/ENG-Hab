/*		Listener.cpp
	This file deals with
	all input the server
	will receive. It then
	will send whatever it
	gets to the parser,
	which breaks up the
	data into blocks, and
	sends them to the
	appropriate computers.
*/

#include "globals.cpp"		//Set up all includes/defines/linkers

void *get_in_addr(struct sockaddr *sa) {				//This function will get the internet adress of any socket adress
  if (sa->sa_family == AF_INET) {                       
      return &(((struct sockaddr_in*)sa)->sin_addr);    //Execute this only if socket adress is determined to be IPv4
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);      //Execute this only if socket adress is determined to be IPv6
}

std::string listener() {								//This function will return an std::string, the message sent.
  int sockfd;											//SockFD will be the socket that we connect to
  struct addrinfo hints, *servinfo, *p;					//Hints is a struct that contains parameters about how we connect, servinfo and p hold the actual data
  int rv;												//Used to check if port connections were successful.
  int numbytes;											//The number of bytes contained in the received message. NOTE: The recvfrom() function returns the number of bytes recieved. Since this server software uses UDP as opposed to TCP, there is a slight chance that some data may be lost. This will rarely if ever be an issue. If, however, you decide to start sending copious amounts of data, you can either choose to have a callback that checks if every byte of data was received, or overhaul the listener and sender into TCP.
  struct sockaddr_storage their_addr;					//Stores the address we are connecting to.
  char buf[MAXBUFLEN];
  socklen_t addr_len;
 
  memset(&hints, 0, sizeof hints);						//Allocate memory to hints
  hints.ai_family = AF_UNSPEC;							//Set the protocol to be unspecified (It can be either IPv4 or IPv6 this way)
  hints.ai_socktype = SOCK_DGRAM;                       //Connect as a UDP socket, rather than TCP
  hints.ai_flags = AI_PASSIVE;							//Let Windows handle the IP.
 
  if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {		//Attempt to connect to the port with the specified parameters
    std::cout << "Error connecting to port " << PORT << std::endl; 
  }
  for(p = servinfo; p != NULL; p = p->ai_next) {						//Move through all sockets on the port
      if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {    //Try to connect to the socket
          continue;
      }
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {				//Try to bind to the socket
          shutdown(sockfd, 2);											//If we cannot, close it
          continue;
      }
      break;
  }
  if (p == NULL) {
      std::cout << "Failed to bind to socket." << std::endl;
	  system("pause");
  }
  freeaddrinfo(servinfo);
  addr_len = sizeof their_addr;
  if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {	//Retreive the message, throw an error if it is shorter than expected
      std::cout << "Error receiving message." << std::endl;
  }
  if(numbytes > 0) buf[numbytes] = '\0';	//Add the terminating null character
  shutdown(sockfd, 2);						//Shutdown the socket
  std::string str;
  str.assign(buf, strlen(buf));				//Convert buf to an std::string
  return str;								//Return that string
}
 
