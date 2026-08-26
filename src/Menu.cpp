#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace CryptoCompass {

Menu::Menu(CryptoDatabase& db, RecommendationEngine& engine)
    : db_(db), engine_(engine) {}

void Menu::printDivider() const {
    std::cout << "=========================================\n";
}

void Menu::showBanner() const {
    std::cout << "\n";
    printDivider();
    std::cout << "           CRYPTOCOMPASS\n";
    std::cout << "   Crypto Search & Recommendation\n";
    printDivider();
}

void Menu::showMainMenu() const {
    showBanner();
    std::cout << "\n";
    std::cout << "  1. Search Crypto\n";
    std::cout << "  2. Autocomplete Search\n";
    std::cout << "  3. View All Coins\n";
    std::cout << "  4. Recommend Coins\n";
    std::cout << "  5. Search by Category\n";
    std::cout << "  6. Search by Risk\n";
    std::cout << "  7. Search by Blockchain\n";
    std::cout << "  8. Top Ranked Coins\n";
    std::cout << "  9. Exit\n";
    std::cout << "\n";
    printDivider();
}

std::string Menu::readLine(const std::string& prompt) const {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    // Trim
    auto start = line.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = line.find_last_not_of(" \t\r\n");
    return line.substr(start, end - start + 1);
}

int Menu::readInt(const std::string& prompt, int min, int max) const {
    while (true) {
        std::string line = readLine(prompt);
        try {
            int value = std::stoi(line);
            if (value >= min && value <= max) return value;
        } catch (...) {
            // fall through
        }
        std::cout << "  Invalid input. Enter a number between "
                  << min << " and " << max << ".\n";
    }
}

int Menu::readMenuChoice(int min, int max) const {
    return readInt("\nEnter choice: ", min, max);
}

void Menu::printTableHeader() const {
    std::cout << std::left
              << std::setw(4)  << "#"
              << std::setw(18) << "Name"
              << std::setw(8)  << "Symbol"
              << std::setw(12) << "Category"
              << std::setw(10) << "Risk"
              << std::setw(10) << "MCap"
              << std::setw(10) << "Score"
              << "\n";
    std::cout << std::string(72, '-') << "\n";
}

void Menu::handleSearch() {
    printDivider();
    std::cout << "         SEARCH CRYPTO (Trie)\n";
    printDivider();

    std::string prefix = readLine("\nSearch prefix: ");
    if (prefix.empty()) {
        std::cout << "  Empty search. Returning to menu.\n";
        return;
    }

    auto results = db_.searchByPrefix(prefix);
    if (results.empty()) {
        // Also try exact name / symbol
        if (const Crypto* byName = db_.findByName(prefix)) {
            std::cout << "\nFound:\n";
            byName->displayDetails();
            return;
        }
        if (const Crypto* bySym = db_.findBySymbol(prefix)) {
            std::cout << "\nFound:\n";
            bySym->displayDetails();
            return;
        }
        std::cout << "\nNo cryptocurrencies found for \"" << prefix << "\".\n";
        return;
    }

    std::cout << "\nFound " << results.size() << " match(es):\n\n";
    printTableHeader();
    int rank = 1;
    for (const Crypto* c : results) {
        c->displaySummaryRow(rank++);
    }

    if (results.size() == 1) {
        results.front()->displayDetails();
    } else {
        std::string detail = readLine("\nEnter full name for details (or blank to skip): ");
        if (!detail.empty()) {
            if (const Crypto* c = db_.findByName(detail)) {
                c->displayDetails();
            } else {
                std::cout << "  Coin not found.\n";
            }
        }
    }
}

void Menu::handleAutocomplete() {
    printDivider();
    std::cout << "       AUTOCOMPLETE SEARCH\n";
    printDivider();

    std::string prefix = readLine("\nType prefix: ");
    if (prefix.empty()) {
        std::cout << "  Empty prefix.\n";
        return;
    }

    auto suggestions = db_.autocomplete(prefix);
    if (suggestions.empty()) {
        std::cout << "\nNo suggestions for \"" << prefix << "\".\n";
        return;
    }

    std::cout << "\nSuggestions:\n";
    for (std::size_t i = 0; i < suggestions.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << suggestions[i] << "\n";
    }

    std::string pick = readLine("\nSelect number or type name (blank to skip): ");
    if (pick.empty()) return;

    const Crypto* coin = nullptr;
    try {
        int idx = std::stoi(pick);
        if (idx >= 1 && idx <= static_cast<int>(suggestions.size())) {
            coin = db_.findByName(suggestions[static_cast<std::size_t>(idx - 1)]);
        }
    } catch (...) {
        coin = db_.findByName(pick);
    }

    if (coin) {
        coin->displayDetails();
    } else {
        std::cout << "  Invalid selection.\n";
    }
}

void Menu::handleViewAll() {
    printDivider();
    std::cout << "           ALL COINS\n";
    printDivider();
    std::cout << "\nTotal: " << db_.size() << " cryptocurrencies\n\n";
    printTableHeader();
    int rank = 1;
    for (const Crypto* c : db_.getAll()) {
        c->displaySummaryRow(rank++);
    }
}

