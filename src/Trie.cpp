#include "Trie.h"
#include <cctype>

namespace CryptoCompass {

TrieNode::~TrieNode() {
    for (auto& pair : children) {
        delete pair.second;
    }

    children.clear();
}

Trie::Trie() : root_(new TrieNode()) {}

Trie::~Trie() {
    delete root_;
    root_ = nullptr;
}

char Trie::toLower(char c) {
    return static_cast<char>(
        std::tolower(static_cast<unsigned char>(c))
    );
}

std::string Trie::toLowerStr(const std::string& s) {
    std::string out;
    out.reserve(s.size());

    for (char c : s) {
        out.push_back(toLower(c));
    }

    return out;
}

void Trie::insert(const std::string& word) {
    if (word.empty()) {
        return;
    }

    TrieNode* node = root_;
    const std::string key = toLowerStr(word);

    for (char c : key) {
        if (node->children.find(c) == node->children.end()) {
            node->children[c] = new TrieNode();
        }

        node = node->children[c];
    }

    node->isEndOfWord = true;
    node->cryptoName = word;
}

bool Trie::searchPrefix(const std::string& prefix) const {
    if (prefix.empty()) {
        return false;
    }

    const TrieNode* node = root_;
    const std::string key = toLowerStr(prefix);

    for (char c : key) {
        auto it = node->children.find(c);

        if (it == node->children.end()) {
            return false;
        }

        node = it->second;
    }

    return true;
}

std::vector<std::string> Trie::autocomplete(
    const std::string& prefix
) const {
    std::vector<std::string> results;

    if (prefix.empty()) {
        return results;
    }

    const TrieNode* node = root_;
    const std::string key = toLowerStr(prefix);

    for (char c : key) {
        auto it = node->children.find(c);

        if (it == node->children.end()) {
            return results;
        }

        node = it->second;
    }

    collectNames(node, results);

    return results;
}

void Trie::collectNames(
    const TrieNode* node,
    std::vector<std::string>& results
) const {
    if (node == nullptr) {
        return;
    }

    if (node->isEndOfWord) {
        results.push_back(node->cryptoName);
    }

    for (const auto& pair : node->children) {
        collectNames(pair.second, results);
    }
}

}