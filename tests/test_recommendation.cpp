#include <gtest/gtest.h>
#include "Database.h"
#include "RecommendationEngine.h"

using namespace CryptoCompass;
TEST(RecommendationTest, ReturnsRecommendations){
    CryptoDatabase db;
    db.loadBuiltinData();

    RecommendationEngine engine(db);
    RecommendationFilters filters;

    auto results = engine.recommend(filters, 5);
    EXPECT_FALSE(results.empty());
}

TEST(RecommendationTest, RespectsTopN){
    CryptoDatabase db;
    db.loadBuiltinData();

    RecommendationEngine engine(db);
    RecommendationFilters filters;

    auto results = engine.recommend(filters, 3);
    EXPECT_LE(results.size(), 3);
}

TEST(RecommendationTest, MarketCapFilter){
    CryptoDatabase db;
    db.loadBuiltinData();

    RecommendationEngine engine(db);
    RecommendationFilters filters;
    filters.minMarketCap = 10000000000.0;

    auto results = engine.recommend(filters, 10);

    for (const auto& result : results){
        EXPECT_GE(result.getMarketCap(), 10000000000.0);
    }
}
