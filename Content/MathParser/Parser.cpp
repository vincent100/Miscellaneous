/**
 * Program that turns normal typed math into latex format
 * All of the actual work is being done by functions in 
 * Helper.cpp
 * 
 * Vincent Bucourt
*/

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>

#include "Helper.h"

// Main function

signed main(){
    std::string inp;
    std::cout << "Enter mathematical formula:\n";
    std::getline(std::cin, inp);

    std::queue<std::string> tokens = getTokens(inp);

    std::queue<std::string> goodOrder = parse (tokens);

    std::string finalAns = toTex(goodOrder);
    std::cout << "Your mathematical formula in LaTex script:\n" << finalAns << '\n';
}