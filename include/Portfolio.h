#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <string>
#include <map>
#include <vector>
#include "Order.h"

struct Position {
    std::string symbol;
    double quantity;
    double averagePrice;
    double unrealizedPnL;
    double realizedPnL;
    
    Position(const std::string& sym = "", double qty = 0.0, double price = 0.0)
        : symbol(sym), quantity(qty), averagePrice(price), unrealizedPnL(0.0), realizedPnL(0.0) {}
};

class Portfolio {
private:
    double cash;
    double totalValue;
    std::map<std::string, Position> positions;
    std::vector<Order> orderHistory;

public:
    Portfolio(double initialCash = 100000.0);
    
    // Portfolio management
    bool canAffordOrder(const Order& order) const;
    void executeOrder(const Order& order, double executionPrice);
    void updatePositionValue(const std::string& symbol, double currentPrice);
    
    // Getters
    double getCash() const { return cash; }
    double getTotalValue() const { return totalValue; }
    const std::map<std::string, Position>& getPositions() const { return positions; }
    const std::vector<Order>& getOrderHistory() const { return orderHistory; }
    
    // Position queries
    bool hasPosition(const std::string& symbol) const;
    double getPositionQuantity(const std::string& symbol) const;
    double getPositionValue(const std::string& symbol, double currentPrice) const;
    
    // Portfolio analytics
    double getTotalPnL() const;
    double getPortfolioReturn() const;
    void printPortfolioSummary() const;
    
private:
    void updateCash(double amount) { cash += amount; }
    void updateTotalValue();
};

#endif // PORTFOLIO_H
