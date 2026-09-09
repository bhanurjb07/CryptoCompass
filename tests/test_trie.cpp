#include <gtest/gtest.h>
#include "Trie.h"

using namespace CryptoCompass;
TEST(TrieTest, InsertAndSearchPrefix){
    Trie trie;
    trie.insert("Bitcoin");
    trie.insert("Binance");
    trie.insert("Bitget");

    EXPECT_TRUE(trie.searchPrefix("Bit"));
    EXPECT_TRUE(trie.searchPrefix("Bin"));
    EXPECT_FALSE(trie.searchPrefix("Dog"));
}

TEST(TrieTest, Autocomplete){
    Trie trie;
    trie.insert("Bitcoin");
    trie.insert("Bitget");
    trie.insert("Binance");

    auto results = trie.autocomplete("Bit");
    EXPECT_EQ(results.size(), 2);
}

TEST(TrieTest, EmptyPrefix){
    Trie trie;
    trie.insert("Bitcoin");

    auto results = trie.autocomplete("");
    EXPECT_FALSE(results.empty());
}
