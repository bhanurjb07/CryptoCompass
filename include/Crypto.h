#ifndef CRYPTOCOMPASS_CRYPTO_H
#define CRYPTOCOMPASS_CRYPTO_H
#include <string>
#include <iostream>
#include <iomanip>

namespace CryptoCompass{

class Crypto{
public:

    Crypto() = default;

    Crypto(std::string name,
           std::string symbol,
           double marketCap,
           double currentPrice,
           double volume,
           std::string sector,
           std::string blockchain,
           std::string consensus,
           std::string riskLevel,
           double volatility,
           int launchYear,
           std::string category,
           int popularityScore,
           double recommendationScore,
           std::string description
        );

    const std::string& getName() const;
    const std::string& getSymbol() const;
    double getMarketCap() const;
    double getCurrentPrice() const;
    double getVolume() const;
    const std::string& getSector() const;
    const std::string& getBlockchain() const;
    const std::string& getConsensus() const;
    const std::string& getRiskLevel() const;
    double getVolatility() const;
    int getLaunchYear() const;
    const std::string& getCategory() const;
    int getPopularityScore() const;
    double getRecommendationScore() const;
    const std::string& getDescription() const;

    void setRecommendationScore(double score);
    void setPopularityScore(int score);

    std::string marketCapLabel() const;

    void displayDetails() const;

    void displaySummaryRow(int rank = 0) const;

private:
    std::string name_;
    std::string symbol_;
    double marketCap_ = 0.0;         
    double currentPrice_ = 0.0;       
    double volume_ = 0.0;             
    std::string sector_;
    std::string blockchain_;
    std::string consensus_;
    std::string riskLevel_;           
    double volatility_ = 0.0;         
    int launchYear_ = 0;
    std::string category_;            
    int popularityScore_ = 0;         
    double recommendationScore_ = 0.0;
    std::string description_;
};

} 


#endif 