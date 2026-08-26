#ifndef CRYPTOCOMPASS_MENU_H
#define CRYPTOCOMPASS_MENU_H

#include "Database.h"
#include "RecommendationEngine.h"
#include <string>

namespace CryptoCompass {
class Menu {
public:
    Menu(CryptoDatabase& db, RecommendationEngine& engine);
    void run();

private:
    CryptoDatabase& db_;
    RecommendationEngine& engine_;

    void showBanner() const;
    void showMainMenu() const;
    int readMenuChoice(int min, int max) const;
    std::string readLine(const std::string& prompt) const;
    int readInt(const std::string& prompt, int min, int max) const;

    void handleSearch();
    void handleAutocomplete();
    void handleViewAll();
    void handleRecommend();
    void handleSearchByCategory();
    void handleSearchByRisk();
    void handleSearchByBlockchain();
    void handleTopRanked();

    void printTableHeader() const;
    void printDivider() const;
};

}
#endif
