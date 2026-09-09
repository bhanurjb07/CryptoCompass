#include <gtest/gtest.h>
#include "Database.h"

using namespace CryptoCompass;
TEST(DatabaseTest, FindByName){
    CryptoDatabase db;
    db.loadBuiltinData();

    const Crypto* crypto = db.findByName("Bitcoin");
    ASSERT_NE(crypto, nullptr);
    EXPECT_EQ(crypto->getName(), "Bitcoin");
}

TEST(DatabaseTest, FindBySymbol){
    CryptoDatabase db;
    db.loadBuiltinData();

    const Crypto* crypto = db.findBySymbol("BTC");
    ASSERT_NE(crypto, nullptr);
    EXPECT_EQ(crypto->getSymbol(), "BTC");
}

TEST(DatabaseTest, PrefixSearch){
    CryptoDatabase db;
    db.loadBuiltinData();

    auto results = db.searchByPrefix("Bit");
    EXPECT_FALSE(results.empty());
}

TEST(DatabaseTest, CategoryFilter){
    CryptoDatabase db;
    db.loadBuiltinData();

    auto results = db.filterByCategory("Layer1");
    EXPECT_FALSE(results.empty());
}

TEST(DatabaseTest, RiskFilter){
    CryptoDatabase db;
    db.loadBuiltinData();

    auto results = db.filterByRisk("Low");
    EXPECT_FALSE(results.empty());
}
