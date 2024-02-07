/**
 * Helper functions for Parser.cpp
 * Vincent Bucourt
*/

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>

// Lower number is higher priority
std::map<std::string, int> priority = {
    {"(", 1},
    {")", 1},
    {"^", 2},
    {"**", 2},
    {"sqrt", 2},
    {"`", 3},
    {"sin", 4},
    {"cos", 4},
    {"tan", 4},
    {"log", 4},
    {"*", 5},
    {"/", 5},
    {"+", 6},
    {"-", 6}
};

bool charDigit(char inp){
    return ('0' <= inp && inp <= '9');
}

bool charLetter(char inp){
    return ('a' <= inp && inp <= 'z') || ('A' <= inp && inp <= 'Z');
}

bool charOperator(char inp){
    return (!charDigit(inp) && !charLetter(inp));
}

std::vector<std::string> functions = {"sin", "cos", "tan", "log", "sqrt"};
std::vector<std::string> constants = {"pi"};

bool isFunction (std::string inp){
    for (std::string s : functions){
        if (s == inp) return true;
    }
    return false;
}

bool isConstant (std::string inp){
    for (std::string s : constants){
        if (s == inp) return true;
    }
    return false;
}

bool checkToken (std::string inp, int& i, int lenCheck, std::queue<std::string>& tokens){
    if (i < inp.size() - lenCheck + 1 && isFunction(inp.substr(i, lenCheck))){
        tokens.push(inp.substr(i, lenCheck));
        i += (lenCheck - 1);
        return true;
    }
    else if (i < inp.size() - lenCheck + 1 && isConstant(inp.substr(i, lenCheck))){
        tokens.push(inp.substr(i, lenCheck) + (i != inp.size() - 1 ? "*" : ""));
        i += (lenCheck - 1);
        return true;
    }
    return false;
}

void decompToken (std::string inp, std::queue<std::string>& tokens){
    std::string posToken, on;

    for (int i = 0; i < inp.size(); i++){
        
        bool works = false;
        for (int len = 1; len < 5; len++){

            works = checkToken (inp, i, len, tokens);
            if (works) break;
        }
        if (!works){
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
    char c;
    last = noSpace[0];
    for (int i = 0; i < noSpace.size(); i++){ // Splits input into tokens
        c = noSpace[i];

        same = (charDigit(c) && charDigit(last)) || (charLetter(c) && charLetter(last)) || (i == 0);

        if (same){ // If same type
            token += c;
        }
        else{ // If not same type

            if (charLetter(token[0])){ // If letters
                decompToken(token, tokens);
            }
            else{ // If numbers or operators

                // If first operand of right before another operand
                bool isUnary = token == "-" && (tokens.empty() || ( priority.find(tokens.back()) != priority.end() && tokens.back() != ")" ));
                if (isUnary){ 
                    token = "`";
                }
                
                tokens.push(token);
            }
            
            if ((charDigit(last) && charLetter(c)) || (last == ')' && (!charOperator(c) || c == '(')) ){ // If implicit multiplication
                tokens.push("*");
            }
            
            token = c;
        }

        last = c;
    }
    if (charLetter(token[0])){ // If letters
        decompToken(token, tokens);
    }
    else if (token != " "){ // If numbers
        tokens.push(token);
    }

    return tokens;
}

std::queue<std::string> parse (std::queue<std::string> inp){
    std::string tokenOn, nextToken;
    std::stack<std::string> operators;
    std::queue<std::string> ordered;

    while (!inp.empty()){
        tokenOn = inp.front();

        if (!isFunction(tokenOn) && (charDigit(tokenOn[0]) || charLetter(tokenOn[0]))){ // It inp.front() is a number
            ordered.push(tokenOn);
        }
        else if (tokenOn == ")"){ // Parenthesis
            while (operators.top() != "("){
                ordered.push(operators.top());
                operators.pop();
            }
            operators.pop();
        }
        else if ((operators.empty()) || (priority[tokenOn] < priority[operators.top()])){ // If higher order (in good order)
            operators.push(tokenOn);
        }
        else{ // If lower order (in bad order)
            while ((!operators.empty()) && (priority[tokenOn] >= priority[operators.top()]) && (operators.top() != "(")){ // While bad
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

/**
 * Returns if a (*) symbol is needed
*/
bool needMul (std::string token2){ // If you need a multiplication symbol
    return (charDigit(token2[0]));
}

/**
 * Returns if a token is an operator
*/
bool isOperator (std::string token){
    return !(priority[token] == 0 || priority[token] == 1);
}

/**
 * Turns sorted tokens into LaTex format
*/
std::string toTex (std::queue<std::string> inp){
    std::string tokenOn;
    std::pair<std::string, int> str1, str2, toAdd;
    std::stack<std::pair<std::string, int>> prev; // first is the token, second is the priority of last operation
    bool firstElement = true;

    if (inp.front() == "") return "$$";

    while (!inp.empty()){
        tokenOn = inp.front();
        toAdd.second = 0;
        
        if (isConstant(tokenOn)){
            toAdd.first = "\\" + tokenOn + " ";
        }
        else if (isFunction(tokenOn)){ // If function
            toAdd.first = "\\" + tokenOn + "{(" + prev.top().first + ")}";
            prev.pop();
        }
        else if (charDigit(inp.front()[0]) || charLetter(tokenOn[0])){ // If on is not an operator
            toAdd.first = tokenOn;
            toAdd.second = -1;
        }
        else if (tokenOn == "/"){ // If fraction
            
            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();
            
            toAdd.first = "\\dfrac{" + str1.first + "}{" + str2.first + "}";
        }
        else if (tokenOn == "^" || tokenOn == "**"){ // If exponential
            
            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();
            
            toAdd.first = "(" + str1.first + ")^{" + str2.first + "}";
        }
        else if (tokenOn == "`"){ // If unary (-) (Chose ` objectively)
            str1 = prev.top();
            prev.pop();

            if (priority["`"] < str1.second){ // If lower priority before (-)   (ex: -(x + 1))
                str1.first = "(" + str1.first + ")";
            }
            toAdd.first = "-" + str1.first;
        }
        else if (tokenOn == "*"){ // If multiplication
            
            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();
            
            if (priority["*"] < str1.second){
                str1.first = "(" + str1.first + ")";
                str1.second = 1;
            }
            if (priority["*"] < str2.second){
                str2.first = "(" + str2.first + ")";
                str2.second = 1;
            }
            if (needMul(str2.first)){
                str2.first = "\\cdot " + str2.first;
            }
            
            toAdd.first = str1.first + str2.first;
            
        }
        else if (tokenOn == "-"){ // If subtraction (-)
                str2 = prev.top();
                prev.pop();
                str1 = prev.top();
                prev.pop();

                toAdd.first = str1.first + "-" + str2.first;
        }
        else{ // If no special latex scripting
            
            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();
            
            toAdd.first = str1.first + tokenOn + str2.first;
        }
        
        if (toAdd.second != -1) toAdd.second = priority[tokenOn];
        
        prev.push(toAdd);
        inp.pop();
        firstElement = false;
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