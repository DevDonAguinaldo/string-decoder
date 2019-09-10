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

int counter(string, char); // Counter Prototype
bool existsInStack(stack<char>, char); // Stack Checker
void findChars(stack<char>&, string); // Character Finder Prototype
void addToFreq(stack<char>, vector<pair<char, int>>&, string); // Frequency Array Loader
void sortFreqDescending(vector<pair<char, int>>&); // Sort Frequencies Accordingly
void deStack(stack<char>&); // Deconstructs Stack

int main(int argc, char const *argv[]) {
   time_t start, end;
   time(&start); ios_base::sync_with_stdio(false);
   string line;
   getline(cin, line);
   ifstream input_file;
   ofstream output_file;
   pid_t pid;

   stack<char> myStack, charStack; // stack of characters to look for
   vector<pair<char, int>> arrOfFreq; // associative vector of frequencies

   findChars(myStack, line);
   addToFreq(myStack, arrOfFreq, line);
   sortFreqDescending(arrOfFreq);

   // *Child Processes
   // *Must exit so it does not create forks in child processes.
   for(int i = 0; i < arrOfFreq.size(); i++) {
      if((pid = fork()) == 0) {
         string filename(1, arrOfFreq[i].first);
         string binary;

         // removes the characters
         for(int n = 0; n < line.length(); n++)
            for(int k = 0; k < i; k++)
               if(line[n] == arrOfFreq[k].first)
                  line[n] = ' ';

         line.erase(remove(line.begin(), line.end(), ' '), line.end());

         // create binary code
         for(int j = 0; j < line.length(); j++)
            if(line[j] == arrOfFreq[i].first) binary += "1";
            else binary += "0";

         // output to designated text file
         output_file.open(filename + ".txt");
         output_file << line << endl;
         output_file << binary << endl;
         output_file.close();

         _exit(0);
      }
   }

   // *Parent Processes
   // *Must wait for each child process to finish.
   for(int j = 0; j < arrOfFreq.size(); j++)
      printf("%c frequency %d\n", arrOfFreq[j].first, arrOfFreq[j].second);

   for(int i = 0; i < arrOfFreq.size(); i++) {
      wait(0); // waits for child processes to finish

      string data_string, data_binary;
      string filename(1, arrOfFreq[i].first);

      // opens the appropriate files
      input_file.open(filename + ".txt");
      getline(input_file, data_string);
      getline(input_file, data_binary);
      input_file.close();

      // output data received
      if(i == 0) cout << "Original Message:\t" << data_string << endl;
      else cout << "Remaining Message:\t" << data_string << endl;
      cout << "Symbol " << arrOfFreq[i].first << " code:\t\t" << data_binary << endl;
   }

   // deconstruct
   deStack(myStack);
   deStack(charStack);
   arrOfFreq.clear();

   time(&end);

   double exec_time = double(end - start);
   cout << "\nExecution Time: " << fixed << exec_time << 's' << setprecision(5) << endl;

   return 0;
} // end main

/* 
* Counter Function
* -----------------
* Parameters:
* Input String
* Character to Find
* -----------------
* Returns: The Frequency of Character
*/
int counter(string line, char findMe) {
   int count = 0; // temp variable to store count of character
   
   for(int i = 0; i < line.length(); i++) {
      if(line[i] == findMe)
         count++;
   } // end loop

   return count;
} // end function

/*
* Character Finder Function
* -------------------------
* Parameters:
* Character Stack
* Input String
* -------------------------
* Returns: Stack of Characters
*/
void findChars(stack<char>& stk, string line) {
   // loop through string to find characters
   for(int i = 0; i < line.length(); i++) {
      // if doesn't exist then push
      if(existsInStack(stk, line[i]) == false)
         stk.push(line[i]);
      else // if does then continue
         continue;
   } // end loop
} // end function

/*
* Stack Checker Function
* ----------------------
* Parameters:
* Character Stack
* Character to Find
* ----------------------
* Returns: Boolean
*/
bool existsInStack(stack<char> stk, char findMe) {
   while(!stk.empty()) {
      if(stk.top() == findMe)
         return true;
      else
         stk.pop();
   } // end loop

   if(stk.empty())
      return false;
   else
      return true;
} // end function

/*
* Add To Frequency Array Function
* -------------------------------
* Parameters:
* Stack of Characters
* Associative Array of Frequecies
* -------------------------------
* Returns: Array of Frequencies
*/
void addToFreq(stack<char> stk, vector<pair<char, int>>& freq, string line) {
   while(!stk.empty()) {
      // count the frequencies and push to array
      freq.push_back(pair<char, int> (stk.top(), counter(line, stk.top())));
      stk.pop();
   }
}

/*
* Sort Frequency Array Function
* -------------------------------
* Parameters:
* Associative Vector of Frequencies
* -------------------------------
* Returns: Array of Frequencies in Descending Order
*/
void sortFreqDescending(vector<pair<char, int>>& freq) {
   sort(freq.begin(), freq.end(), 
      [](pair<char, int> elem1, pair<char, int> elem2) {
         if(elem1.second == elem2.second) {
            return elem1.first < elem2.first;
         } else {
            return elem1.second > elem2.second;
         }
      });
} // end function

/*
* Deconstruct Stack
*/
void deStack(stack<char>& stk) {
   while(!stk.empty())
      stk.pop();
}