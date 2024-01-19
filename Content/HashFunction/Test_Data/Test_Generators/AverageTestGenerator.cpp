#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <stdlib.h> 
#include <cmath>
#include <string>
#include <filesystem>

using namespace std;

signed main(){

    int numDigits; // How many digits
    int numNums; // How many numbers to generate
    vector<array<double, 10>> distribution; // The distribution of the digits
    string fileName; // What file to write this generation to
    ofstream fileWrite;
    string onString; // Number currently on in string form

    cout << "File name:\n";
    cin >> fileName;
    filesystem::path path1 = fileName;
    fileWrite.open(path1);

    cout << "How many digits?\n";
    cin >> numDigits;
    cout << "How many numbers?\n";
    cin >> numNums;

    for (int i = 0; i < numDigits; i++){ // Initialize vector
        distribution.push_back({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    }

    fileWrite << numNums;

    for (int i = 0; i < numNums; i++){
        onString = "";
        for (int i = 0; i < numDigits; i++){
            onString += rand() % 10 + '0';
        }
        fileWrite << (i % 10 == 0 ? "\n" : "") << onString << " "; // Formatting
        for (int d = 0; d < numDigits; d++){ // Add number to the distribution
            distribution[d][onString[d] - '0']++;
        }
    }

    fileWrite << "\n\nThe load factor is " << (numNums/10) << "%\n\n" "\n\nThis is the distribution input for this test (This is just a note):\n\n";

    fileWrite << numDigits << '\n';

    for (int i = 0; i < numDigits; i++){
        for (int d = 0; d < 10; d++){
            fileWrite << distribution[i][d] << '/' << numNums << ' ';
        }
        fileWrite << '\n';
    }

    fileWrite.close();
}