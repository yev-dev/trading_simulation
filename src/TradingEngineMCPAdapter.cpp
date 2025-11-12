// TradingEngineMCPAdapter.cpp
// In-process adapter that implements ITradingEngineAPI by delegating
// to an existing TradingEngine instance. This adapter is intended to be wrapped
// by an MCP/IPC layer (JSON-RPC, REST, socket) that forwards external commands
// to the local engine.

#include "ITradingEngineAPI.h"
#include "TradingEngine.h"
#include "ITradingListener.h"
#include <algorithm>

class TradingEngineMCPAdapter : public ITradingEngineAPI
{
public:
    explicit TradingEngineMCPAdapter(TradingEngine &engine) : engine_(engine) {}

    bool submitOrder(const Order &order) override
    {
        return engine_.submitOrder(order);
    }

    void executeMarketOrder(const std::string &symbol, OrderType type, double quantity) override
    {
        engine_.executeMarketOrder(symbol, type, quantity);
    }
    void cancelOrder(int orderId) override
    {
        engine_.cancelOrder(orderId);
    }

    std::vector<Order> getExecutedOrders() const override
    {
        return engine_.getExecutedOrders();
    }

    size_t getPendingOrderCount() const override
    {
        return engine_.getPendingOrderCount();
    }

    void runSimulation(int steps) override
    {
        engine_.runSimulation(steps);
    }

    void stopSimulation() override
    {
        // TradingEngine does not currently implement an async stop API. If you
        // need this behavior, add a stop flag to TradingEngine and check it in
        // the simulation loop. For now this is a no-op.
    }

    void registerListener(ITradingListener *listener) override
    {
        // Simple example: engine doesn't currently store listeners; if you
        // extend TradingEngine to support listeners then forward here.
        (void)listener;
    }

    void unregisterListener(ITradingListener *listener) override
    {
        (void)listener;
    }

private:
    TradingEngine &engine_;
};
