#ifndef TRADING_ENGINE_H
#define TRADING_ENGINE_H

#include <vector>
#include <queue>
#include <memory>
#include "Order.h"
#include "Portfolio.h"
#include "Market.h"

class TradingEngine {
private:
    Market& market;
    Portfolio& portfolio;
    std::queue<Order> pendingOrders;
    std::vector<Order> executedOrders;
    double transactionCost;
    bool enableLogging;

public:
    TradingEngine(Market& mkt, Portfolio& port, double txnCost = 5.0);
    
    // Order management
    bool submitOrder(const Order& order);
    void processOrders();
    void cancelOrder(int orderId);
    
    // Trading strategies
    void executeMarketOrder(const std::string& symbol, OrderType type, double quantity);
    void executeLimitOrder(const std::string& symbol, OrderType type, double quantity, double price);
    
    // Engine configuration
    void setTransactionCost(double cost) { transactionCost = cost; }
    void enableOrderLogging(bool enable) { enableLogging = enable; }
    
    // Query methods
    const std::vector<Order>& getExecutedOrders() const { return executedOrders; }
    size_t getPendingOrderCount() const { return pendingOrders.size(); }
    
    // Simulation control
    void runSimulation(int steps);
    void printTradingStats() const;
    
private:
    bool validateOrder(const Order& order) const;
    bool tryExecuteOrder(Order& order);
    void logOrderExecution(const Order& order, double executionPrice) const;
    double calculateOrderValue(const Order& order, double price) const;
};

#endif // TRADING_ENGINE_H
