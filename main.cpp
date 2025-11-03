#include <iostream>
#include <thread>
#include <chrono>
#include "include/Order.h"
#include "include/Portfolio.h"
#include "include/Market.h"
#include "include/TradingEngine.h"

using namespace std;

void demonstrateBasicTrading()
{
    std::cout << "\n=== Basic Trading Demonstration ===" << std::endl;

    // Create market and add some stocks
    Market market;
    market.addSymbol("AAPL", 150.0, 0.02);    // Apple with 2% volatility
    market.addSymbol("GOOGL", 2800.0, 0.025); // Google with 2.5% volatility
    market.addSymbol("MSFT", 300.0, 0.018);   // Microsoft with 1.8% volatility
    market.addSymbol("TSLA", 800.0, 0.04);    // Tesla with 4% volatility

    // Create portfolio with $100,000 initial cash
    Portfolio portfolio(100000.0);

    // Create trading engine
    TradingEngine engine(market, portfolio);

    std::cout << "Initial market state:" << std::endl;
    market.printMarketSummary();

    std::cout << "Initial portfolio state:" << std::endl;
    portfolio.printPortfolioSummary();

    // Execute some market orders
    std::cout << "Executing market orders..." << std::endl;
    engine.executeMarketOrder("AAPL", OrderType::BUY, 100);
    engine.executeMarketOrder("GOOGL", OrderType::BUY, 10);
    engine.executeMarketOrder("MSFT", OrderType::BUY, 50);

    // Execute some limit orders
    std::cout << "\nSubmitting limit orders..." << std::endl;
    engine.executeLimitOrder("TSLA", OrderType::BUY, 25, 790.0);  // Buy if price drops to $790
    engine.executeLimitOrder("AAPL", OrderType::SELL, 50, 155.0); // Sell if price rises to $155

    // Run simulation for a few steps
    engine.runSimulation(20);
}

void demonstrateAdvancedTrading()
{
    std::cout << "\n=== Advanced Trading Strategies ===" << std::endl;

    Market market;
    market.addSymbol("SPY", 400.0, 0.015); // S&P 500 ETF
    market.addSymbol("QQQ", 350.0, 0.02);  // NASDAQ ETF
    market.addSymbol("AMD", 80.0, 0.035);  // AMD with higher volatility

    Portfolio portfolio(50000.0);
    TradingEngine engine(market, portfolio, 2.0); // Lower transaction cost

    // Implement a simple momentum strategy
    std::cout << "Implementing momentum trading strategy..." << std::endl;

    for (int day = 0; day < 30; ++day)
    {
        market.updatePrices();

        // Check daily returns and implement momentum strategy
        for (const std::string &symbol : market.getAvailableSymbols())
        {
            double dailyReturn = market.getDailyReturn(symbol);
            double currentPrice = market.getCurrentPrice(symbol);

            if (dailyReturn > 2.0)
            { // Strong positive momentum
                if (portfolio.getCash() > currentPrice * 10)
                {
                    engine.executeMarketOrder(symbol, OrderType::BUY, 10);
                    std::cout << "Momentum BUY: " << symbol << " (Return: " << dailyReturn << "%)" << std::endl;
                }
            }
            else if (dailyReturn < -2.0 && portfolio.hasPosition(symbol))
            { // Strong negative momentum
                double position = portfolio.getPositionQuantity(symbol);
                if (position >= 5)
                {
                    engine.executeMarketOrder(symbol, OrderType::SELL, 5);
                    std::cout << "Momentum SELL: " << symbol << " (Return: " << dailyReturn << "%)" << std::endl;
                }
            }
        }

        // Process pending orders
        engine.processOrders();

        // Update portfolio values
        for (const std::string &symbol : market.getAvailableSymbols())
        {
            portfolio.updatePositionValue(symbol, market.getCurrentPrice(symbol));
        }

        // Print status every 10 days
        if ((day + 1) % 10 == 0)
        {
            std::cout << "\n--- Day " << (day + 1) << " Status ---" << std::endl;
            market.printMarketSummary();
            portfolio.printPortfolioSummary();
        }

        // Small delay to make output readable
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    engine.printTradingStats();
}

void demonstrateOrderManagement()
{
    std::cout << "\n=== Order Management Demo ===" << std::endl;

    Market market;
    market.addSymbol("NVDA", 500.0, 0.03);

    Portfolio portfolio(25000.0);
    TradingEngine engine(market, portfolio);

    // Submit multiple limit orders
    engine.executeLimitOrder("NVDA", OrderType::BUY, 20, 495.0);
    engine.executeLimitOrder("NVDA", OrderType::BUY, 15, 490.0);
    engine.executeLimitOrder("NVDA", OrderType::BUY, 10, 485.0);

    std::cout << "Submitted 3 limit buy orders for NVDA" << std::endl;
    std::cout << "Pending orders: " << engine.getPendingOrderCount() << std::endl;

    // Run simulation to potentially fill orders
    std::cout << "\nRunning simulation to fill orders..." << std::endl;
    engine.runSimulation(15);

    std::cout << "Final pending orders: " << engine.getPendingOrderCount() << std::endl;
    std::cout << "Executed orders: " << engine.getExecutedOrders().size() << std::endl;
}

int main(int, char **)
{
    std::cout << "=== C++ Trading Simulation ===" << std::endl;
    std::cout << "A comprehensive trading system with separate header organization" << std::endl;

    try
    {
        // Run different demonstrations
        demonstrateBasicTrading();

        std::cout << "\n"
                  << std::string(60, '=') << std::endl;
        demonstrateAdvancedTrading();

        std::cout << "\n"
                  << std::string(60, '=') << std::endl;
        demonstrateOrderManagement();

        std::cout << "\n=== Simulation Complete ===" << std::endl;
        std::cout << "All trading scenarios executed successfully!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
