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
    
    std::string Crypto::marketCapLabel()const{
        if(marketCap_ >= 100e9)return "Very High";
        if(marketCap_ >= 20e9)return "High";
        if(marketCap_ >= 5e9)return "Medium";
        if(marketCap_ >= 1e9)return "Low-medium";
        return "Low";
    }

    void Crypto::displayDetails()const{
        std::cout<<"\n----------------------------------------------\n";
        std::cout<<" "<<name_ << " (" << symbol_ << "  )\n ";
        std::cout<<"\n----------------------------------------------\n";
        std::cout<<"Market Cap          : "<< marketCapLabel()
                 <<" ($" <<std::fixed << std:: setprecision(2)
                 <<(marketCap_ / 1e9)<< "B )\n";

        std::cout<<"  Current Price        : $" <<std::fixed <<std::setprecision(4)
                 <<currentPrice_ << "\n";

        std::cout<<"  24h Volume           : $" <<std::fixed <<std::setprecision(2)
                 <<(volume_ / 1e9) << "B\n";

        std::cout <<"  Sector               : " << sector_ <<"\n";
        std::cout <<"  Blockchain           : " << blockchain_ << "\n";
        std::cout<<"  Consensus            : " << consensus_<< "\n";
        std::cout<<"  Risk Level           : " << riskLevel_<< "\n";
        std::cout<<"  Volatility           : " << std::fixed<<std::setprecision(1)
                  << volatility_ << "\n";

        std::cout <<"  Launch Year          : " << launchYear_ << "\n";
        std::cout <<"  Category             : " << category_ << "\n";
        std::cout <<"  Popularity Score     : " << popularityScore_ << "/100\n";
        std::cout <<"  Recommendation Score : " << std::fixed << std::setprecision(1)
                  << recommendationScore_ << "/100\n";

        std::cout << "  Description          : " << description_ << "\n";
        std::cout << "-----------------------------------------\n";
    }

    void Crypto::displaySummaryRow(int rank)const{
        if(rank >0){
            std::cout<<std::left <<std::setw(4) <<rank;
        }else{
            std::cout <<std::left <<std::setw(4)<<"-";
        }

        std::cout <<std::left <<std::setw(18)<<name_
                  <<std::setw(8) <<symbol_
                  <<std::setw(12)<<category_
                  <<std::setw(10)<<riskLevel_
                  <<std::setw(10)<<std::fixed << std::setprecision(1)
                  <<(marketCap_ / 1e9) <<"B"
                  <<std::setw(10)<<recommendationScore_
                  <<std::endl;
    }
}