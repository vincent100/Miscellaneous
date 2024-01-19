// This is the .h file for Helper.cpp

#ifndef GETLINE_H_
#define GETLINE_H_

#include <string>
#include <vector>

using namespace std;

double StrToDouble(string strOn);
double AnalyzeLine();

vector<pair<int, double>> ImperfectionFactor(vector<array<double, 10>> vectorArray);
bool ComparePair(pair<int, double> pair1, pair<int, double> pair2);

int HashingFunctionChange(char char1, char char2, char char3, bool insert);
pair<int, long long> ChangeDistribution(int lengthNumber, int where1, int where2, int where3);

int HashingFunctionSimple(long long numHash, bool insert);

int SecondHash(long long numHash, bool insert);
int ProbingFunction(int hash1, int hash2, long long original, bool insert);

int SearchTable(string whatSearch, bool whatHash);

#endif