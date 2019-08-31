#ifndef FREQUENCY_H_INCLUDED
#define FREQUENCY_H_INCLUDED

#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>

using namespace std;

int counter(string, char); // Counter Prototype
bool existsInStack(stack<char>, char); // Stack Checker
void findChars(stack<char>&, string); // Character Finder Prototype
void addToFreq(stack<char>, vector<pair<char, int>>&, string); // Frequency Array Loader
void sortFreqDescending(vector<pair<char, int>>&); // Sort Frequencies Accordingly

#endif // FREQUENCY_H_INCLUDED