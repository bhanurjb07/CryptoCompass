#ifndef CRYPTOCOMPASS_RECOMMENDATION_ENGINE_H
#define CRYPTOCOMPASS_RECOMMENDATION_ENGINE_H

#include "Crypto.h"
#include "Database.h"
#include <string>
#include <vector>

namespace CryptoCompass {
struct RecommendationFilters {
    std::string riskLevel;    // Low / Medium / High (empty = ignore)
    std::string category;     // Layer1 / DeFi / Meme /(empty = ignore)
    std::string consensus;    // PoW / PoS (empty = ignore)
    std::string sector;       // optional sector string
    int minPopularity = 0;    // 0–100
    double minMarketCap = 0;  // USD; 0 = no floor
};

/*
     formula.
 * Score =
 *   30% Risk Match +
 *   25% Category Match +
 *   20% Market Cap +
 *   15% Popularity +
 *   10% Volatility (lower volatility scores higher)
 *
 * Ranking complexity: O(n log n)
 */
class RecommendationEngine {
public:
    explicit RecommendationEngine(CryptoDatabase& db);

    // Score all coins against filters, sort descending by recommendation score,
    // and return the top results    
    std::vector<Crypto> recommend(const RecommendationFilters& filters,
                                  std::size_t topN = 0) const;

    ////Top coins by existing recommendationScore / popularity
    std::vector<const Crypto*> topRanked(std::size_t topN = 10) const;

    ///Compute a single coin's score against filters (0–100
    double calculateScore(const Crypto& coin,
                          const RecommendationFilters& filters) const;

private:
    CryptoDatabase& db_;

    static double riskMatchScore(const std::string& coinRisk,
                                 const std::string& preferredRisk);
    static double categoryMatchScore(const std::string& coinCategory,
                                     const std::string& preferredCategory);
    static double marketCapScore(double marketCap);
    static double popularityScore(int popularity);
    static double volatilityScore(double volatility);
    static double consensusBonus(const std::string& coinConsensus,
                                 const std::string& preferredConsensus);
    static double sectorBonus(const std::string& coinSector,
                              const std::string& preferredSector);
    static bool equalsIgnoreCase(const std::string& a, const std::string& b);
};

}
#endif
