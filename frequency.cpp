#include "frequency.h"

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
      if(existsInStack(stk, line[i]) == false) {
         stk.push(line[i]);
      } else // if does then continue
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
         return elem1.second > elem2.second;
      });
} // end function
