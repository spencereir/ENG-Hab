#include "globals.cpp"

int sender(const char *message)
{
  int numbytes;
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(SERVER_COMPUTER, PORT, &hints, &servinfo) != 0) {
    std::cout << "Error: Could not connect to target " << SERVER_COMPUTER << " on port " << PORT << "." << std::endl;
  }
  assert(servinfo != NULL);
  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      std::cout << "Error: Could not intialize socket" << std::endl;
      continue;
    }
    break;
  }
  assert(p != NULL);								
  if ((numbytes = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen)) == -1) {
        std::cout << "Error: Message not sent." << std::endl;
  }
  freeaddrinfo(servinfo);
  closesocket(sockfd);
  return 0;
}
