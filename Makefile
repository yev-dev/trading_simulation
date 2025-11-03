# Trading Simulation Makefile
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O2
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

# Find all .cpp files in src directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/TradingSimulation

# Default target
all: $(TARGET)

# Test target
tests: $(TEST_TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build target
$(TARGET): $(BUILD_DIR)/main.o $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $^ -o $@ -pthread

# Build test target
$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $^ -o $@ -pthread

# Build main object file
$(BUILD_DIR)/main.o: main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Build test object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -I$(TEST_DIR) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Build and run tests
check: tests test

# Debug build
debug: CXXFLAGS += -DDEBUG -g -O0
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: clean $(TARGET)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Show help
help:
	@echo "Available targets:"
	@echo "  all     - Build the project (default)"
	@echo "  tests   - Build the test suite"
	@echo "  clean   - Remove build files"
	@echo "  run     - Build and run the program"
	@echo "  test    - Run the test suite"
	@echo "  check   - Build and run tests"
	@echo "  debug   - Build with debug flags"
	@echo "  release - Build optimized release version"
	@echo "  install - Install to /usr/local/bin"
	@echo "  help    - Show this help message"

# Print variables (for debugging makefile)
print-vars:
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "TARGET: $(TARGET)"

# Phony targets
.PHONY: all tests clean run test check debug release install help print-vars
