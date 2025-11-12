// IStrategy.h
// Interface for trading strategies used by the TradingEngine
#ifndef ISTRATEGY_H
#define ISTRATEGY_H

#include <string>
#include "Order.h"

// Forward declarations to avoid heavy includes in the header
class Market;
class Portfolio;
class TradingEngine;

class IStrategy
{
public:
    virtual ~IStrategy() = default;

    // Called once when the simulation starts
    virtual void onSimulationStart() {}

    // Called once when the simulation stops
    virtual void onSimulationStop() {}

    // Called on every simulation step / tick. Implementations should inspect
    // the Market and Portfolio and may submit orders through the TradingEngine.
    virtual void onTick(Market &market, Portfolio &portfolio, TradingEngine &engine, int step) = 0;

    // Optional callback for order executions
    virtual void onOrderExecuted(const Order &order) {}

    // Human-readable strategy name
    virtual std::string name() const = 0;
};

#endif // ISTRATEGY_H
#pragma once
#
