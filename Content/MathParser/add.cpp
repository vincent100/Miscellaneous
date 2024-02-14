/**
 * Helper functions for Parser.cpp
 * Vincent Bucourt
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <stack>
#include <queue>
#include <map>

// Global Variables
std::map<std::string, int> priority = { // Lower number is higher priority
    {"(", 1},
    {")", 1},
    {"|", 1},
    {"(|", 1}, // Left abs sign
    {"|)", 1}, // Right abs sign
    {"^", 2},
    {"**", 2},
    {"sqrt", 2},
    {"sin", 4},
    {"cos", 4},
    {"tan", 4},
    {"log", 4},
    {"--", 5},
    {"*", 6},
    {"/", 6},
    {"+", 7},
    {"-", 7},
    {"=", 8}
};

std::vector<std::string> functions = {"sin", "cos", "tan", "log", "sqrt", "Sigma"}; // Functions
std::vector<std::string> constants = {"pi", "sigma"}; // Constants

std::vector<bool> sideAbs; // Ignore if no (|), false if left (|), true if right (|)

// Functions
bool charDigit(char inp);
bool charLetter(char inp);
bool charOperator(char inp);

bool isFunction (std::string inp);
bool isConstant (std::string inp);

bool constantToken (std::string inp, int& i, int lenCheck, std::queue<std::string>& tokens);
bool constantFunction (std::string inp, int& i, int lenCheck, std::queue<std::string>& tokens);

void decompToken (std::string inp, std::queue<std::string>& tokens);
std::queue<std::string> getTokens(std::string in);

std::queue<std::string> machineSort (std::queue<std::string> inp);

std::string toTex (std::queue<std::string> inp);

// Main
signed main(){
    std::string inp;
    std::queue<std::string> tokens;
    
    std::cout << "Enter mathematical formula:\n";
    std::getline(std::cin, inp);
    sideAbs.resize (inp.size());
    
    tokens = getTokens(inp);
    
    // std::cout << "\n";
    // while (!tokens.empty()){
    //     std::cout << tokens.front() << "\n";
    //     tokens.pop();
    // }
    
    tokens = machineSort (tokens);
    
    try{
        std::string finalAns = toTex(tokens);
        std::cout << "\n\nYour mathematical formula in LaTeX script:\n" << finalAns << '\n';
    }
    catch (std::invalid_argument& e){ // If user types something invalid
        std::cerr << "\n\nBad input:\n" << e.what() << "\n";
        return -1;
    }
    
    return 0;
}

/**
 * If character is a digit
 */
bool charDigit(char inp){
    return ('0' <= inp && inp <= '9');
}

/**
 * If character is a letter
 */
bool charLetter(char inp){
    return ('a' <= inp && inp <= 'z') || ('A' <= inp && inp <= 'Z');
}

/**
 * If character is an operator
 */
bool charOperator(char inp){
    return (!charDigit(inp) && !charLetter(inp));
}

/**
 * If string is a function
 */
bool isFunction (std::string inp){
    for (std::string s : functions){
        if (s == inp) return true;
    }
    return false;
}

/**
 * If string is a constant
 */
bool isConstant (std::string inp){
    for (std::string s : constants){
        if (s == inp) return true;
    }
    return false;
}

/**
 * Check if substring is a constant
 */
bool constantToken (std::string inp, int& i, int lenCheck, std::queue<std::string>& tokens){
    
    if (i < inp.size() - lenCheck + 1 && isConstant(inp.substr(i, lenCheck))){
        tokens.push(inp.substr(i, lenCheck));
        if (i + lenCheck != inp.size()){
            tokens.push("*");
        }
        i += (lenCheck - 1);
        return true;
    }
    return false;
}

/**
 * Check if substring is a constantToken
 */
bool functionToken (std::string inp, int& i, int lenCheck, std::queue<std::string>& tokens){
    if (i < inp.size() - lenCheck + 1 && isFunction(inp.substr(i, lenCheck))){
        tokens.push(inp.substr(i, lenCheck));
        i += (lenCheck - 1);
        return true;
    }
    return false;
}

/**
 * Decomposes string of letters into parts
 */
void decompToken (std::string inp, std::queue<std::string>& tokens){
    std::string posToken, on;

    for (int i = 0; i < inp.size(); i++){
        
        bool works = false;
        for (int len = 1; len < 6; len++){
            
            works = constantToken (inp, i, len, tokens) || functionToken (inp, i, len, tokens);
            
            if (works){ // If valid emplacement
                break;
            }
        }
        
        if (!works){
            on = inp[i];
            tokens.push(on);
            if (i != inp.size() - 1) tokens.push("*");
        }
    }
}

