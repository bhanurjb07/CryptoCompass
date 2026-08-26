#include "Database.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace CryptoCompass {

std::string CryptoDatabase::toLower(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return out;
}

std::string CryptoDatabase::trim(const std::string& s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }
    return s.substr(start, end - start);
}

std::vector<std::string> CryptoDatabase::splitCsvLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field.push_back(c);
        }
    }
    fields.push_back(trim(field));
    return fields;
}

void CryptoDatabase::insertCoin(Crypto coin) {
    const std::string key = toLower(coin.getName());
    nameTrie_.insert(coin.getName());
    coins_[key] = std::move(coin);
}

std::size_t CryptoDatabase::loadFromCsv(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return 0;
    }

    std::string line;
    bool first = true;
    std::size_t count = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (first) {
            first = false;  // skip header
            continue;
        }

        auto fields = splitCsvLine(line);
        // Expected: 15 columns matching Crypto attributes
        if (fields.size() < 15) continue;

        try {
            Crypto coin(
                fields[0],                          // name
                fields[1],                          // symbol
                std::stod(fields[2]),               // marketCap
                std::stod(fields[3]),               // currentPrice
                std::stod(fields[4]),               // volume
                fields[5],                          // sector
                fields[6],                          // blockchain
                fields[7],                          // consensus
                fields[8],                          // riskLevel
                std::stod(fields[9]),               // volatility
                std::stoi(fields[10]),              // launchYear
                fields[11],                         // category
                std::stoi(fields[12]),              // popularityScore
                std::stod(fields[13]),              // recommendationScore
                fields[14]                          //// description
            );
            insertCoin(std::move(coin));
            ++count;
        } catch (const std::exception&) {
            // Skip malformed rows
            continue;
        }
    }
    return count;
}

void CryptoDatabase::loadBuiltinData() {
    // Fallback dataset — mirrors data/crypto_data.csv
    struct Row {
        const char* name;
        const char* symbol;
        double marketCap;
        double price;
        double volume;
        const char* sector;
        const char* blockchain;
        const char* consensus;
        const char* risk;
        double volatility;
        int year;
        const char* category;
        int popularity;
        double recScore;
        const char* description;
    };

    const Row rows[] = {
        {"Bitcoin", "BTC", 1.3e12, 67000.0, 28e9, "Store of Value", "Bitcoin", "PoW", "Medium", 45.0, 2009, "Layer1", 100, 96.0, "The original cryptocurrency and digital gold"},
        {"Ethereum", "ETH", 400e9, 3400.0, 15e9, "Smart Contracts", "Ethereum", "PoS", "Medium", 55.0, 2015, "Layer1", 98, 94.0, "Leading smart contract platform"},
        {"Solana", "SOL", 70e9, 150.0, 3.5e9, "Smart Contracts", "Solana", "PoS", "High", 75.0, 2020, "Layer1", 88, 85.0, "High-throughput Layer1 blockchain"},
        {"Cardano", "ADA", 18e9, 0.45, 0.6e9, "Smart Contracts", "Cardano", "PoS", "Medium", 50.0, 2017, "Layer1", 75, 80.0, "Research-driven proof-of-stake blockchain"},
        {"Dogecoin", "DOGE", 20e9, 0.14, 1.2e9, "Payments", "Dogecoin", "PoW", "High", 80.0, 2013, "Meme", 82, 70.0, "Popular meme coin with strong community"},
        {"Ripple", "XRP", 35e9, 0.62, 1.8e9, "Payments", "XRP Ledger", "PoS", "Medium", 48.0, 2012, "Layer1", 80, 78.0, "Cross-border payments focused ledger"},
        {"Polygon", "MATIC", 8e9, 0.85, 0.4e9, "Scaling", "Polygon", "PoS", "Medium", 60.0, 2019, "Layer1", 72, 76.0, "Ethereum scaling and infrastructure"},
        {"Polkadot", "DOT", 10e9, 7.2, 0.35e9, "Interoperability", "Polkadot", "PoS", "Medium", 58.0, 2020, "Layer1", 70, 74.0, "Multi-chain interoperability network"},
        {"Litecoin", "LTC", 6e9, 85.0, 0.5e9, "Payments", "Litecoin", "PoW", "Low", 40.0, 2011, "Layer1", 68, 72.0, "Silver to Bitcoin gold payments coin"},
        {"Avalanche", "AVAX", 12e9, 35.0, 0.55e9, "Smart Contracts", "Avalanche", "PoS", "High", 70.0, 2020, "Layer1", 74, 77.0, "Fast finality smart contract platform"},
        {"Chainlink", "LINK", 9e9, 14.5, 0.45e9, "Oracles", "Ethereum", "PoS", "Medium", 55.0, 2017, "DeFi", 78, 81.0, "Decentralized oracle network"},
        {"BNB", "BNB", 90e9, 580.0, 1.5e9, "Exchange", "BNB Chain", "PoS", "Medium", 52.0, 2017, "Exchange", 90, 88.0, "Binance ecosystem utility token"},
        {"TRON", "TRX", 12e9, 0.12, 0.7e9, "Entertainment", "TRON", "PoS", "Medium", 50.0, 2017, "Layer1", 65, 68.0, "High-throughput entertainment blockchain"},
        {"Uniswap", "UNI", 7e9, 9.5, 0.3e9, "DEX", "Ethereum", "PoS", "High", 68.0, 2020, "DeFi", 76, 79.0, "Leading decentralized exchange token"},
        {"Shiba Inu", "SHIB", 10e9, 0.000025, 0.8e9, "Payments", "Ethereum", "PoS", "High", 85.0, 2020, "Meme", 79, 65.0, "Ethereum-based meme token ecosystem"},
        {"Aptos", "APT", 5e9, 8.5, 0.25e9, "Smart Contracts", "Aptos", "PoS", "High", 72.0, 2022, "Layer1", 60, 71.0, "Move-based high-performance Layer1"},
        {"Near Protocol", "NEAR", 6e9, 5.8, 0.28e9, "Smart Contracts", "NEAR", "PoS", "Medium", 62.0, 2020, "Layer1", 64, 73.0, "Developer-friendly sharded blockchain"},
        {"Cosmos", "ATOM", 4e9, 9.0, 0.22e9, "Interoperability", "Cosmos", "PoS", "Medium", 58.0, 2019, "Layer1", 66, 72.0, "Internet of blockchains hub"},
        {"Monero", "XMR", 3e9, 160.0, 0.15e9, "Privacy", "Monero", "PoW", "High", 55.0, 2014, "Privacy", 58, 69.0, "Privacy-focused fungible cryptocurrency"},
        {"Arbitrum", "ARB", 3.5e9, 1.1, 0.4e9, "Scaling", "Arbitrum", "PoS", "Medium", 65.0, 2023, "Layer1", 70, 75.0, "Leading Ethereum Layer2 optimistic rollup"},
        {"Aave", "AAVE", 2.5e9, 95.0, 0.18e9, "Lending", "Ethereum", "PoS", "Medium", 60.0, 2020, "DeFi", 67, 74.0, "Decentralized lending and borrowing protocol"},
        {"Render", "RNDR", 2.8e9, 7.5, 0.2e9, "Compute", "Ethereum", "PoS", "High", 70.0, 2017, "AI", 62, 70.0, "Decentralized GPU rendering network"},
    };

    for (const auto& r : rows) {
        insertCoin(Crypto(r.name, r.symbol, r.marketCap, r.price, r.volume,
                          r.sector, r.blockchain, r.consensus, r.risk,
                          r.volatility, r.year, r.category, r.popularity,
                          r.recScore, r.description));
    }
}

