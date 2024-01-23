/**

This is my attempt at building a deterministic spell checker
I tried to used DP to improve on the obvious recursive
answer.
This file takes a long time to compile because of the
dictionary. (10k words)
For convenience, compiled .exe file is included (compileing
is rather slow, but execution is fast)
Vincent Bucourt

*/

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>

#include "ImportDictionary.h"

const int maxVal32 = (1 << 30);

struct Match{
    std::string word;
    double val;
};

Match bestMatch;

// DP recurrence to find how many edits turns word1 -> word2
double findLeastDif (std::string from, std::string to){

    std::vector<std::vector<double>> dp (from.size(), std::vector<double> (to.size(), maxVal32 )); // dp[from][to]

    // Both words are made same length

    if (from[0] == to[0]) dp[0][0] = 0;
    else if (fingers[from[0] - 'a'] == fingers[to[0] - 'a']) dp[0][0] = 0.5;
    else dp[0][0] = 1;

    for (int i = 1; i < from.size(); i++){
        dp[i][0] = dp[i-1][0] + 1;
    }

    for (int i = 1; i < to.size(); i++){
        dp[0][i] = dp[0][i-1] + 1;
    }

    for (int i = 1; i < from.size(); i++){
        for (int d = 1; d < to.size(); d++){

            dp[i][d] = std::min(dp[i][d], dp[i-1][d] + 0.75);
            dp[i][d] = std::min(dp[i][d], dp[i][d-1] + 0.75);

            if (from[i] == to[d]){
                dp[i][d] = std::min(dp[i][d], dp[i-1][d-1]);
            }
            else if (from[i-1] == to[d] && from[i] == to[d-1]){
                dp[i][d] = std::min(dp[i][d], dp[i-1][d-1] - 0.5);
            }
            else{
                dp[i][d] = std::min(dp[i][d], dp[i-1][d-1] + 1);
            }
        }
    }

    return dp[from.size()-1][to.size()-1]; // last square is when they will both be equal
}

// Tests all "reasonable" matching words (+- 3 length)
void spellCheck(std::string word){
    double difOn;


    for (int i = std::max(0, (int) word.size() - 3); i <= std::min(22, (int) word.size() + 1); i++){ // offset (-2 to +2)

        for (std::string s : dictionary[i]){ // For all words of length i

            difOn = findLeastDif(word, s);
            if (difOn < bestMatch.val){
                bestMatch.val = difOn;
                bestMatch.word = s;
            }

        }

    }

}

signed main(){
    std::string inp;
    std::cout << "Enter a word, and the autocorrect will try to correct it:\n";
    std::cin >> inp;

    bool goodWord = false;
    for (std::string s : dictionary[ inp.size() - 1 ]){
        if (s == inp) goodWord = true;
    }

    if (goodWord){ // If word is in word list
        std::cout << "Word is already well spelled\n";
    }
    else{
        // Run checks on possible words
        bestMatch.val = 9999.0;
        spellCheck(inp);

        // Output

        std::cout << "Best Match:   " << bestMatch.word << '\n';
        
    }
}
