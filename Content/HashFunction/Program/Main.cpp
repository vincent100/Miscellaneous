/**
 * MIT PRIMES PROBLEM G3 COMPUTER SCIENCE
 * 
 * This project was written in C++ 20, and developped using Visual Studio Code on Windows 11
 */
 

// Beginning of Header 

#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <string>
#include <filesystem>

#include "Helper.h"

// End of Header

using namespace std;
using pairInt = pair<int, long long>;

vector<array<double, 10>> storing; // The vector stores the info given on the digits.
vector<pair<int, double>> storingRanked; // Stores the index and "imperfection factor" of each row of storing.
ifstream fileReading; // File to read from.
long long hashTable[1009]; // Hash table
int numOperations; // Number of operations

signed main()
{
    // Variable initialization:

    int numDigits; // The number of digits.
    double total; // The total for one input line.
    bool whatInsert; // What kind of hashing function is used.
    string fileName; // Name of file.
    filesystem::path filePath; // 
    int numItems; // Number of elements in file.
    pairInt numEnter; // Number to add to hash table.
    bool continueing = true; // Boolean tracking whether or not to continue reading a new file in main while loop.
    int printTable; // Whether to print the table or not
    string numSearch; // Number to search for in the Hash Table.
    long long answerSearch; // Answer to the search in the Hash Table.
    double imperfectionNormal, imperfectionChange; // The imperfection factors of both possibilities
    
    
    // End of variable initialization

    cout << "Enter the number of digits:\n";
    cin >> numDigits;
    cout << "Enter the distribution for these N digits:\n";

    // For every line of input:

    for (int i = 0; i < numDigits; i++){
        
        storing.push_back({0,0,0,0,0,0,0,0,0,0});
        total = AnalyzeLine();
        
        // If distribution is not valid:
        
        if (1 - 0.00000001 > total || total > 1 + 0.00000001){
            cout << "Bad input on line " << i+1 << '\n';
            return 0;
        }
    }
    
    // Computes the "imperfection factor" of the distribution (Not an official name, just what I thought sounded good for what it does).
    
    storingRanked = ImperfectionFactor(storing);
    
    // Get rank all rows of storing in order from most uniform to least uniform distribution.
   
    sort(storingRanked.begin(), storingRanked.end(), ComparePair);
    
    cout << '\n';
    for (int i = 0; i < storingRanked.size(); i++){
        // 248 is the ASCII value for the degree symbol
        cout <<"n" << (char)(248) << i+1 << " best distribution: " << storingRanked[i].second << " at index " << storingRanked[i].first << '\n';
    }
    
    // The hash table size will always be 1000 because of the way my functions work. Any number that
    // cannot be written in the form 10^x would mess with them.

    cout << "\nTable capacity: 1000\n\n";

    for (int i = 0; i < numDigits; i++){
        if (storingRanked[i].first >= numDigits-3){
            imperfectionNormal += storingRanked[i].second;
        }
        if (i < 3){
            imperfectionChange += storingRanked[i].second;
        }
    }

    cout << "Imperfection factor of the first three: " << imperfectionNormal << "\nImperfection factor of the best three: " << imperfectionChange << "\n\n";

    whatInsert = false;
    if (imperfectionNormal - imperfectionChange > 1.75){ // In my testing I found this usually generated less operations
        whatInsert = true;
    }

    // Main while loop.

    while (continueing){

        for (int i = 0; i < 1009; i++) { hashTable[i] = -1; } // Reset hash table
        numOperations = 0;

        cout << "Enter a file name:\n";
        cin >> fileName;

        filePath = fileName;

        if (fileName == "Q") return 0;

        fileReading.open(filePath);

        if (!fileReading){ // If file cannot be opened
            cout << "\nUnable to open file. This might be due to it's location\n\n";
            continue;
        }

        fileReading >> numItems;

        for (int i = 0; i < numItems; i++){
            if (whatInsert){ // If there are significant gains
                numEnter = ChangeDistribution(numDigits, storingRanked[0].first, storingRanked[1].first, storingRanked[2].first);
            }
            else{ // If there are no significant gains
                long long num1;
                fileReading >> num1;
                numEnter.first = HashingFunctionSimple(num1, true);
                numEnter.second = num1;
            }

            if (hashTable[numEnter.first] == -1){ // If empty:
                hashTable[numEnter.first] = numEnter.second;
            }
            else{ // If full:
                hashTable[ProbingFunction(numEnter.first, SecondHash(numEnter.second, true), numEnter.second, true)] = numEnter.second;
            }

        }

        cout << "\nThe number of operations done in this file: " << numOperations << "\n\nPrint the hash table? 1 for yes, 0 for no.\n";

        cin >> printTable;

        // Get good input:

        while (printTable != 0 && printTable != 1){
            cout << "Bad input, enter a new value.\n";
            cin >> printTable;
        }

        // Print table:

        if (printTable){
            cout << "\n-1 represents NULL in my hash table.\n";
            for (int i = 0; i < 1000; i++){
                cout << hashTable[i] << " ";
            }
            cout << '\n';
        }

        cout << "\nEnter number to search for in hash table. (Can end search at any time by inputting -1)\n";
        cin >> numSearch;

        // Search in table:

        while (numSearch != "-1"){

            // If bad input:

            while (numSearch != "-1" && (numSearch[0] == '-' || numSearch.size() != numDigits)){
                cout << "\nBad input, try again.\n";
                cin >> numSearch;
            }

            if (numSearch == "-1") break;

            answerSearch = SearchTable(numSearch, whatInsert);

            if (answerSearch == -1) cout << "\nThis element is not in the table.\n";
            else cout << "\nThis number is stored at index " << answerSearch << '\n';

            cout << "\nEnter number to search for in hash table.\n";
            cin >> numSearch;
        }

        cout << "\n\n";

        fileReading.close();
    }
}