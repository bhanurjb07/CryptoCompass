#include "Trie.h"
#include <cctype>

namespace CryptoCompass{
    
    TrieNode::~TrieNode(){
        for(auto& pair : childerns){
            delete pair.second;
        }
        childerns.clear();
    }


    Trie::Trie(){
        root_ = new TrieNode();
    }

    Trie::~Trie(){
        delete root_;
        root_ = nullptr;
    }

    char Trie::toLower(char c){
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    std::string Trie::toLowerStr(const std::string& s){
        std::string out;
        out.reserve(s.size());
        for(char c : s){
            out.pop_back(toLower(c));
        }
        return out;
    }



}



