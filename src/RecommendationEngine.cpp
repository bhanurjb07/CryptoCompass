#include "RecommendationEngine.h"
#include <algorithm>
#include <cctype>
#include <queue>
#include <cmath>

namespace CryptoCompass {

RecommendationEngine::RecommendationEngine(CryptoDatabase& db) : db_(db) {}

bool RecommendationEngine::equalsIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

double RecommendationEngine::riskMatchScore(const std::string& coinRisk,
                                            const std::string& preferredRisk) {
    if (preferredRisk.empty()) return 70.0; 
    if (equalsIgnoreCase(coinRisk, preferredRisk)) return 100.0;

    auto rank = [](const std::string& r) -> int {
        if (equalsIgnoreCase(r, "Low")) return 0;
        if (equalsIgnoreCase(r, "Medium")) return 1;
        if (equalsIgnoreCase(r, "High")) return 2;
        return -1;
    };
    int a = rank(coinRisk);
    int b = rank(preferredRisk);
    if (a < 0 || b < 0) return 40.0;
    int dist = std::abs(a - b);
    if (dist == 1) return 55.0;
    return 25.0;
}

double RecommendationEngine::categoryMatchScore(const std::string& coinCategory,
                                                const std::string& preferredCategory) {
    if (preferredCategory.empty()) return 70.0;
    if (equalsIgnoreCase(coinCategory, preferredCategory)) return 100.0;
    return 30.0;
}

double RecommendationEngine::marketCapScore(double marketCap) {
    if (marketCap <= 0) return 0.0;
    const double score = (std::log10(marketCap) - 8.0) / 4.0 * 100.0;
    return std::max(0.0, std::min(100.0, score));
}

double RecommendationEngine::popularityScore(int popularity) {
    return std::max(0.0, std::min(100.0, static_cast<double>(popularity)));
}

double RecommendationEngine::volatilityScore(double volatility) {
    return std::max(0.0, std::min(100.0, 100.0 - volatility));
}

double RecommendationEngine::consensusBonus(const std::string& coinConsensus,
                                            const std::string& preferredConsensus) {
    if (preferredConsensus.empty()) return 0.0;
    return equalsIgnoreCase(coinConsensus, preferredConsensus) ? 5.0 : -3.0;
}

double RecommendationEngine::sectorBonus(const std::string& coinSector,
                                         const std::string& preferredSector) {
    if (preferredSector.empty()) return 0.0;
    // Substring match allows flexible sector filtering
    std::string a = coinSector;
    std::string b = preferredSector;
    for (char& c : a) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    for (char& c : b) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (a.find(b) != std::string::npos) return 5.0;
    return -2.0;
}

double RecommendationEngine::calculateScore(const Crypto& coin,
                                            const RecommendationFilters& filters) const {
    // Hard filters: exclude if below floors
    if (filters.minPopularity > 0 &&
        coin.getPopularityScore() < filters.minPopularity) {
        return -1.0;
    }
    if (filters.minMarketCap > 0 &&
        coin.getMarketCap() < filters.minMarketCap) {
        return -1.0;
    }

    /*
     * Weighted recommendation score ((formula):
     *   30% Risk Match + 25% Category Match + 20% Market Cap
     * + 15% Popularity + 10% Volatility
     * Soft bonuses for consensus / sector preferences (±5).
     */
    const double score =
        0.30 * riskMatchScore(coin.getRiskLevel(), filters.riskLevel) +
        0.25 * categoryMatchScore(coin.getCategory(), filters.category) +
        0.20 * marketCapScore(coin.getMarketCap()) +
        0.15 * popularityScore(coin.getPopularityScore()) +
        0.10 * volatilityScore(coin.getVolatility()) +
        consensusBonus(coin.getConsensus(), filters.consensus) +
        sectorBonus(coin.getSector(), filters.sector);

    return std::max(0.0, std::min(100.0, score));
}

std::vector<Crypto> RecommendationEngine::recommend(
    const RecommendationFilters& filters,
    std::size_t topN) const {

    // Max-heap by score for efficient top-N extraction
    using Scored = std::pair<double, const Crypto*>;
    auto cmp = [](const Scored& a, const Scored& b) {
        return a.first < b.first; 
    };
    std::priority_queue<Scored, std::vector<Scored>, decltype(cmp)> heap(cmp);

    for (const Crypto* coin : db_.getAll()) {
        const double score = calculateScore(*coin, filters);
        if (score < 0) continue; 
        heap.push({score, coin});
    }

    std::vector<Crypto> results;
    const std::size_t limit = (topN == 0) ? heap.size() : std::min(topN, heap.size());
    results.reserve(limit);

    while (!heap.empty() && results.size() < limit) {
        auto top = heap.top();
        heap.pop();
        Crypto copy = *top.second;
        copy.setRecommendationScore(top.first);
        results.push_back(std::move(copy));
    }
    // Results already in descending score order from priority_queue pops
    return results;
}

std::vector<const Crypto*> RecommendationEngine::topRanked(std::size_t topN) const {
    auto all = db_.getAll();
    ////O(n log n) sort by recommendation score descending
    std::sort(all.begin(), all.end(),
              [](const Crypto* a, const Crypto* b) {
                  if (a->getRecommendationScore() != b->getRecommendationScore()) {
                      return a->getRecommendationScore() > b->getRecommendationScore();
                  }
                  return a->getPopularityScore() > b->getPopularityScore();
              });
    if (topN > 0 && all.size() > topN) {
        all.resize(topN);
    }
    return all;
}
}
