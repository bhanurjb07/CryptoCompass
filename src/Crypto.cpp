#include "Crypto.h"
#include<sstream>

namespace CryptoCompass{

    Crypto::Crypto(std::string name,
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
            )
    : name_(name),
      symbol_(symbol),
      marketCap_(marketCap),
      currentPrice_(currentPrice),
      volume_(volume),
      sector_(sector),
      blockchain_(blockchain),
      consensus_(consensus),
      riskLevel_(riskLevel),
      volatility_(volatility),
      launchYear_(launchYear),
      category_(category),
      popularityScore_(popularityScore),
      recommendationScore_(recommendationScore),
      description_(description) 
      {}


    const std::string& Crypto::getName()const{
        return name_;
    }

    const std::string& Crypto::getSymbol()const{
        return symbol_;
    }

    double Crypto::getMarketCap()const{
        return marketCap_;
    }

    double Crypto::getCurrentPrice() const{
        return currentPrice_;
    }

    double Crypto::getVolume() const{
        return volume_;
    }

    const std::string& Crypto::getSector() const{
        return sector_;
    }

    const std::string& Crypto::getBlockchain() const{
        return blockchain_;
    }

    const std::string& Crypto::getConsensus() const{
        return consensus_;
    }

    const std::string& Crypto::getRiskLevel() const{
        return riskLevel_;
    }

    double Crypto::getVolatility() const{
        return volatility_;
    }

    int Crypto::getLaunchYear() const{
        return launchYear_;
    }

    const std::string& Crypto::getCategory() const{
        return category_;
    }

    int Crypto::getPopularityScore() const{
        return popularityScore_;
    }

    double Crypto::getRecommendationScore() const{
        return recommendationScore_;
    }

    const std::string& Crypto::getDescription() const{
        return description_;
    }

    void Crypto::setRecommendationScore(double score){
        recommendationScore_ = score;
    }

    void Crypto::setPopularityScore(int score){
        popularityScore_ = score;
    }
    
    
}