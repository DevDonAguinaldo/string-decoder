#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <ctype.h>

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }

  int sockfd, newsockfd, portno, n;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) 
    error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));

  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  listen(sockfd, 5);

  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  if (newsockfd < 0)
    error("ERROR on accept");

  FILE *fp;

  int ch = 0;
  fp = fopen("input_rec.txt", "w");
  int words;

  read(newsockfd, &words, sizeof(int));

  printf("Passed integer: %d\n", words);

  while(ch != words) {
    read(newsockfd, buffer, 255);
    fprintf(fp, "%s", buffer);
    printf("%s %d\n", buffer, ch);
    ch++;
  }

  printf("Received successfully! Saved as: input_rec.txt\n");

  close(newsockfd);
  close(sockfd);

  return 0; 
}