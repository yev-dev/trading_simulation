#include "Market.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

Market::Market() : randomGenerator(std::chrono::system_clock::now().time_since_epoch().count()),
                   normalDist(0.0, 1.0) {}

void Market::addSymbol(const std::string& symbol, double initialPrice, double vol) {
    currentPrices[symbol] = initialPrice;
    volatility[symbol] = vol;
    priceHistory[symbol] = std::vector<PriceData>();
    recordPrice(symbol, initialPrice);
}

void Market::updatePrices() {
    for (auto& pair : currentPrices) {
        simulatePriceMovement(pair.first);
    }
}

void Market::simulatePriceMovement(const std::string& symbol) {
    if (currentPrices.find(symbol) == currentPrices.end()) {
        return;
    }
    
    double currentPrice = currentPrices[symbol];
    double vol = volatility[symbol];
    
    // Simple geometric Brownian motion simulation
    double dt = 1.0 / 252.0; // Daily time step (assuming 252 trading days per year)
    double drift = 0.05; // 5% annual expected return
    double randomShock = normalDist(randomGenerator);
    
    // Price change using GBM: dS = S * (mu * dt + sigma * sqrt(dt) * Z)
    double priceChange = currentPrice * (drift * dt + vol * std::sqrt(dt) * randomShock);
    double newPrice = std::max(0.01, currentPrice + priceChange); // Ensure price doesn't go negative
    
    currentPrices[symbol] = newPrice;
    recordPrice(symbol, newPrice);
}

double Market::getCurrentPrice(const std::string& symbol) const {
    auto it = currentPrices.find(symbol);
    return (it != currentPrices.end()) ? it->second : 0.0;
}

bool Market::hasSymbol(const std::string& symbol) const {
    return currentPrices.find(symbol) != currentPrices.end();
}

const std::vector<PriceData>& Market::getPriceHistory(const std::string& symbol) const {
    static const std::vector<PriceData> empty;
    auto it = priceHistory.find(symbol);
    return (it != priceHistory.end()) ? it->second : empty;
}

double Market::getDailyReturn(const std::string& symbol) const {
    auto it = priceHistory.find(symbol);
    if (it == priceHistory.end() || it->second.size() < 2) {
        return 0.0;
    }
    
    const auto& history = it->second;
    double currentPrice = history.back().price;
    double previousPrice = history[history.size() - 2].price;
    
    return (currentPrice - previousPrice) / previousPrice * 100.0;
}

double Market::getVolatility(const std::string& symbol) const {
    auto it = volatility.find(symbol);
    return (it != volatility.end()) ? it->second : 0.0;
}

void Market::setVolatility(const std::string& symbol, double vol) {
    if (hasSymbol(symbol)) {
        volatility[symbol] = vol;
    }
}

std::vector<std::string> Market::getAvailableSymbols() const {
    std::vector<std::string> symbols;
    for (const auto& pair : currentPrices) {
        symbols.push_back(pair.first);
    }
    return symbols;
}

void Market::printMarketSummary() const {
    std::cout << "\n=== Market Summary ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    for (const auto& pair : currentPrices) {
        const std::string& symbol = pair.first;
        double price = pair.second;
        double dailyReturn = getDailyReturn(symbol);
        double vol = getVolatility(symbol);
        
        std::cout << symbol << ": $" << price 
                  << " (Daily Return: " << std::showpos << dailyReturn << "%"
                  << ", Volatility: " << std::noshowpos << vol * 100 << "%)" << std::endl;
    }
    std::cout << "=====================\n" << std::endl;
}

void Market::recordPrice(const std::string& symbol, double price, double volume) {
    priceHistory[symbol].emplace_back(price, volume);
    
    // Keep only last 1000 price points to prevent memory issues
    if (priceHistory[symbol].size() > 1000) {
        priceHistory[symbol].erase(priceHistory[symbol].begin());
    }
}
