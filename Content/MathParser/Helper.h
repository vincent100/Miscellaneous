#pragma once

#ifndef helper
#define helper

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>

bool charDigit (char inp);
bool charLetter (char inp);
bool isToken (std::string inp);
std::queue<std::string> decompToken (std::string inp);
std::queue<std::string> getTokens (std::string inp);

std::queue<std::string> parse (std::queue<std::string> inp);
std::string toTex (std::queue<std::string> inp);

#endif