#ifndef CRYPTOCOMPASS_DATABASE_H
#define CRYPTOCOMPASS_DATABASE_H

#include "Crypto.h"
#include "Trie.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace CryptoCompass {
class CryptoDatabase {
public:
    CryptoDatabase() = default;

    //Load cryptocurrencies from a CSV file. Returns count loaded
    std::size_t loadFromCsv(const std::string& filepath);

    // Load a built-in fallback dataset (used if CSV is missing)
    void loadBuiltinData();

    bool empty() const;
    std::size_t size() const;

    // O(1) average lookup by name (case-insensitive)
    const Crypto* findByName(const std::string& name) const;
    Crypto* findByNameMutable(const std::string& name);

    ////Exact symbol lookup (case-insensitive
    const Crypto* findBySymbol(const std::string& symbol) const;

    /////Prefix search via Trie; returns matching Crypto pointer
    std::vector<const Crypto*> searchByPrefix(const std::string& prefix) const;

    //Autocomplete name suggestions via Trie
    std::vector<std::string> autocomplete(const std::string& prefix) const;

    //all coins as const pointers (stable order by insertion key)
    std::vector<const Crypto*> getAll() const;

    std::vector<const Crypto*> filterByCategory(const std::string& category) const;
    std::vector<const Crypto*> filterByRisk(const std::string& risk) const;
    std::vector<const Crypto*> filterByBlockchain(const std::string& blockchain) const;

    std::vector<Crypto*> getAllMutable();

private:
    std::unordered_map<std::string, Crypto> coins_; 
    Trie nameTrie_;

    void insertCoin(Crypto coin);
    static std::string toLower(const std::string& s);
    static std::string trim(const std::string& s);
    static std::vector<std::string> splitCsvLine(const std::string& line);
};

}
#endif