bool CryptoDatabase::empty() const { return coins_.empty(); }
std::size_t CryptoDatabase::size() const { return coins_.size(); }

const Crypto* CryptoDatabase::findByName(const std::string& name) const {
    auto it = coins_.find(toLower(name));
    if (it == coins_.end()) return nullptr;
    return &it->second;
}

Crypto* CryptoDatabase::findByNameMutable(const std::string& name) {
    auto it = coins_.find(toLower(name));
    if (it == coins_.end()) return nullptr;
    return &it->second;
}

const Crypto* CryptoDatabase::findBySymbol(const std::string& symbol) const {
    const std::string target = toLower(symbol);
    for (const auto& pair : coins_) {
        if (toLower(pair.second.getSymbol()) == target) {
            return &pair.second;
        }
    }
    return nullptr;
}

std::vector<const Crypto*> CryptoDatabase::searchByPrefix(const std::string& prefix) const {
    std::vector<const Crypto*> results;
    auto names = nameTrie_.autocomplete(prefix);
    for (const auto& name : names) {
        if (const Crypto* c = findByName(name)) {
            results.push_back(c);
        }
    }
    return results;
}

std::vector<std::string> CryptoDatabase::autocomplete(const std::string& prefix) const {
    return nameTrie_.autocomplete(prefix);
}

std::vector<const Crypto*> CryptoDatabase::getAll() const {
    std::vector<const Crypto*> results;
    results.reserve(coins_.size());
    for (const auto& pair : coins_) {
        results.push_back(&pair.second);
    }
    std::sort(results.begin(), results.end(),
              [](const Crypto* a, const Crypto* b) {
                  return a->getName() < b->getName();
              });
    return results;
}

std::vector<Crypto*> CryptoDatabase::getAllMutable() {
    std::vector<Crypto*> results;
    results.reserve(coins_.size());
    for (auto& pair : coins_) {
        results.push_back(&pair.second);
    }
    return results;
}

std::vector<const Crypto*> CryptoDatabase::filterByCategory(const std::string& category) const {
    const std::string key = toLower(category);
    std::vector<const Crypto*> results;
    for (const auto& pair : coins_) {
        if (toLower(pair.second.getCategory()) == key) {
            results.push_back(&pair.second);
        }
    }
    return results;
}

std::vector<const Crypto*> CryptoDatabase::filterByRisk(const std::string& risk) const {
    const std::string key = toLower(risk);
    std::vector<const Crypto*> results;
    for (const auto& pair : coins_) {
        if (toLower(pair.second.getRiskLevel()) == key) {
            results.push_back(&pair.second);
        }
    }
    return results;
}

std::vector<const Crypto*> CryptoDatabase::filterByBlockchain(const std::string& blockchain) const {
    const std::string key = toLower(blockchain);
    std::vector<const Crypto*> results;
    for (const auto& pair : coins_) {
        if (toLower(pair.second.getBlockchain()).find(key) != std::string::npos) {
            results.push_back(&pair.second);
        }
    }
    return results;
}

} // namespace CryptoCompass
