#include "globals.cpp"

int sender(const char *message, PCSTR target, PCSTR portNum)
{
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(target, portNum, &hints, &servinfo)) != 0) {
    std::cout << "Error: Could not connect to target " << target << " on port " << portNum << "." << std::endl;
  }
  //std::cout << servinfo << std::endl;
  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      std::cout << "Error: Could not intialize socket" << std::endl;
      continue;
    }
    break;
  }

  if (p == NULL) {
        std::cout << "Error: Could not bind to socket" << std::endl;
  }													
  if ((numbytes = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen)) == -1) {
        std::cout << "Error: Message not sent." << std::endl;
  }
  if(message == "quit") exit(1);
  std::cout << std::endl;
  freeaddrinfo(servinfo);
  shutdown(sockfd,2);
  return 0;
}
