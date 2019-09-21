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

<<<<<<< HEAD
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
=======
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
    input_file.open(getFilename(frequencies[i].first) + ".txt");

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
>>>>>>> Refactored code.
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
  ofile.open(fn + ".txt");

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

<<<<<<< HEAD
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
=======
string getFilename(char c) {
  if(c == '\n') {
    string s = "EOL";
    return s;
  } else {
    string s(1, c);
    return s;
  }
>>>>>>> Refactored code.
}