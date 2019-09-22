#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <algorithm>
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
string getFilename(char);
string binary(vector<char>, char);

int main(int argc, char const *argv[]) {
  ifstream input_file;
  ofstream output_file;

  pid_t pid;
  
  vector<char> input_txt;
  stack<char> char_stack;
  vector<pair<char, int>> frequencies;
  
  getString(input_txt);
  getChars(input_txt, char_stack);
  getFreq(frequencies, input_txt, char_stack);
  sortFreq(frequencies);
  printFreq(frequencies);

  for(int i = 0; i < frequencies.size(); i++) {
    if((pid = fork()) == 0) {
      for(int j = 0; j < input_txt.size(); j++) {
        for(int k = 0; k < i; k++) {
          input_txt.erase(remove(input_txt.begin(), input_txt.end(), frequencies[k].first), input_txt.end());
        }
      }

      outputString(output_file, input_txt, getFilename(frequencies[i].first), binary(input_txt, frequencies[i].first));

      _exit(0);
    }
  }

  for(int i = 0; i < frequencies.size(); i++)
    wait(0);

  string data_string;
  string data_binary;

  for(int i = 0; i < frequencies.size(); i++) {
    input_file.open(getFilename(frequencies[i].first));

    getline(input_file, data_string);
    getline(input_file, data_binary);

    input_file.close();

    if(i == 0)
      cout << "Original Message: " << data_string << endl;
    else
      cout << "Remaining Message: " << data_string << endl;

    if(frequencies[i].first == '\n')
      cout << "Symbol <EOL> code: " << data_binary << endl;
    else
      cout << "Symbol " << frequencies[i].first << " code: " << data_binary << endl;
  }

  return 0;
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

string binary(vector<char> txt, char c) {
  string binary;

  for(int i = 0; i < txt.size(); i++) {
    if(txt[i] == c)
      binary += '1';
    else
      binary += '0';
  }

  return binary;
}

void outputString(ofstream& ofile, vector<char> txt, string fn, string binary) {
  ofile.open(fn);

  for(int i = 0; i < txt.size(); i++) {
    if(txt[i] == '\n') {
      ofile << "<EOL>";
    } else {
      ofile << txt[i];
    }
  } ofile << endl;

  ofile << binary;

  ofile.close();
}

string getFilename(char c) {
  if(c == '\n') {
    string s = "EOL.txt";
    return s;
  } else {
    string s(1, c);
    s += ".txt";
    return s;
  }
}