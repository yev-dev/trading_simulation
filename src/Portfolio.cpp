#include "Portfolio.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Portfolio::Portfolio(double initialCash) : cash(initialCash), totalValue(initialCash) {}

bool Portfolio::canAffordOrder(const Order& order) const {
    if (order.getType() == OrderType::BUY) {
        double orderValue = order.getQuantity() * order.getPrice();
        return cash >= orderValue;
    } else {
        // For sell orders, check if we have enough position
        return getPositionQuantity(order.getSymbol()) >= order.getQuantity();
    }
}

void Portfolio::executeOrder(const Order& order, double executionPrice) {
    if (!canAffordOrder(order)) {
        return; // Cannot execute order
    }
    
    const std::string& symbol = order.getSymbol();
    double quantity = order.getQuantity();
    
    if (order.getType() == OrderType::BUY) {
        // Buy order
        double orderValue = quantity * executionPrice;
        updateCash(-orderValue);
        
        if (positions.find(symbol) == positions.end()) {
            positions[symbol] = Position(symbol, 0.0, 0.0);
        }
        
        Position& pos = positions[symbol];
        // Update average price using weighted average
        if (pos.quantity > 0) {
            double totalCost = (pos.quantity * pos.averagePrice) + (quantity * executionPrice);
            pos.quantity += quantity;
            pos.averagePrice = totalCost / pos.quantity;
        } else {
            pos.quantity = quantity;
            pos.averagePrice = executionPrice;
        }
    } else {
        // Sell order
        Position& pos = positions[symbol];
        double saleValue = quantity * executionPrice;
        updateCash(saleValue);
        
        // Calculate realized P&L
        double realizedPnL = quantity * (executionPrice - pos.averagePrice);
        pos.realizedPnL += realizedPnL;
        pos.quantity -= quantity;
        
        // Remove position if quantity becomes zero
        if (pos.quantity <= 0.001) { // Use small epsilon for floating point comparison
            positions.erase(symbol);
        }
    }
    
    // Add to order history
    orderHistory.push_back(order);
    updateTotalValue();
}

void Portfolio::updatePositionValue(const std::string& symbol, double currentPrice) {
    auto it = positions.find(symbol);
    if (it != positions.end()) {
        Position& pos = it->second;
        pos.unrealizedPnL = pos.quantity * (currentPrice - pos.averagePrice);
    }
    updateTotalValue();
}

bool Portfolio::hasPosition(const std::string& symbol) const {
    return positions.find(symbol) != positions.end();
}

double Portfolio::getPositionQuantity(const std::string& symbol) const {
    auto it = positions.find(symbol);
    return (it != positions.end()) ? it->second.quantity : 0.0;
}

double Portfolio::getPositionValue(const std::string& symbol, double currentPrice) const {
    auto it = positions.find(symbol);
    return (it != positions.end()) ? it->second.quantity * currentPrice : 0.0;
}

double Portfolio::getTotalPnL() const {
    double totalPnL = 0.0;
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        totalPnL += pos.realizedPnL + pos.unrealizedPnL;
    }
    return totalPnL;
}

double Portfolio::getPortfolioReturn() const {
    double initialValue = 100000.0; // Assume default initial value
    return (totalValue - initialValue) / initialValue * 100.0;
}

void Portfolio::printPortfolioSummary() const {
    std::cout << "\n=== Portfolio Summary ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Cash: $" << cash << std::endl;
    std::cout << "Total Portfolio Value: $" << totalValue << std::endl;
    std::cout << "Total P&L: $" << getTotalPnL() << std::endl;
    std::cout << "Portfolio Return: " << getPortfolioReturn() << "%" << std::endl;
    
    if (!positions.empty()) {
        std::cout << "\nCurrent Positions:" << std::endl;
        for (const auto& pair : positions) {
            const Position& pos = pair.second;
            std::cout << pos.symbol << ": " << pos.quantity << " shares @ $" 
                      << pos.averagePrice << " (Unrealized P&L: $" 
                      << pos.unrealizedPnL << ")" << std::endl;
        }
    }
    std::cout << "========================\n" << std::endl;
}

void Portfolio::updateTotalValue() {
    totalValue = cash;
    for (const auto& pair : positions) {
        const Position& pos = pair.second;
        // For this calculation, we'd need current market prices
        // This is a simplified version
        totalValue += pos.quantity * pos.averagePrice + pos.unrealizedPnL;
    }
}
