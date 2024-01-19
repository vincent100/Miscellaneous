/**
This is my attempt at building a spell checker (not using AI, deterministic)
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

// DP recurrence to find how many edits turns word1 -> word2
int findLeastDif (std::string word1, std::string word2){
    std::vector<std::vector<int>> dp (word1.size(), std::vector<int> (word2.size(), maxVal32 )); // dp[word1][word2]

    if (word1[0] == word2[0]) dp[0][0] = 0;
    else dp[0][0] = 1;

    for (int i = 1; i < word1.size(); i++){
        dp[i][0] = dp[i-1][0] + 1;
    }

    for (int i = 1; i < word2.size(); i++){
        dp[0][i] = dp[0][i-1] + 1;
    }

    for (int i = 1; i < word1.size(); i++){
        for (int d = 1; d < word2.size(); d++){
            dp[i][d] = std::min(dp[i-1][d], std::min(dp[i][d-1], dp[i-1][d-1]) ); // dp recurence
            if (word1[i] != word2[d]) dp[i][d]++;
        }
    }

    return dp[word1.size()-1][word2.size()-1];
}

// Tests all "reasonable" matching words (+- 3 length)
std::stack<std::pair<int, std::string>> spellCheck(std::string word){
    std::stack<std::pair<int, std::string>> match;
    int difOn;
    int whereAdd;
    bool toAdd;

    for (int i = std::max(0, (int) word.size() - 3); i <= word.size() + 3; i++){ // offset (-3 to 3)

        for (std::string s : dictionary[i]){ // For all words of length i

            toAdd = false;
            difOn = findLeastDif(word, s);
            if (difOn == 1){ // great match
                match.push(std::make_pair(difOn, s));
            }

        }

    }

    return match;

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
        std::stack<std::pair<int, std::string>> returnVal = spellCheck(inp);

        // Output
        std::cout << "Good matches:\n";

        while (!returnVal.empty()){
            std::cout << returnVal.top().second << " (" << returnVal.top().first << " changes)\n";
            returnVal.pop();
        }
    }
}