/**
 * Decomposes string into tokens
 */
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
    char on;
    last = noSpace[0];
    
    // Abs checks
    bool lastAbsType = false; // False means right |, true means left |
    bool insideAbs = false; // If something between (| |)
    int depthAbs = 0; // Depth inside abs (| |)
    
    for (int i = 0; i <= noSpace.size(); i++){ // Splits input into tokens
        
        if (i < noSpace.size()){
            on = noSpace[i];
        }
        else{
            on = ' ';
        }
        
        same = (charDigit(on) && charDigit(last)) || (charLetter(on) && charLetter(last)) || (i == 0);
        
        if (same){ // If same type (longer token)
            token += on;

            if (on == '|' && i == 0){
                depthAbs++;
                insideAbs = false;
                lastAbsType = true;
                sideAbs[0] = true;
            }
        }
        
        if (!same || i == noSpace.size()) { // If not same type
            
            if (charLetter(token[0])) // If letters
            {
                decompToken(token, tokens);
            }
            else{ // If numbers or operators
                
                // Abs checks
                if (on == '|' && (depthAbs == 0 || !insideAbs))
                { // If go one level deeper
                    depthAbs++;
                    insideAbs = false;
                    lastAbsType = true;
                }
                else if (depthAbs > 0 && on != '|')
                { // If inside | |
                    insideAbs = true;
                }
                else if (insideAbs && on == '|')
                { // If go one level less deep
                    depthAbs--;
                    insideAbs = false;
                    lastAbsType = false;
                }

                if (on == '|') sideAbs[i] = lastAbsType;
                
                // If first operand or right before another operand
                if (token == "-" && (tokens.empty() || ( priority.find(tokens.back()) != priority.end() && (tokens.back() != ")" || tokens.back() == "(") )))
                {
                    token = "--";
                }
                
                if (token == "|"){
                    tokens.push( (sideAbs[i-1] ? "(|" : "|)" ) );
                }
                else{
                    tokens.push(token);
                }

                // If implicit multiplication
                if ( (charLetter(last) || charDigit(last) || last == ')' || ( last == '|' && !sideAbs[i-1] )) && (charLetter(on) || charDigit(on) || on == '(' || ( on == '|' && sideAbs[i] )) )
                {
                    std::cout << last << " " << on << "\n";

                    tokens.push("*");
                }
                
            }
            
            token = on;
        }
        
        last = on;
    }
    
    return tokens;
}

/**
 * Sorts tokens in machine readable order
 */
std::queue<std::string> machineSort (std::queue<std::string> inp){
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
        // If (strictly higher priority than last operator) or (right asociative operator)
        else if ( (operators.empty()) || (priority[tokenOn] < priority[operators.top()]) || (priority[tokenOn] == priority[operators.top()] && (tokenOn == "^" || tokenOn == "**" || isFunction(tokenOn)) ) )
        {
            operators.push(tokenOn);
        }
        // If (lower or equal priority) (and left asociative)
        else{
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
 * Turns sorted tokens into LaTeX format
*/
std::string toTex (std::queue<std::string> inp){
    std::string tokenOn;
    std::pair<std::string, int> str1, str2, toAdd;
    std::stack<std::pair<std::string, int>> prev; // first is the token, second is the priority of last operation
    bool firstElement = true;

    if (inp.front() == "") throw std::invalid_argument ("Empty formula");

    while (!inp.empty()){
        tokenOn = inp.front();
        toAdd.second = 0;
        
        if (isConstant(tokenOn)){
            toAdd.first = "\\" + tokenOn + " ";
        }
        else if (isFunction(tokenOn)){ // If function
        
            if (prev.empty()){ // If syntax error
                throw std::invalid_argument("No arguments for " + tokenOn + "()");
            }
            toAdd.first = "\\" + tokenOn + "{(" + prev.top().first + ")}";
            prev.pop();
        }
        else if (charDigit(inp.front()[0]) || charLetter(tokenOn[0])){ // If on is not an operator
            toAdd.first = tokenOn;
            toAdd.second = -1;
        }
        else if (tokenOn == "/"){ // If fraction
            if (prev.size() < 2){
                throw std::invalid_argument("Too little arguments for (/)");
            }
            
            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();
            
            toAdd.first = "\\dfrac{" + str1.first + "}{" + str2.first + "}";
        }
        else if (tokenOn == "^" || tokenOn == "**"){ // If exponential
            if (prev.size() < 2){
                throw std::invalid_argument("Too little arguments for (^)");
            }
            
            str2 = prev.top();
            prev.pop();
            str1 = prev.top();
            prev.pop();
            
            toAdd.first = (str1.second == -1 ? "" : "(") + str1.first + (str1.second == -1 ? "" : ")") + "^{" + str2.first + "}";
        }
        else if (tokenOn == "--"){ // If unary (-) (Chose -- objectively)
            if (prev.size() < 1){
                throw std::invalid_argument("Too little arguments for (-)");
            }
            
            str1 = prev.top();
            prev.pop();
            
            if (priority["--"] < str1.second){ // If lower priority before (-)   (ex: -(x + 1))
                str1.first = "(" + str1.first + ")";
            }
            toAdd.first = "-" + str1.first;
        }
        else if (tokenOn == "*"){ // If multiplication
            if (prev.size() < 2){
                throw std::invalid_argument("Too little arguments for (*)");
            }
            
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
        else{ // If no special LaTeX scripting
            if (prev.size() < 2){
                throw std::invalid_argument ("Too little arguments for (" + tokenOn + ")");
            }
            
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