// This is the .h file for Main.cpp

#ifndef MAIN_H_
#define MAIN_H_

#include <vector>
#include <array>
#include <fstream>

using namespace std;

extern vector<array<double,10>> storing;
extern vector<pair<int, double>> storingRanked;
extern ifstream fileReading;
extern long long hashTable[1000];
extern int numOperations;

#endif