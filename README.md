# C++ Trading Simulation

A comprehensive C++ trading simulation system with proper header file organization, demonstrating object-oriented design principles and modern C++ features.

## Project Structure

```
trading_simulation/
├── include/                 # Header files directory
│   ├── Order.h             # Order management and types
│   ├── Portfolio.h         # Portfolio and position management
│   ├── Market.h            # Market data and price simulation
│   └── TradingEngine.h     # Trading engine and order execution
├── src/                    # Source files directory
│   ├── Order.cpp           # Order implementation
│   ├── Portfolio.cpp       # Portfolio implementation
│   ├── Market.cpp          # Market implementation
│   └── TradingEngine.cpp   # Trading engine implementation
├── build/                  # Build output directory
├── main.cpp               # Main application demonstrating trading
├── CMakeLists.txt         # CMake build configuration
├── Makefile              # Alternative Makefile build system
└── README.md             # This documentation
```

## Features

### 🏗️ **Architecture**
- **Modular Design**: Separate header files in `include/` directory
- **Clean Separation**: Source files organized in `src/` directory
- **Modern C++17**: Uses modern C++ features and best practices
- **Cross-Platform**: Works on Windows, macOS, and Linux

### 📈 **Trading Simulation Components**

#### Order Management (`Order.h/cpp`)
- **Order Types**: Buy/Sell orders with different statuses
- **Order Lifecycle**: Pending → Filled/Cancelled/Partially Filled
- **Timestamps**: Automatic timestamping of all orders
- **Validation**: Built-in order validation and error handling

#### Portfolio Management (`Portfolio.h/cpp`)
- **Position Tracking**: Real-time position and P&L calculation
- **Cash Management**: Automatic cash flow management
- **Risk Controls**: Position limits and cash availability checks
- **Performance Analytics**: Portfolio returns and P&L tracking

#### Market Simulation (`Market.h/cpp`)
- **Price Generation**: Geometric Brownian Motion price simulation
- **Multiple Symbols**: Support for multiple trading instruments
- **Volatility Control**: Configurable volatility per symbol
- **Historical Data**: Price history storage and analysis

#### Trading Engine (`TradingEngine.h/cpp`)
- **Order Execution**: Market and limit order processing
- **Order Queuing**: Pending order management
- **Transaction Costs**: Configurable transaction fees
- **Strategy Support**: Framework for trading strategy implementation

### 🚀 **Trading Strategies Included**

1. **Basic Trading**: Simple market and limit orders
2. **Momentum Strategy**: Buy on strong positive momentum, sell on negative
3. **Order Management**: Advanced order queuing and execution

## Building the Project

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16+ (optional, for CMake build)
- Make (for Makefile build)

### Option 1: Using CMake (Recommended)

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the simulation
./TradingSimulation
```

### Option 2: Using Makefile

```bash
# Build the project
make

# Run the simulation
make run

# Build with debug symbols
make debug

# Build optimized release
make release

# Clean build files
make clean
```

### Build Options

```bash
# Makefile targets
make help           # Show all available targets
make debug          # Build with debug flags (-g -O0)
make release        # Build optimized release (-O3)
make clean          # Remove all build files
make install        # Install to /usr/local/bin
```

## Usage Examples

### Basic Usage

```cpp
#include "include/Market.h"
#include "include/Portfolio.h"
#include "include/TradingEngine.h"

// Create market with stocks
Market market;
market.addSymbol("AAPL", 150.0, 0.02);  // Apple, $150, 2% volatility

// Create portfolio with $100,000
Portfolio portfolio(100000.0);

// Create trading engine
TradingEngine engine(market, portfolio);

// Execute trades
engine.executeMarketOrder("AAPL", OrderType::BUY, 100);
engine.executeLimitOrder("AAPL", OrderType::SELL, 50, 155.0);

// Run simulation
engine.runSimulation(30);
```

### Advanced Strategy Example

```cpp
// Momentum trading strategy
for (int day = 0; day < 30; ++day) {
    market.updatePrices();

    for (const std::string& symbol : market.getAvailableSymbols()) {
        double dailyReturn = market.getDailyReturn(symbol);

        if (dailyReturn > 2.0) {  // Strong positive momentum
            engine.executeMarketOrder(symbol, OrderType::BUY, 10);
        } else if (dailyReturn < -2.0 && portfolio.hasPosition(symbol)) {
            engine.executeMarketOrder(symbol, OrderType::SELL, 5);
        }
    }

    engine.processOrders();
}
```

## Key Classes and Methods

### Order Class
```cpp
Order(symbol, type, quantity, price)    // Constructor
getOrderId()                            // Get unique order ID
getStatus()                             // Get order status
fillOrder(quantity)                     // Fill order partially/fully
toString()                              // Get order description
```

### Portfolio Class
```cpp
Portfolio(initialCash)                  // Constructor with starting cash
executeOrder(order, price)              // Execute an order
getPositions()                          // Get all positions
getTotalValue()                         // Get portfolio value
getTotalPnL()                           // Get profit/loss
printPortfolioSummary()                 // Print detailed summary
```

### Market Class
```cpp
Market()                               // Constructor
addSymbol(symbol, price, volatility)   // Add trading symbol
updatePrices()                         // Update all prices
getCurrentPrice(symbol)                // Get current price
getDailyReturn(symbol)                 // Get daily return %
printMarketSummary()                   // Print market status
```

### TradingEngine Class
```cpp
TradingEngine(market, portfolio)       // Constructor
executeMarketOrder(symbol, type, qty)  // Execute market order
executeLimitOrder(symbol, type, qty, price) // Submit limit order
runSimulation(steps)                   // Run simulation
processOrders()                        // Process pending orders
printTradingStats()                    // Print execution statistics
```

## Performance Features

- **Efficient STL Usage**: Optimized use of containers and algorithms
- **Memory Management**: Automatic memory management with RAII
- **Threading Support**: Built-in thread safety considerations
- **Scalable Design**: Supports large numbers of orders and positions

## Sample Output

```
=== C++ Trading Simulation ===
A comprehensive trading system with separate header organization

