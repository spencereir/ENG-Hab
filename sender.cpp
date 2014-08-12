#include "globals.cpp"

int sender(const char *message, SOCKET sockfd, addrinfo *p) {
  int numbytes;
  if ((numbytes = sendto(sockfd, message, strlen(message), 0, p->ai_addr, p->ai_addrlen)) == -1) {
        std::cout << "Error: Message not sent." << std::endl;
  }
  return 0;
}
