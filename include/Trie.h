#ifndef CRYPTOCOMPASS_TRIE_H
#define CRYPTOCOMPASS_TRIE_H

#include "TrieNode.h"
#include <string>
#include <vector>

namespace CryptoCompass{

class Trie{
public:
    Trie();
    ~Trie();

    Trie(const Trie&) =delete;
    Trie& operator=(const Trie&)=delete;

    //Insert a cryptocurrency name into the Trie
    void insert(const std::string& word);

    //Return true if any word starts with the given prefix
    bool searchPrefix(const std::string& prefix)const;

    //Return all crypto names matching the prefix
    std::vector<std::string>autocomplete(const std::string& prefix)const;

private:
    TrieNode* root_;
    
    void collectNames(const TrieNode* node, std::vector<std::string>&results)const;

    static char toLower(char c);
    static std::string toLowerStr(const std::string& s);
};
    
}


#endif