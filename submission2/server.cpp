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
  char binary[256];
  char character[2];
  char newMessage[256];
  char charToAppend[2];
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
      bzero(message, 256);
      n = recv(newsockfd, message, sizeof(message), 0);

      if (n < 0)
        cerr << "Error - Cannot read from socket." << endl;

      // printf("Received Message: \t\t[%s]\n", message);

      // Receive Character
      bzero(character, 2);
      n = recv(newsockfd, character, sizeof(character), 0);

      if (n < 0)
        cerr << "Error - Cannot read from socket." << endl;

      // if(character[0] == '\n') {
      //   printf("Removing Character: \t\t[<EOL>]\n");
      // } else {
      //   printf("Removing Character: \t\t[%s]\n", character);
      // }

      // *BEGIN
      for(int i = 0; i < strlen(message); i++) {
        if(message[i] == character[0]) {
          binary[i] = '1';
        } else {
          binary[i] = '0';
        }
      }

      n = send(newsockfd, binary, sizeof(binary), 0);

      if (n < 0)
        cerr << "Error - Cannot write to socket." << endl;

      for(int i = 0; i < strlen(message); i++) {
        if(message[i] != character[0]) {
          charToAppend[0] = message[i];
          charToAppend[1] = '\0';
          strcat(newMessage, charToAppend);
        }
      }

      n = send(newsockfd, newMessage, sizeof(newMessage), 0);

      if (n < 0)
        cerr << "Error - Cannot write to socket." << endl;

      // *END

      // printf("Sending: [%s]\n", newMessage);
      // printf("Sending: [%s]\n", binary);

      sleep(1);
      _exit(0);
    }
  }

  return 0;
}
