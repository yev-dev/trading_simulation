// MomentumStrategy.cpp
// Simple example strategy implementing IStrategy: buys symbols with positive daily returns

#include "IStrategy.h"
#include "TradingEngine.h"
#include "Market.h"
#include "Portfolio.h"
#include <iostream>

class MomentumStrategy : public IStrategy
{
public:
    MomentumStrategy(double threshold = 0.02, double qty = 10.0)
        : returnThreshold(threshold), orderQty(qty) {}

    std::string name() const override { return "MomentumStrategy"; }

    void onSimulationStart() override
    {
        std::cout << "[Strategy] " << name() << " started" << std::endl;
    }

    void onSimulationStop() override
    {
        std::cout << "[Strategy] " << name() << " stopped" << std::endl;
    }

    void onTick(Market &market, Portfolio &portfolio, TradingEngine &engine, int /*step*/) override
    {
        for (const auto &sym : market.getAvailableSymbols())
        {
            double daily = market.getDailyReturn(sym);
            if (daily > returnThreshold)
            {
                // Buy on positive momentum
                engine.executeMarketOrder(sym, OrderType::BUY, orderQty);
            }
            else if (daily < -returnThreshold && portfolio.hasPosition(sym))
            {
                // Sell some on negative momentum
                engine.executeMarketOrder(sym, OrderType::SELL, std::max(1.0, orderQty / 2.0));
            }
        }
    }

    void onOrderExecuted(const Order &order) override
    {
        std::cout << "[Strategy] Order executed: " << order.toString() << std::endl;
    }

private:
    double returnThreshold;
    double orderQty;
};

// Note: to use this strategy create an instance and register it with the code
// that drives the TradingEngine (e.g., in main or a strategy manager).
