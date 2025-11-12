// ITradingListener.h
// Interface for receiving trading engine events (orders, simulation lifecycle)
#ifndef ITRADINGLISTENER_H
#define ITRADINGLISTENER_H

#include <string>
#include "Order.h"

class ITradingListener
{
public:
    virtual ~ITradingListener() = default;

    // Called when an order is submitted (before validation)
    virtual void onOrderSubmitted(const Order &order) = 0;

    // Called after validation. 'valid' is true if order passed validation
    virtual void onOrderValidated(const Order &order, bool valid) = 0;

    // Called when an order (partially) executes
    virtual void onOrderExecuted(const Order &order, double executedPrice, double executedQuantity) = 0;

    // Called when an order is cancelled
    virtual void onOrderCancelled(const Order &order) = 0;

    // Called when an order fails
    virtual void onOrderFailed(const Order &order, const std::string &reason) = 0;

    // Optional lifecycle hooks
    virtual void onSimulationStart() {}
    virtual void onSimulationStop() {}
};

#endif // ITRADINGLISTENER_H
