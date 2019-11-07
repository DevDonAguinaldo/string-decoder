#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

mutex m;

void decompress(int i, string a, string b, char c) {
  m.lock();

  cout << "ID: " << i << endl;

  m.unlock();
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

  string file_contents = "";

  for(int i = NTHREADS - 1; i >= 0; --i) {
    tid[i] = thread(decompress, i, file_contents, input_vector[i].second, input_vector[i].first);
  }

  for(int i = NTHREADS - 1; i >= 0; --i) {
    tid[i].join();
  }

  return 0;
}
