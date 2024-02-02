/**
 * Helper functions for Parser.cpp
 * Vincent Bucourt
*/

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>

bool charDigit(char inp){
    return ('0' <= inp && inp <= '9');
}

bool charLetter(char inp){
    return ('a' <= inp && inp <= 'z') || ('A' <= inp && inp <= 'Z');
}

std::vector<std::string> fun = {"sin", "cos", "tan", "log", "sqrt"};

bool isToken (std::string inp){
    for (std::string s : fun){
        if (s == inp) return true;
    }
    return false;
}

void decompToken (std::string inp, std::queue<std::string>& tokens){
    std::string posToken, on;

    for (int i = 0; i < inp.size(); i++){

        if (i < inp.size() - 3 && isToken(inp.substr(i, 4))){ // If operand
            on = inp.substr(i, 4);
            tokens.push(on);
            i += 3;
        }
        else if (i < inp.size() - 2 && isToken(inp.substr(i, 3))){ // If operand
            on = inp.substr(i, 3);
            tokens.push(on);
            i += 2;
        }
        else{
            on = inp[i];
            tokens.push(on);
            if (i != inp.size() - 1) tokens.push("*");
        }
    }
}

std::queue<std::string> getTokens(std::string in){
    char last;
    std::string token = "", noSpace = "";
    std::queue<std::string> tokens;

    for (char c : in){ // Remove spaces
        if (c != ' '){
            noSpace += c;
        }
    }

    bool same;
    last = noSpace[0];
    for (char c : noSpace){ // Splits input into tokens

        same = (charDigit(c) && charDigit(last)) || (charLetter(c) && charLetter(last));

        if (same){ // If same type
            token += c;
        }
        else{ // If not same type

            if (charLetter(token[0])){ // If letters
                decompToken(token, tokens);
            }
            else{ // If numbers
                tokens.push(token);
            }
            
            if (charDigit(last) && charLetter(c) && last != ' '){ // If implicit multiplication
                tokens.push("*");
            }
            token = c;
        }

        last = c;
    }
    if (charLetter(token[0])){ // If letters
        decompToken(token, tokens);
    }
    else{ // If numbers
        tokens.push(token);
    }

    return tokens;
}

// Lower number is higher priority
std::map<std::string, int> priority = {
    {"(", 1},
    {")", 1},
    {"^", 2},
    {"**", 2},
    {"sqrt", 2},
    {"sin", 3},
    {"cos", 3},
    {"tan", 3},
    {"log", 3},
    {"*", 4},
    {"/", 4},
    {"+", 5},
    {"-", 5}
};

std::queue<std::string> parse (std::queue<std::string> inp){
    std::string tokenOn, nextToken;
    std::stack<std::string> operators;
    std::queue<std::string> ordered;

    while (!inp.empty()){
        tokenOn = inp.front();

        if (!isToken(tokenOn) && (charDigit(tokenOn[0]) || charLetter(tokenOn[0]))){ // It inp.front() is a number
            ordered.push(tokenOn);
        }
        else if (tokenOn == ")"){ // Parenthesis
            while (operators.top() != "("){
                ordered.push(operators.top());
                operators.pop();
            }
            operators.pop();
        }
        else if ((operators.empty()) || (priority[tokenOn] <= priority[operators.top()])){ // If higher order (in good order)
            operators.push(tokenOn);
        }
        else{ // If lower order (in bad order)
            while ((!operators.empty()) && (priority[tokenOn] > priority[operators.top()]) && (operators.top() != "(")){ // While bad
                ordered.push(operators.top());
                operators.pop();
            }
            operators.push(tokenOn);
        }

        inp.pop();
    }
    while (!operators.empty()){
        ordered.push(operators.top());
        operators.pop();
    }
    if (ordered.front() == "") ordered.pop(); // To get rid of extra from parenthesis

    return ordered;
}

bool needMul (std::string token2){
    bool allLet = true;
    for (char c : token2){
        if (charDigit(c)) allLet = false;
    }
    return !allLet;
}

std::string toTex (std::queue<std::string> inp){
    std::string tokenOn;
    std::pair<std::string, int> str1, str2, toAdd;
    std::stack<std::pair<std::string, int>> prev;

    if (inp.front() == "") return "$$";

    while (!inp.empty()){
        tokenOn = inp.front();

        if (isToken(tokenOn)){ // If trig function
            toAdd.first = "\\" + tokenOn + "{(" + prev.top().first + ")}";
            prev.pop();
        }
        else if (charDigit(inp.front()[0]) || charLetter(tokenOn[0])){ // If on is not an operator
            toAdd.first = tokenOn;
        }
        else if (tokenOn == "/"){ // If fraction

            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();

            toAdd.first = "\\dfrac{" + str1.first + "}{" + str2.first + "}";
        }
        else if (tokenOn == "^" || tokenOn == "**"){

            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();

            toAdd.first = "(" + str1.first + ")^{" + str2.first + "}";
        }
        else if (tokenOn == "*"){

            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();

            if (priority[tokenOn] >= str1.second && priority[tokenOn] >= str2.second){
                toAdd.first = str1.first + (needMul(str2.first) ? "*" : "") + str2.first;
            }
            else{
                toAdd.first = "(" + str1.first + ")" + (needMul(str2.first) ? "*" : "") + "(" + str2.first + ")";
            }
        }
        else{ // If no special latex scripting

            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();

            toAdd.first = str1.first + tokenOn + str2.first;
        }

        toAdd.second = priority[tokenOn];

        prev.push(toAdd);
        inp.pop();
    }

    std::string ans = "$" + prev.top().first + "$";
    return ans;
}

signed main(){
    std::string inp;
    std::cout << "Enter mathematical formula:\n";
    std::getline(std::cin, inp);

    std::queue<std::string> tokens = getTokens(inp);

    std::queue<std::string> goodOrder = parse (tokens);

    std::string finalAns = toTex(goodOrder);
    std::cout << "Your mathematical formula in LaTex script:\n" << finalAns << '\n';
}