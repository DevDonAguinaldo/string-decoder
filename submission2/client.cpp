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
#include <pthread.h>
#include <sys/wait.h>

using namespace std;

void getString(vector<char>&);
void getChars(vector<char>, stack<char>&);
int count(vector<char>, char);
void getFreq(vector<pair<char,int>>&, vector<char>, stack<char>);
bool checkStack(stack<char>, char);
void sortFreq(vector<pair<char, int>>&);
void printString(vector<char>);
void printFreq(vector<pair<char,int>>);
void outputString(ofstream&, vector<char>, string, string);
void *connectToServer(void*);

int main(int argc, char *argv[]) {
  int sockfd, portno, n, num_of_threads;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  vector<char> input_txt;
  stack<char> char_stack; 
  vector<pair<char, int>> frequencies;
  
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  getString(input_txt);
  getChars(input_txt, char_stack);
  getFreq(frequencies, input_txt, char_stack);
  sortFreq(frequencies);
  cout << "Sending these to server: \n";
  printFreq(frequencies);

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  num_of_threads = frequencies.size();
  pthread_t tid[num_of_threads];

  if (sockfd < 0) 
    cerr << "Error - Opening socket." << endl;
  
  server = gethostbyname(argv[1]);
  
  if (server == NULL) {
    cerr << "Error - IP/Port does not exist." << endl;
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  
  // printf("Please enter the message: ");

  for(int i = 0; i < num_of_threads; i++) {
    if(pthread_create(&tid[i], NULL, connectToServer, &frequencies[i].first)) {
      cerr << "Error - Creating thread." << endl;
      return 1;
    }
    
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
      cerr << "Error - Connecting to server." << endl;

    bzero(buffer, 256);
    buffer[0] = frequencies[i].first;
    n = write(sockfd, buffer, strlen(buffer));
    
    if (n < 0)
      cerr << "Error - Writing to socket." << endl;
    
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    
    if (n < 0)
      cerr << "Error - Reading from socket." << endl;

    printf("%s\n", buffer);
  }

  for(int i = 0; i < num_of_threads; i++) {
    pthread_join(tid[i], NULL);
  }
  
  return 0;
}

bool checkStack(stack<char> stk, char c) {
  while(!stk.empty()) {
    if(stk.top() == c)
      return true;
    else if(stk.empty())
      return false;
    else
      stk.pop();
  }
}

void getString(vector<char>& txt) {
  char c;

  while(cin.get(c))
    txt.push_back(c);
}

void getChars(vector<char> txt, stack<char>& stk) {
  for(int i = 0; i < txt.size(); i++) {
    if(!checkStack(stk, txt[i]))
      stk.push(txt[i]);
    else
      continue;
  }
}

int count(vector<char> txt, char c) {
  int counter = 0;

  for(int i = 0; i < txt.size(); i++) {
    if(txt[i] == c)
      counter++;
    else if(c == '\n' && txt[i] == '<')
      counter++;
  }

  return counter;
}

void getFreq(vector<pair<char,int>>& freq, vector<char> txt, stack<char> stk) {
  while(!stk.empty()) {
    freq.push_back(pair<char,int> (stk.top(), count(txt, stk.top())));
    stk.pop();
  }
}

void sortFreq(vector<pair<char, int>>& freq) {
  sort(freq.begin(), freq.end(), 
    [](pair<char, int> elem1, pair<char, int> elem2) {
        if(elem1.second == elem2.second) {
          return elem1.first < elem2.first;
        } else {
          return elem1.second > elem2.second;
        }
    });
}

void printString(vector<char> txt) {
  for(int i = 0; i < txt.size(); i++) {
      if(txt[i] == '\n') {
        cout << "<EOL>";
      } else {
        cout << txt[i];
      }
    } cout << endl;
}

void printFreq(vector<pair<char,int>> freq) {
  for(int i = 0; i < freq.size(); i++) {
    if(freq[i].first == '\n')
      cout << "<EOL> frequency = " << freq[i].second << endl;
    else if(freq[i].first == ' ')
      cout << "frequency = " << freq[i].second << endl;
    else 
      cout << freq[i].first << " frequency = " << freq[i].second << endl;
  }
}

void *connectToServer(void *ptr) {
  
  

  return NULL;
}