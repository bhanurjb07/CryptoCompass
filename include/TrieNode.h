#ifndef CRYPTOCOMPASS_TRIENODE_H
#define CRYPTOCOMPASS_TRIENODE_H

#include <string>
#include <unordered_map>

namespace CryptoCompass {

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord = false;
    std::string cryptoName;

    TrieNode() = default;
    ~TrieNode();

    TrieNode(const TrieNode&) = delete;
    TrieNode& operator=(const TrieNode&) = delete;
};

}

#endif