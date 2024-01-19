#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <random>
#include <fstream>

using namespace std;

// Credit: I used "https://en.cppreference.com/w/cpp/algorithm/random_shuffle" for shuffle() function.

/** 
 * This test generator does not necessarily generate unique elements. 
 * This was a concious decision: It pumps up the total ammount of calculations with little to no downsides.
 * If my program passes these tests, it passes tests with unique numbers: It is just a more general
 * tester.
 * 
 * This test generator takes in a distribution and makes a test case.
 *  This helps with making more skewed tests.
 *  Enter distribution in fraction form.
 */

signed main(){
    ofstream fileWrite;
    int numDigits, numNums;
    vector<vector<pair<int, int>>> distribution; // Stores digit distribution
    int multiUse; // Variable for ad hoc uses
    char multiChar; // Variable for ad hoc uses
    const pair<int, int> emptyPair = make_pair(0, 0); 
    vector<string> finalTest; // Final numbers for the test
    vector<int> oneToN; // Stores numbers 1...N for sample() function
    string fileName;

    cout << "Enter file name:\n";
    cin >> fileName;
    fileWrite.open(fileName);

    cout << "Enter the number of digits:\n";
    cin >> numDigits;
    cout << "Enter distribution (in fraction form)\n";

    for (int i = 0; i < numDigits; i++){
        distribution.push_back({emptyPair, emptyPair, emptyPair, emptyPair, emptyPair, emptyPair, emptyPair, emptyPair, emptyPair, emptyPair});
        for (int d = 0; d < 10; d++){
            cin >> multiUse;
            distribution[i][d].first = multiUse;
            cin >> multiChar;
            cin >> multiUse;
            distribution[i][d].second = multiUse;
        }
    }

    cout << "How many numbers?\n";
    cin >> numNums;

    int on;
    int total = 0;

    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < numNums; i++){
        finalTest.push_back("");
    }

    for (int i = 0; i < numDigits; i++){
        for (int d = 0; d < 10; d++){
            for (int w = 0; w < distribution[i][d].first; w++){
                oneToN.push_back(d);
            }
        }
        shuffle(oneToN.begin(), oneToN.end(), g);
        for (int i = 0; i < numNums; i++){
            finalTest[i] += oneToN[i] + '0';
        }
        oneToN.clear();
    }

    fileWrite << numNums;
    for (int i = 0; i < numNums; i++){
        fileWrite << (i % 10 == 0 ? "\n" : "") << finalTest[i] << ' ';
    }

    fileWrite << "\n\n" << numDigits << '\n';
    for (int i = 0; i < numDigits; i++){
        for (int d = 0; d < 10; d++){
            fileWrite << distribution[i][d].first << '/' << distribution[i][d].second << ' ';
        }
        fileWrite << '\n';
    }

    fileWrite.close();
}