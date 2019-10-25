#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stack>
#include <map>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[]) {

  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc < 2) {
    cerr << "Error - No port provided." << endl;
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  if (sockfd < 0)
    cerr << "Error - Cannot open socket." << endl;
  
  bzero((char *) &serv_addr, sizeof(serv_addr));
  
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    cerr << "Error - Cannot bind." << endl;

  listen(sockfd, 5);
  
  clilen = sizeof(cli_addr);
  
  while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen)) > 0) {
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);

    if (n < 0)
      cerr << "Error - Cannot read from socket." << endl;

    printf("Here is the message: %s\n", buffer);
    n = write(newsockfd, "I got your message", 18);

    if (n < 0)
      cerr << "Error - Cannot write to socket." << endl;

    if (newsockfd < 0) 
      cerr << "Error - Cannot accept socket." << endl;
  }

  return 0;
}
