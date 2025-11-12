// ITradingEngineAPI.h
// Minimal interface to expose TradingEngine operations to external agents (IPC/MCP).
// Implementations of this interface can be used to forward calls from an external
// process (over IPC, HTTP, etc.) to an in-process TradingEngine instance.
#ifndef ITRADINGENGINEAPI_H
#define ITRADINGENGINEAPI_H

#include <string>
#include <vector>
#include "Order.h"

class Market;
class Portfolio;
class TradingEngine;
class ITradingListener;

class ITradingEngineAPI
{
public:
    virtual ~ITradingEngineAPI() = default;

    // Submit an order. Returns true if the order was accepted and queued.
    virtual bool submitOrder(const Order &order) = 0;

    // Convenience helpers that mirror TradingEngine public API
    virtual void executeMarketOrder(const std::string &symbol, OrderType type, double quantity) = 0;

    // Cancel an order by id
    virtual void cancelOrder(int orderId) = 0;

    // Query methods
    virtual std::vector<Order> getExecutedOrders() const = 0;
    virtual size_t getPendingOrderCount() const = 0;

    // Simulation control
    virtual void runSimulation(int steps) = 0;
    // stopSimulation may be a no-op if the underlying engine does not support async stop
    virtual void stopSimulation() = 0;

    // Listener registration: adapter implementations should forward notifications
    virtual void registerListener(ITradingListener *listener) = 0;
    virtual void unregisterListener(ITradingListener *listener) = 0;
};

#endif // ITRADINGENGINEAPI_H
