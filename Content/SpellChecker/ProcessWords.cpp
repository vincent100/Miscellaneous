/**
Processes the words (turns them into a vector sorted by size of words)
This puts them into vector form, which I then copy pasted into a .h file
*/

#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::vector<std::string>> words (22); // Max size of word is 22

signed main(){
    std::ifstream read("EnglishWords.txt");

    std::string on;
    for (int i = 0; i < 10000; i++){ // Number of words
        read >> on;
        words[on.size() - 1].push_back(on);
    }
    read.close();

    std::ofstream dictionary("FinalDictionary.txt");

    // Write it to old txt file to avoid recomp every time

    dictionary << "{ ";
    for (int i = 0; i < 22; i++){
        dictionary << "\n    {\n";
        for (std::string s : words[i]){
            dictionary << " \"" << s << "\", ";
        }
        dictionary << "    },\n";
    }
    dictionary << "}";
    dictionary.close();
}