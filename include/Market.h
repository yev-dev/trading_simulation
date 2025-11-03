#ifndef MARKET_H
#define MARKET_H

#include <string>
#include <map>
#include <vector>
#include <random>
#include <chrono>

struct PriceData {
    double price;
    double volume;
    std::chrono::system_clock::time_point timestamp;
    
    PriceData(double p = 0.0, double v = 0.0)
        : price(p), volume(v), timestamp(std::chrono::system_clock::now()) {}
};

class Market {
private:
    std::map<std::string, std::vector<PriceData>> priceHistory;
    std::map<std::string, double> currentPrices;
    std::map<std::string, double> volatility;
    std::mt19937 randomGenerator;
    std::normal_distribution<double> normalDist;

public:
    Market();
    
    // Market data management
    void addSymbol(const std::string& symbol, double initialPrice, double vol = 0.02);
    void updatePrices();
    void simulatePriceMovement(const std::string& symbol);
    
    // Price queries
    double getCurrentPrice(const std::string& symbol) const;
    bool hasSymbol(const std::string& symbol) const;
    const std::vector<PriceData>& getPriceHistory(const std::string& symbol) const;
    
    // Market statistics
    double getDailyReturn(const std::string& symbol) const;
    double getVolatility(const std::string& symbol) const;
    void setVolatility(const std::string& symbol, double vol);
    
    // Utility methods
    std::vector<std::string> getAvailableSymbols() const;
    void printMarketSummary() const;
    
private:
    void recordPrice(const std::string& symbol, double price, double volume = 1000.0);
};

#endif // MARKET_H
