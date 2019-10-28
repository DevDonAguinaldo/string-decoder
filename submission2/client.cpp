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
bool checkStack(stack<char>, char);
void getCharacters(stack<char>&, string);
void countFrequencies(string, stack<char>, map<char, int>&);
void getFrequencies(map<char, int>&, vector<pair<char, int>>&, map<char, int>::iterator);
void sortFrequencies(vector<pair<char, int>>&);
void printFrequencies(vector<pair<char, int>>);
void freqToStack(stack<char>&, vector<pair<char, int>>);
void checkEOL(string&);

struct data_struct {
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int tid, nthreads, sockfd, portno;
  char c;
  string msg, binary;
};

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  struct data_struct data;
  stack<char> charsUnsorted, charsSorted;
  map<char, int> freqMap;
  map<char, int>::iterator itr;
  vector<pair<char, int>> freqVector;

  data.server = gethostbyname(argv[1]);
  data.portno = atoi(argv[2]);
  
  char c;
  while(cin.get(c)) {
    data.msg += c;
  }

  getCharacters(charsUnsorted, data.msg);
  countFrequencies(data.msg, charsUnsorted, freqMap);
  getFrequencies(freqMap, freqVector, itr);
  freqToStack(charsSorted, freqVector);
  printFrequencies(freqVector);
  data.nthreads = charsSorted.size();
  pthread_t tid[data.nthreads];

  if (data.server == NULL) {
    cerr << "Error - IP/Port does not exist." << endl;
    exit(0);
  }

  bzero((char *) &data.serv_addr, sizeof(data.serv_addr));
  data.serv_addr.sin_family = AF_INET;
  bcopy((char *)data.server->h_addr, (char *) &data.serv_addr.sin_addr.s_addr, data.server->h_length);
  data.serv_addr.sin_port = htons(data.portno);

  for(int i = 0; i < data.nthreads; i++) {
    data.c = charsSorted.top();
    data.tid = i;
    
    if(pthread_create(&tid[i], NULL, connection_handler, (void*) &data) > 0) {
      cerr << "Error - Creating thread." << endl;
      return 1;
    }

    if(i == 0) {
      cout << "Original Message: "; checkEOL(data.msg);
    } else {
      cout << "Remaining Message: "; checkEOL(data.msg);
    }

    sleep(1);

    if(data.c == '\n') {
      cout << "Symbol <EOL> code: " << data.binary << endl;
    } else if(data.c == ' ') {
      cout << "Symbol code: " << data.binary << endl;
    } else {
      cout << "Symbol " << data.c << " code: " << data.binary << endl;
    }

    charsSorted.pop();
  }

  for(int i = 0; i < data.nthreads; i++)
    pthread_join(tid[i], NULL);

  return 0;
}

void *connection_handler(void *args) {
  struct data_struct *data = (struct data_struct *) args;
  char buffer[256];
  char binary[256];
  int n;

  data->sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (data->sockfd < 0) 
    cerr << "Error - Opening socket." << endl;

  if (connect(data->sockfd, (struct sockaddr *) &data->serv_addr, sizeof(data->serv_addr)) < 0)
    cerr << "Error - Connecting to server." << endl;

  // *SENDING INPUT TEXT
  bzero(buffer, 256);
  strcpy(buffer, data->msg.c_str());

  n = send(data->sockfd, buffer, sizeof(buffer), 0);

  if(n < 0) {
    cerr << "Error - Sending message to server." << endl;
  }
  // *END SENDING INPUT TEXT

  // *SENDING CHARACTER
  bzero(buffer, 256);
  buffer[0] = data->c;

  n = send(data->sockfd, buffer, sizeof(buffer), 0);

  if(n < 0) {
    cerr << "Error - Sending message to server." << endl;
  }
  // *END SENDING CHARACTER

  // *RECEIVING BINARY
  bzero(binary, 256);
  n = recv(data->sockfd, binary, sizeof(binary), 0);

  if(n < 0) {
    cerr << "Error - Receiving data from server." << endl;
  }

  data->binary = binary;
  // *END RECEIVING BINARY

  // *RECEIVING NEW MESSAGE
  bzero(buffer, 256);
  n = recv(data->sockfd, buffer, sizeof(buffer), 0);

  if(n < 0) {
    cerr << "Error - Receiving data from server." << endl;
  }

  data->msg = buffer;
  // *END RECEIVING NEW MESSAGE

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

void countFrequencies(string msg, stack<char> stk, map<char, int>& freq) {
  int counter = 0;
  while(!stk.empty()) {
    counter = 0;
    for(int i = 0; i < msg.length(); i++) {
      if(stk.top() == msg[i]) {
        counter++;
      }
    }

    freq.insert(pair<char, int>(stk.top(), counter));
    stk.pop();
  }
}

void sortFrequencies(vector<pair<char, int>>& freq) {
  sort(freq.begin(), freq.end(), 
    [](pair<char, int> elem1, pair<char, int> elem2) {
        if(elem1.second == elem2.second) {
          return elem1.first < elem2.first;
        } else {
          return elem1.second > elem2.second;
        }
    });
}

void getFrequencies(map<char, int> &freq, vector<pair<char, int>>&freqV, map<char, int>::iterator itr) {
  for(itr = freq.begin(); itr != freq.end(); itr++)
    freqV.push_back(pair<char, int> (itr->first, itr->second));

  sortFrequencies(freqV);
}

void printFrequencies(vector<pair<char, int>> freqV) {
  for(int i = 0; i < freqV.size(); i++) {
    if(freqV[i].first == '\n') {
      cout << "<EOL> frequency = " << freqV[i].second << endl;
    } else if(freqV[i].first == ' ') {
      cout << " frequency = " << freqV[i].second << endl;
    } else {
      cout << freqV[i].first << " frequency = " << freqV[i].second << endl;
    }
  }
}

void freqToStack(stack<char> &stk, vector<pair<char, int>> freqV) {
  for(int i = freqV.size() - 1; i >= 0; i--) {
    stk.push(freqV[i].first);
  }
}

void checkEOL(string &str) {
  for(int i = 0; i < str.length(); i++) {
    if(str[i] == '\n') {
      cout << "<EOL>";
    } else {
      cout << str[i];
    }
  } cout << endl;
}