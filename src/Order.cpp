#include "Order.h"
#include <sstream>
#include <iomanip>

int Order::nextOrderId = 1;

Order::Order(const std::string& symbol, OrderType type, double quantity, double price)
    : orderId(nextOrderId++), symbol(symbol), type(type), quantity(quantity), 
      price(price), filledQuantity(0.0), status(OrderStatus::PENDING),
      timestamp(std::chrono::system_clock::now()) {}

void Order::fillOrder(double fillQuantity) {
    if (fillQuantity <= 0 || fillQuantity > getRemainingQuantity()) {
        return; // Invalid fill quantity
    }
    
    filledQuantity += fillQuantity;
    
    if (isFullyFilled()) {
        status = OrderStatus::FILLED;
    } else {
        status = OrderStatus::PARTIALLY_FILLED;
    }
}

std::string Order::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Order #" << orderId << " - " << symbol << " ";
    oss << (type == OrderType::BUY ? "BUY" : "SELL") << " ";
    oss << quantity << " @ $" << price << " ";
    oss << "(Filled: " << filledQuantity << "/" << quantity << ") ";
    
    std::string statusStr;
    switch (status) {
        case OrderStatus::PENDING: statusStr = "PENDING"; break;
        case OrderStatus::FILLED: statusStr = "FILLED"; break;
        case OrderStatus::CANCELLED: statusStr = "CANCELLED"; break;
        case OrderStatus::PARTIALLY_FILLED: statusStr = "PARTIALLY_FILLED"; break;
    }
    oss << "[" << statusStr << "]";
    
    return oss.str();
}
