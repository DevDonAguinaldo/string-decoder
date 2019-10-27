#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stack>
#include <map>
#include <vector>
#include <algorithm>
#include <thread>
#include <pthread.h>
#include <sys/wait.h>

using namespace std;

void *connection_handler(void *args);

// !Segmentation Fault Error
// !Coversion from string to char array
// !Needs a way to pass string and char to server

struct data_struct {
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int tid;
  int sockfd;
  int portno;
  char c;
  char* msg;
};

int main(int argc, char *argv[]) {
  int NTHREADS;
  string input;
  struct data_struct data;

  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  data.portno = atoi(argv[2]);
  data.server = gethostbyname(argv[1]);
  
  // get string from input
  getline(cin, input);

  strcpy(data.msg, input.c_str());

  printf("%s\n", data.msg);

  if (data.server == NULL) {
    cerr << "Error - IP/Port does not exist." << endl;
    exit(0);
  }
  
  NTHREADS = 5;
  pthread_t tid[NTHREADS];
  char arr[NTHREADS] = {'a', 'b', 'c', 'd'};

  bzero((char *) &data.serv_addr, sizeof(data.serv_addr));
  data.serv_addr.sin_family = AF_INET;
  bcopy((char *)data.server->h_addr, (char *) &data.serv_addr.sin_addr.s_addr, data.server->h_length);
  data.serv_addr.sin_port = htons(data.portno);

  for(int i = 0; i < NTHREADS; i++) {
    data.c = arr[i];
    if(pthread_create(&tid[i], NULL, connection_handler, (void*) &data) > 0) {
      cerr << "Error - Creating thread." << endl;
      return 1;
    }
    sleep(1);
  }

  for(int i = 0; i < NTHREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  return 0;
}

void *connection_handler(void *args) {
  struct data_struct *data = (struct data_struct *) args;
  char buffer[256];
  int n;

  data->sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (data->sockfd < 0) 
    cerr << "Error - Opening socket." << endl;

  if (connect(data->sockfd, (struct sockaddr *) &data->serv_addr, sizeof(data->serv_addr)) < 0)
    cerr << "Error - Connecting to server." << endl;

  // TODO: send data of msg and char to server
  n = send(data->sockfd, data->msg, sizeof(data->msg), 0);

  bzero(buffer, 256);
  buffer[0] = '-';
  n = send(data->sockfd, buffer, 255, 0);

  bzero(buffer, 256);
  buffer[0] = data->c;
  n = send(data->sockfd, buffer, 255, 0);

  if(n < 0) {
    cerr << "Error - Sending message to server." << endl;
  }

  // TODO: read msg from server and continue sending data
  
  n = recv(data->sockfd, buffer, 20, 0);

  if(n < 0) {
    cerr << "Error - Receiving data from server." << endl;
  }

  printf("%s\n", buffer);

  close(data->sockfd);

  return NULL;
}