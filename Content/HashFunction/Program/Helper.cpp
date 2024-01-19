/**
 * This function contains all helper functions, clearing up the main function for added visibility.
 * 
 * This program was written in C++ 20, with Visual Studio Code on Windows 11
*/


// Beginning of Header

#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <algorithm>
#include <cmath>
#include <queue>
#include <fstream>
#include <string>

#include "Main.h"

using namespace std;
using pairInt = pair<int, long long>;

// End of Header



/**
 * 
 * Helper functions for getting the input of digit distribution
 * 
 */


// Takes a decimal or fraction in the form of a string and turns it into
// a double.

double StrToDouble(string strOn){
    
    string frac[2] = {"", "1"};
    long long onFrac = 0;
    double returnDouble;
    
    for (int i = 0; i < strOn.size(); i++){
        
        if (strOn[i] == '/'){
            onFrac = 1;
            frac[1] = "";
            continue;
        }
        
        frac[onFrac] += strOn[i];
    }
    
    returnDouble = atof(frac[0].c_str()) / atof(frac[1].c_str());
    
    return returnDouble;
}

// Function to get a line of distribution from the user

double AnalyzeLine(){
    
    string getLine;
    double total = 0; // Sum of all numbers in one line
    double onnow;
    
    // For every number on one line:
    
    for (int d = 0; d < 10; d++){
        cin >> getLine;
        onnow = StrToDouble(getLine);
        
        total += onnow;
        storing[storing.size()-1][d] = onnow;
    }
    
    return total;
}




/**
 * 
 * Helper functions for finding "imperfection factor" of one distribution. 
 * 
 * The imperfection factor is abs(0.1-onnow)^2. I use 0.1 because it is the most
 * standard distribution possible (1/10). abs() because no negative values, or
 * they will rank higher. ^2 because the smaller it is, the smaller it becomes,
 * and vice versa (not linear).
 * 
 * This Imperfection Factor iterates over every length between 1 and 9, summing up everything in 
 * between. This is to differentiate between something like {0 0.5 0 0 0.5 0 0 0 0 0} and {0 0 0.5 0.5 0 0 0 0 0 0}.
 * The closer values are, the more they penalise the distribution.
 * 
 * For the least standard distribution, the total factor is 1.92897
 * The least standard distribution is one 1 and 9 0s
 *    input: {0 1 0 0 0 0 0 0 0 0}
 *    output: 1.92897
 * 
 */

// For sorting vectorArrayRanked in main.cpp

bool ComparePair(pair<int, double> pair1, pair<int, double> pair2) { return pair1.second < pair2.second; } 

// For computing the imperfection factor of the distributions

vector<pair<int, double>> ImperfectionFactor(vector<array<double, 10>> vectorArray) {
    
    double totalDif = 0;
    vector<pair<int, double>> vectorArrayRanked;
    double difference; // Finds the difference from perfectly uniform input for every size. (Not linear)
    double total;
    
    for (int row = 0; row < vectorArray.size(); row++){
        totalDif = 0;
        for (int size = 0; size < 9; size++){
            
            total = 0;
            difference = 0;
            
            for (int i = 0; i < size; i++){
                total += vectorArray[row][i];
            }
            
            for (int column = 0; column < 10; column++){
                total += vectorArray[row][(column+size) % 10];
                difference += pow ( abs ( (0.1 * ( size + 1 ) - total ) / ( size + 1 ) ), 2 ); // Computes imperfection factor
                total -= vectorArray[row][column];
            }
            
            totalDif += difference;
            
        }

        vectorArrayRanked.push_back(make_pair(row, totalDif));
    }
    
    return vectorArrayRanked;
}


/**
 * 
 * Helper functions for the case where there are a lot of gains from using a new distribution.
 * The gains must be good because every addition to the hash table takes 7 operations (without counting
 * possible probing function). 
 * 
 * (Char_To_Int(firstDigit) * 100 + Char_To_Int(secondDigit) * 10 + Char_To_Int(thirdDigit)) = 7 operations, where
 * Char_To_Int turns a character into its corresponding int by 'character' - '0'.
 * 
 * If the distribution is good without it, my program uses a simple modular hash function.
 * 
 * In both cases, the probing function is a double hashing function because I found that this, on average,
 * minimizes the number of operations. While double hashing would also be a good option, I found in my testing that
 * double hashing makes up for the larger upfront operational cost by outputting a much more even distribution.
 * 
*/

// The hashing function using data from the "Imperfection factor"

signed HashingFunctionChange(char char1, char char2, char char3, bool insert){
    if (insert) numOperations += 7;
    return (char1 - '0') * 100 + (char2 - '0') * 10 + (char3 - '0');
}

// The more general function that uses the 

pairInt ChangeDistribution(int numDigits, int where1, int where2, int where3){
    char char1, char2, char3;
    char charOn;
    string storingTotal;

    // The input is not counted in the total operations done, just like in the example

    for (int i = 0; i < numDigits; i++){
        fileReading >> charOn;
        storingTotal += charOn;
        if (i == where1) { char1 = charOn; }
        else if (i == where2) { char2 = charOn; }
        else if (i == where3) { char3 = charOn; }
    }

    // The above operations are not counted because they are part of the input

    return make_pair(HashingFunctionChange(char1, char2, char3, true), stoll(storingTotal));
}

bool DistributionGain(){
    return true;
}

/**
 * 
 * Helper functions for the case where there are little gains from using a new distribution.
 * Every addition to the hash table takes 1 operation (without counting the possible probing
 * function)
 * 
 * In both cases, the probing function is a double hashing function because I found that this, on average,
 * minimizes the number of operations. While quadratic probing also minimizes clustering, I chose double 
 * hashing for two reasons:
 * 
 * 1) Quadratic probing does not always find an empty spot in the Hash Table
 * 2) In my testing, I found that quadratic probing did less well than double hashing
 * 
*/

// The hashing function that does not use the "Imperfection Factor"

signed HashingFunctionSimple(long long numHash, bool insert){
    if (insert) numOperations++;
    return numHash % 1009;
}

// The probing function I use is a double hashing function. It is used in both above cases. 

signed SecondHash(long long numHash, bool insert){
    if (insert) numOperations += 2; // If not used for lookup
    return (numHash % 101 ) + 1;
}

signed ProbingFunction(int hash1, int hash2, long long original, bool insert){
    int counting = 0, on;
    while (true){
        counting++;
        if (insert) numOperations += 3;
        on = (hash1 + counting * hash2) % 1009;
        if (!insert && hashTable[on] == original) return on;
        if (hashTable[on] == -1){
            if (insert) return on;
            else return -1;
        }
    }
}

// The function used for searching if an element is present in the Hash Table.
// This function calls the above functions, 

signed SearchTable(string whatSearch, bool whatHash){

    // whatHash tracks what hashing function was used (Simple or complex)
    long long originalNum = stoll(whatSearch);
    int valueHash1, valueHash2;
    string whatSearchStr;

    if (whatHash){ // Uses first type of hash function
        valueHash1 = HashingFunctionChange(whatSearch[storingRanked[0].first], whatSearch[storingRanked[1].first], whatSearch[storingRanked[2].first], false);
    }
    else{ // Uses second type of hash function
        valueHash1 = HashingFunctionSimple(originalNum, false);
    }

    cout << valueHash1 << "\n";

    if (hashTable[valueHash1] == -1) return -1;
    else if (hashTable[valueHash1] == originalNum) return valueHash1;
    else{
        valueHash2 = SecondHash(originalNum, false);
        return ProbingFunction(valueHash1, valueHash2, originalNum, false);
    }
}