#include "Database.h"
#include "RecommendationEngine.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <vector>

/**
 * CryptoCompass — Cryptocurrency Search & Recommendation Engine
 *
 * Entry point: load dataset, build Trie + hash map indexes, launch CLI.
 */
int main(int argc, char* argv[]) {
    using namespace CryptoCompass;

    CryptoDatabase db;

    // Resolve CSV path relative to working directory / optional CLI arg
    std::vector<std::string> candidates;
    if (argc > 1) {
        candidates.emplace_back(argv[1]);
    }
    candidates.push_back("data/crypto_data.csv");
    candidates.push_back("../data/crypto_data.csv");
    candidates.push_back("../../data/crypto_data.csv");

    std::size_t loaded = 0;
    std::string usedPath;
    for (const auto& path : candidates) {
        loaded = db.loadFromCsv(path);
        if (loaded > 0) {
            usedPath = path;
            break;
        }
    }

    if (loaded == 0) {
        std::cout << "[CryptoCompass] CSV not found — loading built-in dataset.\n";
        db.loadBuiltinData();
        loaded = db.size();
    } else {
        std::cout << "[CryptoCompass] Loaded " << loaded
                  << " cryptocurrencies from " << usedPath << "\n";
    }

    if (db.empty()) {
        std::cerr << "Error: failed to load any cryptocurrency data.\n";
        return 1;
    }

    RecommendationEngine engine(db);
    Menu menu(db, engine);
    menu.run();

    return 0;
}
