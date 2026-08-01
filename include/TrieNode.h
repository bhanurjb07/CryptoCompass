#ifndef CRYPTOCOMPASS_TRIENODE_H
#define CRYPTOCOMPASS_TRIENODE_H

#include<string>
#include<unordered_map>

namespace CryptoCompass{

    struct TrieNode{
        std::unordered_map<char, TrieNode*>childerns;
        bool isEndofWord= false;
        std::string crytoName;

        TrieNode()= default;
        ~TrieNode();

        ///Non-copyable: owns child pointers
        TrieNode(const TrieNode&)=delete;
        TrieNode& operator= (const TrieNode&)= delete;
    };
    
}

#endif