=== Market Summary ===
AAPL: $150.00 (Daily Return: +0.00%, Volatility: 2.00%)
GOOGL: $2800.00 (Daily Return: +0.00%, Volatility: 2.50%)
MSFT: $300.00 (Daily Return: +0.00%, Volatility: 1.80%)
TSLA: $800.00 (Daily Return: +0.00%, Volatility: 4.00%)

=== Portfolio Summary ===
Cash: $42000.00
Total Portfolio Value: $100409.49
Total P&L: $409.49
Portfolio Return: 0.41%

Current Positions:
AAPL: 100.00 shares @ $150.00 (Unrealized P&L: $-101.03)
GOOGL: 10.00 shares @ $2800.00 (Unrealized P&L: $412.33)
MSFT: 50.00 shares @ $300.00 (Unrealized P&L: $98.20)
```

## Contributing

1. Follow the existing code style and organization
2. Add new headers to `include/` directory
3. Add new implementations to `src/` directory
4. Update both CMakeLists.txt and Makefile for new files
5. Add appropriate unit tests
6. Update documentation

## License

This project is provided as an educational example of C++ trading simulation systems.

## Future Enhancements

- [ ] Options trading support
- [ ] Advanced order types (Stop-loss, Take-profit)
- [ ] Real market data integration
- [ ] Backtesting framework
- [ ] GUI interface
- [ ] Database integration
- [ ] Multi-threading support
- [ ] Configuration file support


```
g++ -std=c++17 -Wall -Wextra -g -O0 -Iinclude main.cpp src/*.cpp -o TradingSimulation -pthread

```


What I added

ITradingEngineAPI.h

A minimal, documented interface for external agents to interact with the engine.
Methods include submitOrder, executeMarketOrder, executeLimitOrder, cancelOrder, queries (executed orders, pending count, current price, available symbols), simulation control (runSimulation, stopSimulation) and listener registration hooks.
Designed to be lightweight and easy to wrap behind an IPC transport (JSON-RPC, REST, sockets, etc.).
TradingEngineMCPAdapter.cpp

An in-process adapter that implements ITradingEngineAPI by delegating to an existing TradingEngine instance.
Acts as the component you can call from your MCP server implementation: MCP handler -> TradingEngineMCPAdapter -> TradingEngine.
stopSimulation and listener registration are left as no-ops or placeholders because the current TradingEngine implementation doesn't provide those primitives; the adapter includes comments on how to extend the engine.
CMakeLists.txt

Registered the new adapter source and the API header so IDEs and builds pick them up.
Notes and next steps

stopSimulation: TradingEngine currently has no async stop API. If you need to stop a running simulation from an external agent, add a stop flag and a check in the simulation loop (or provide an interrupt mechanism). I can implement this if you want.
Listener support: If you want external agents to receive push notifications (order executed, order failed, etc.), make TradingEngine manage a list of ITradingListener pointers and call them as events happen; the adapter can register a listener that forwards events out over IPC. I included registerListener/unregisterListener in the API to make this wiring straightforward.
Serialization: The API uses C++ types (Order, vector<Order>, etc.). For real IPC you’ll need to serialize/deserialize Orders (JSON or protobuf). I can add helper converters (Order <-> JSON) or implement a small JSON-RPC wrapper around the adapter.
Thread-safety: IPC handlers typically run in separate threads. If your external agent will call the adapter concurrently, consider adding mutexes around the engine calls or ensure TradingEngine is thread-safe.
Examples I can add next (pick one):

Make TradingEngine accept ITradingListener* and call listeners on order events, plus implement a listener in the adapter that forwards events to a JSON socket.
Add a stop flag and implement TradingEngine::requestStop() and have adapter call it.
Implement JSON serialization helpers for Order and Market so the adapter can return JSON-able structures.
Would you like me to:

Implement listener wiring so external agents can receive events?
Add a stopSimulation implementation (engine-side and adapter-side)?
Provide JSON serialization helpers and a small example IPC server wrapper (HTTP/REST or simple TCP)?
