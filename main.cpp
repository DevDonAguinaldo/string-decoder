#include <iostream>
#include "frequency.h"
#include "frequency.cpp"
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <algorithm>
#include <sys/wait.h>

using namespace std;

int main(int argc, char const *argv[]) {
   FILE * file;
   string line; cin >> line; // input string
   pid_t pid; // process IDs
   pid = fork(); // initialized the forked processes

   stack<char> myStack; // stack of characters to look for
   vector<pair<char, int>> arrOfFreq; // associative vector of frequencies

   // TODO: find a way to pass information from parent to child and vice-versa.
   // ? Use output text files?
   // ? How does child/parent know what text file?
   if(pid != 0) {
      // parent process 1
      findChars(myStack, line);
      addToFreq(myStack, arrOfFreq, line);
      sortFreqDescending(arrOfFreq);

      file = fopen("output.txt", "w");
      for(auto key : arrOfFreq)
         fprintf(file, "%c frequency = %d\n", key.first, key.second);
      fclose(file);
   } else {
      // child process 1
      wait(NULL);
      
      pid = fork(); // initializes another parent and child fork
      if(pid != 0) {
         // parent process 2
         file = fopen("output.txt", "a");
         fprintf(file, "PARENT PROCESS READING...\n");
         fclose(file);
      } else {
         // child process 2
         wait(NULL);
         file = fopen("output.txt", "a");
         fprintf(file, "CHILD PROCESS READING...\n");
         fclose(file);
      }
   }

   return 0;
}

