#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>

enum class OrderType {
    BUY,
    SELL
};

enum class OrderStatus {
    PENDING,
    FILLED,
    CANCELLED,
    PARTIALLY_FILLED
};

class Order {
private:
    static int nextOrderId;
    int orderId;
    std::string symbol;
    OrderType type;
    double quantity;
    double price;
    double filledQuantity;
    OrderStatus status;
    std::chrono::system_clock::time_point timestamp;

public:
    Order(const std::string& symbol, OrderType type, double quantity, double price);
    
    // Getters
    int getOrderId() const { return orderId; }
    const std::string& getSymbol() const { return symbol; }
    OrderType getType() const { return type; }
    double getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    double getFilledQuantity() const { return filledQuantity; }
    double getRemainingQuantity() const { return quantity - filledQuantity; }
    OrderStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getTimestamp() const { return timestamp; }
    
    // Setters
    void setStatus(OrderStatus newStatus) { status = newStatus; }
    void fillOrder(double fillQuantity);
    
    // Utility methods
    bool isFullyFilled() const { return filledQuantity >= quantity; }
    std::string toString() const;
};

#endif // ORDER_H
