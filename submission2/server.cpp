#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

using namespace std;

struct data_struct {
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int tid;
  int sockfd;
  int portno;
  char c;
  string msg;
};

void fireman(int) {
  while (waitpid(-1, NULL, WNOHANG) > 0)
    std::cout << "A child process ended" << std::endl;
}

int main(int argc, char const *argv[]) {
  int sockfd, newsockfd, portno, clilen;
  char message[256];
  char character[2];
  struct sockaddr_in serv_addr, cli_addr;
  struct data_struct data;
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

  signal(SIGCHLD, fireman);
  
  while(true) {
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
  
    if(newsockfd < 0) {
      cerr << "Error - Could not accept socket." << endl;
    }

    if(fork() == 0) {
      // Receive Message
      bzero(message, 255);
      n = recv(newsockfd, message, sizeof(message), 0);

      if (n < 0)
        cerr << "Error - Cannot read from socket." << endl;

      printf("%s\n", message);

      // Receive Character
      bzero(character, 2);
      n = recv(newsockfd, character, sizeof(character), 0);

      if (n < 0)
        cerr << "Error - Cannot read from socket." << endl;

      printf("%s\n", character);

      // BEGIN COMPRESSION
      for(int i = 0; i < strlen(message); i++) {
        if(message[i] == character[0]) {
          message[i] = '1';
        } else {
          message[i] = '0';
        }
      }

      printf("%s\n", message);

      // END COMPRESSION

      // Send Message Back
      n = send(newsockfd, "Received Message", 17, 0);

      if (n < 0)
        cerr << "Error - Cannot write to socket." << endl;

      sleep(1);
      _exit(0);
    }

    std::cout << "Press enter to continue" << std::endl;
    std::cin.get();
  }

  return 0;
}
