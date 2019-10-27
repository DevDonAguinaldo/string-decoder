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
#include <thread>
#include <pthread.h>
#include <sys/wait.h>

using namespace std;

void *connection_handler(void *args);
bool checkStack(stack<char>, char);
void getCharacters(stack<char>&, string);

struct data_struct {
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int tid;
  int sockfd;
  int portno;
  char c;
  string msg;
};

int main(int argc, char *argv[]) {
  int NTHREADS;
  struct data_struct data;
  stack<char> characters;

  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  data.portno = atoi(argv[2]);
  data.server = gethostbyname(argv[1]);
  
  getline(cin, data.msg);

  // Find Frequencies
  getCharacters(characters, data.msg);

  if (data.server == NULL) {
    cerr << "Error - IP/Port does not exist." << endl;
    exit(0);
  }
  
  NTHREADS = characters.size();
  pthread_t tid[NTHREADS];

  bzero((char *) &data.serv_addr, sizeof(data.serv_addr));
  data.serv_addr.sin_family = AF_INET;
  bcopy((char *)data.server->h_addr, (char *) &data.serv_addr.sin_addr.s_addr, data.server->h_length);
  data.serv_addr.sin_port = htons(data.portno);

  for(int i = 0; i < NTHREADS; i++) {
    data.c = characters.top();
    if(pthread_create(&tid[i], NULL, connection_handler, (void*) &data) > 0) {
      cerr << "Error - Creating thread." << endl;
      return 1;
    }
    sleep(1);
    characters.pop();
  }

  for(int i = 0; i < NTHREADS; i++)
    pthread_join(tid[i], NULL);

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

  bzero(buffer, 256);
  strcpy(buffer, data->msg.c_str());

  n = send(data->sockfd, buffer, sizeof(buffer), 0);

  if(n < 0) {
    cerr << "Error - Sending message to server." << endl;
  }
  
  bzero(buffer, 256);
  buffer[0] = data->c;

  n = send(data->sockfd, buffer, sizeof(buffer), 0);

  if(n < 0) {
    cerr << "Error - Sending message to server." << endl;
  }

  n = recv(data->sockfd, buffer, 20, 0);

  if(n < 0) {
    cerr << "Error - Receiving data from server." << endl;
  }

  printf("%s\n", buffer);

  close(data->sockfd);

  return NULL;
}

bool checkStack(stack<char> stk, char c) {
  while(!stk.empty()) {
    if(stk.top() == c)
      return true;
    else
      stk.pop();

    if(stk.empty())
      return false;
  }
}

void getCharacters(stack<char> &stk, string msg) {
  for(int i = 0; i < msg.length(); i++) {
    if(checkStack(stk, msg[i]) == false) {
      stk.push(msg[i]);
    } else {
      continue;
    }
  }
}