#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <condition_variable>

using namespace std;

mutex m;
condition_variable cv;
string file_contents = "";
static int neededID;

void decompress(int id, string b, char c) {
  unique_lock<mutex> ul(m);
  
  while(id != neededID) { cv.wait(ul); }

  if(c == '\n')
    cout << "<EOL> Binary code = " << b << endl;
  else if(c == ' ')
    cout << "Binary code = " << b << endl;
  else 
    cout << c << " Binary code = " << b << endl;

  int j = 0;
  string placeholder = "";

  for(int i = 0; i < b.length(); i++) {
    if(b[i] == '1') {
      placeholder += c;
    } else if(b[i] == '0' && file_contents.length() != 0) {
      placeholder += file_contents[j];
      ++j;
    } else continue;
  }

  file_contents = placeholder;

  neededID--;

  cv.notify_all();
}

int main(int argc, char const *argv[]) {
  vector<pair<char, string>> input_vector;
  string input;

  while(getline(cin, input)) {
    char c;
    string binary;

    // split input
    if(input[0] == '<') {
      c = '\n';
      binary = input.substr(input.find(" ") + 1, input.length());
    } else {
      c = input[0];
      binary = input.substr(2, input.length());
    }
    // store as key-value pairs in vector
    input_vector.push_back(pair<char,string>(c, binary));
  }

  int NTHREADS = input_vector.size();
  thread tid[NTHREADS];
  neededID = NTHREADS - 1;

  for(int i = NTHREADS - 1; i > -1; --i) {
    tid[i] = thread(decompress, i, input_vector[i].second, input_vector[i].first);
  }

  for(int i = NTHREADS - 1; i > -1; --i) {
    tid[i].join();
  }

  cout << "Decompressed file contents:\n" << file_contents << endl;

  return 0;
}