void Menu::handleRecommend() {
    printDivider();
    std::cout << "      RECOMMENDATION ENGINE\n";
    printDivider();

    RecommendationFilters filters;

    std::cout << "\nRisk Level:\n";
    std::cout << "  1. Low   2. Medium   3. High   4. Any\n";
    int riskChoice = readInt("Select: ", 1, 4);
    if (riskChoice == 1) filters.riskLevel = "Low";
    else if (riskChoice == 2) filters.riskLevel = "Medium";
    else if (riskChoice == 3) filters.riskLevel = "High";

    std::cout << "\nCategory:\n";
    std::cout << "  1. Layer1   2. DeFi   3. Meme\n";
    std::cout << "  4. Exchange 5. Privacy 6. AI   7. Any\n";
    int catChoice = readInt("Select: ", 1, 7);
    const char* categories[] = {"", "Layer1", "DeFi", "Meme", "Exchange", "Privacy", "AI"};
    if (catChoice >= 1 && catChoice <= 6) {
        filters.category = categories[catChoice];
    }

    std::cout << "\nConsensus:\n";
    std::cout << "  1. PoW   2. PoS   3. Any\n";
    int consChoice = readInt("Select: ", 1, 3);
    if (consChoice == 1) filters.consensus = "PoW";
    else if (consChoice == 2) filters.consensus = "PoS";

    filters.sector = readLine("\nSector keyword (blank for any): ");

    std::cout << "\nMinimum popularity (0-100, 0 = none): ";
    filters.minPopularity = readInt("", 0, 100);

    std::cout << "\nMinimum market cap band:\n";
    std::cout << "  1. Any   2. $1B+   3. $10B+   4. $50B+\n";
    int mcapChoice = readInt("Select: ", 1, 4);
    if (mcapChoice == 2) filters.minMarketCap = 1e9;
    else if (mcapChoice == 3) filters.minMarketCap = 10e9;
    else if (mcapChoice == 4) filters.minMarketCap = 50e9;

    int topN = readInt("\nHow many recommendations (1-20): ", 1, 20);

    auto results = engine_.recommend(filters, static_cast<std::size_t>(topN));

    std::cout << "\n";
    printDivider();
    std::cout << "     TOP RECOMMENDATIONS\n";
    printDivider();

    if (results.empty()) {
        std::cout << "\nNo coins matched your filters.\n";
        return;
    }

    std::cout << "\nScoring: 30% Risk + 25% Category + 20% Market Cap\n"
              << "         + 15% Popularity + 10% Volatility\n\n";

    printTableHeader();
    int rank = 1;
    for (const auto& coin : results) {
        coin.displaySummaryRow(rank++);
    }

    std::cout << "\nTop pick details:\n";
    results.front().displayDetails();
}

void Menu::handleSearchByCategory() {
    printDivider();
    std::cout << "       SEARCH BY CATEGORY\n";
    printDivider();
    std::cout << "\nCategories: Layer1, DeFi, Meme, Exchange, Privacy, AI\n";
    std::string cat = readLine("Enter category: ");
    if (cat.empty()) return;

    auto results = db_.filterByCategory(cat);
    if (results.empty()) {
        std::cout << "\nNo coins in category \"" << cat << "\".\n";
        return;
    }

    std::cout << "\nFound " << results.size() << " coin(s):\n\n";
    printTableHeader();
    int rank = 1;
    for (const Crypto* c : results) {
        c->displaySummaryRow(rank++);
    }
}

void Menu::handleSearchByRisk() {
    printDivider();
    std::cout << "         SEARCH BY RISK\n";
    printDivider();
    std::cout << "\nRisk levels: Low, Medium, High\n";
    std::string risk = readLine("Enter risk level: ");
    if (risk.empty()) return;

    auto results = db_.filterByRisk(risk);
    if (results.empty()) {
        std::cout << "\nNo coins with risk \"" << risk << "\".\n";
        return;
    }

    std::cout << "\nFound " << results.size() << " coin(s):\n\n";
    printTableHeader();
    int rank = 1;
    for (const Crypto* c : results) {
        c->displaySummaryRow(rank++);
    }
}

void Menu::handleSearchByBlockchain() {
    printDivider();
    std::cout << "      SEARCH BY BLOCKCHAIN\n";
    printDivider();
    std::cout << "\nExamples: Ethereum, Solana, Bitcoin, Avalanche\n";
    std::string chain = readLine("Enter blockchain: ");
    if (chain.empty()) return;

    auto results = db_.filterByBlockchain(chain);
    if (results.empty()) {
        std::cout << "\nNo coins on blockchain \"" << chain << "\".\n";
        return;
    }

    std::cout << "\nFound " << results.size() << " coin(s):\n\n";
    printTableHeader();
    int rank = 1;
    for (const Crypto* c : results) {
        c->displaySummaryRow(rank++);
    }
}

void Menu::handleTopRanked() {
    printDivider();
    std::cout << "        TOP RANKED COINS\n";
    printDivider();

    int topN = readInt("\nHow many top coins (1-22): ", 1, 22);
    auto results = engine_.topRanked(static_cast<std::size_t>(topN));

    std::cout << "\n";
    printTableHeader();
    int rank = 1;
    for (const Crypto* c : results) {
        c->displaySummaryRow(rank++);
    }

    if (!results.empty()) {
        std::cout << "\n#1 details:\n";
        results.front()->displayDetails();
    }
}

void Menu::run() {
    bool running = true;
    while (running) {
        showMainMenu();
        int choice = readMenuChoice(1, 9);

        switch (choice) {
            case 1: handleSearch(); break;
            case 2: handleAutocomplete(); break;
            case 3: handleViewAll(); break;
            case 4: handleRecommend(); break;
            case 5: handleSearchByCategory(); break;
            case 6: handleSearchByRisk(); break;
            case 7: handleSearchByBlockchain(); break;
            case 8: handleTopRanked(); break;
            case 9:
                std::cout << "\nThank you for using CryptoCompass. Goodbye!\n";
                running = false;
                break;
            default:
                break;
        }

        if (running) {
            std::cout << "\nPress Enter to return to menu...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }
}

}
