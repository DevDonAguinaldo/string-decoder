#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <sys/wait.h>
#include <map>

using namespace std;

void getString(vector<char>&);
void getChars(vector<char>, stack<char>&);
int count(vector<char>, char);
void getFreq(vector<pair<char,int>>&, vector<char>, stack<char>);
bool checkStack(stack<char>, char);
void sortFreq(vector<pair<char, int>>&);
void printFreq(vector<pair<char,int>>);
void printString(vector<char>);

struct char_data {
  string leftover;
  string binary;
};

int main() {
  vector<char> input_txt;
  stack<char> char_stack;
  vector<pair<char, int>> frequencies;

  getString(input_txt);
  getChars(input_txt, char_stack);
  getFreq(frequencies, input_txt, char_stack);
  sortFreq(frequencies);
  printFreq(frequencies);

  const int THREAD_COUNT = frequencies.size();

  pthread_t tid[THREAD_COUNT];

  for(int i = 0; i < THREAD_COUNT; i++) {
    printString(input_txt);

  }

  for(int i = 0; i < THREAD_COUNT; i++)
    pthread_join(tid[i], NULL);


  return 0;
}

void getString(vector<char>& txt) {
  char c;

  while(cin.get(c))
    txt.push_back(c);
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

void printString(vector<char> txt) {
  for(int i = 0; i < txt.size(); i++) {
      if(txt[i] == '\n') {
        cout << "<EOL>";
      } else {
        cout << txt[i];
      }
    } cout << endl;
}