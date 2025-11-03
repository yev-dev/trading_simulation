#include "TradingEngine.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

TradingEngine::TradingEngine(Market &mkt, Portfolio &port, double txnCost)
    : market(mkt), portfolio(port), transactionCost(txnCost), enableLogging(true) {}

bool TradingEngine::submitOrder(const Order &order)
{
    if (!validateOrder(order))
    {
        if (enableLogging)
        {
            std::cout << "Order validation failed: " << order.toString() << std::endl;
        }
        return false;
    }

    pendingOrders.push(order);
    if (enableLogging)
    {
        std::cout << "Order submitted: " << order.toString() << std::endl;
    }
    return true;
}

void TradingEngine::processOrders()
{
    std::queue<Order> failedOrders;

    while (!pendingOrders.empty())
    {
        Order order = pendingOrders.front();
        pendingOrders.pop();

        if (tryExecuteOrder(order))
        {
            executedOrders.push_back(order);
        }
        else
        {
            // Order couldn't be executed, put it back in queue
            failedOrders.push(order);
        }
    }

    // Put failed orders back in the queue
    pendingOrders = failedOrders;
}

void TradingEngine::cancelOrder(int orderId)
{
    std::queue<Order> remainingOrders;
    bool orderFound = false;

    while (!pendingOrders.empty())
    {
        Order order = pendingOrders.front();
        pendingOrders.pop();

        if (order.getOrderId() == orderId)
        {
            orderFound = true;
            if (enableLogging)
            {
                std::cout << "Order cancelled: " << order.toString() << std::endl;
            }
        }
        else
        {
            remainingOrders.push(order);
        }
    }

    pendingOrders = remainingOrders;

    if (!orderFound && enableLogging)
    {
        std::cout << "Order #" << orderId << " not found in pending orders" << std::endl;
    }
}

void TradingEngine::executeMarketOrder(const std::string &symbol, OrderType type, double quantity)
{
    if (!market.hasSymbol(symbol))
    {
        if (enableLogging)
        {
            std::cout << "Symbol " << symbol << " not found in market" << std::endl;
        }
        return;
    }

    double currentPrice = market.getCurrentPrice(symbol);
    Order marketOrder(symbol, type, quantity, currentPrice);

    if (validateOrder(marketOrder))
    {
        if (tryExecuteOrder(marketOrder))
        {
            executedOrders.push_back(marketOrder);
        }
    }
}

void TradingEngine::executeLimitOrder(const std::string &symbol, OrderType type, double quantity, double price)
{
    Order limitOrder(symbol, type, quantity, price);
    submitOrder(limitOrder);
}

void TradingEngine::runSimulation(int steps)
{
    std::cout << "\n=== Running Trading Simulation for " << steps << " steps ===" << std::endl;

    for (int step = 0; step < steps; ++step)
    {
        // Update market prices
        market.updatePrices();

        // Update portfolio values with current market prices
        for (const std::string &symbol : market.getAvailableSymbols())
        {
            double currentPrice = market.getCurrentPrice(symbol);
            portfolio.updatePositionValue(symbol, currentPrice);
        }

        // Process pending orders
        processOrders();

        // Print status every 10 steps
        if ((step + 1) % 10 == 0)
        {
            std::cout << "\nStep " << (step + 1) << " completed" << std::endl;
            market.printMarketSummary();
        }
    }

    std::cout << "\n=== Simulation Complete ===" << std::endl;
    portfolio.printPortfolioSummary();
    printTradingStats();
}

void TradingEngine::printTradingStats() const
{
    std::cout << "\n=== Trading Statistics ===" << std::endl;
    std::cout << "Total Executed Orders: " << executedOrders.size() << std::endl;
    std::cout << "Pending Orders: " << pendingOrders.size() << std::endl;

    if (!executedOrders.empty())
    {
        int buyOrders = 0, sellOrders = 0;
        double totalVolume = 0.0;

        for (const auto &order : executedOrders)
        {
            if (order.getType() == OrderType::BUY)
            {
                buyOrders++;
            }
            else
            {
                sellOrders++;
            }
            totalVolume += order.getQuantity() * order.getPrice();
        }

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Buy Orders: " << buyOrders << std::endl;
        std::cout << "Sell Orders: " << sellOrders << std::endl;
        std::cout << "Total Trading Volume: $" << totalVolume << std::endl;
        std::cout << "Average Order Value: $" << totalVolume / executedOrders.size() << std::endl;
    }
    std::cout << "==========================\n"
              << std::endl;
}

bool TradingEngine::validateOrder(const Order &order) const
{
    // Check if symbol exists in market
    if (!market.hasSymbol(order.getSymbol()))
    {
        return false;
    }

    // Check order quantity is positive
    if (order.getQuantity() <= 0)
    {
        return false;
    }

    // Check order price is positive
    if (order.getPrice() <= 0)
    {
        return false;
    }

    // Check if portfolio can afford the order
    if (!portfolio.canAffordOrder(order))
    {
        return false;
    }

    return true;
}

bool TradingEngine::tryExecuteOrder(Order &order)
{
    double currentPrice = market.getCurrentPrice(order.getSymbol());
    bool canExecute = false;

    if (order.getType() == OrderType::BUY)
    {
        // For buy orders, execute if current price is at or below limit price
        canExecute = currentPrice <= order.getPrice();
    }
    else
    {
        // For sell orders, execute if current price is at or above limit price
        canExecute = currentPrice >= order.getPrice();
    }

    if (canExecute)
    {
        // Execute the order
        order.fillOrder(order.getQuantity());
        order.setStatus(OrderStatus::FILLED);
        portfolio.executeOrder(order, currentPrice);

        if (enableLogging)
        {
            logOrderExecution(order, currentPrice);
        }

        return true;
    }

    return false;
}

void TradingEngine::logOrderExecution(const Order &order, double executionPrice) const
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "EXECUTED: " << order.getSymbol() << " "
              << (order.getType() == OrderType::BUY ? "BUY" : "SELL") << " "
              << order.getQuantity() << " @ $" << executionPrice
              << " (Order #" << order.getOrderId() << ")" << std::endl;
}

double TradingEngine::calculateOrderValue(const Order &order, double price) const
{
    double baseValue = order.getQuantity() * price;
    return baseValue + transactionCost;
}
