cmake_minimum_required(VERSION 3.16)
project(TradingSimulation VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For VSCode Intellisense

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/Order.cpp
    src/Portfolio.cpp
    src/Market.cpp
    src/TradingEngine.cpp
    src/MomentumStrategy.cpp
    src/TradingEngineMCPAdapter.cpp
)

# Header files (for IDE)
set(HEADERS
    include/Order.h
    include/Portfolio.h
    include/Market.h
    include/TradingEngine.h
    include/IStrategy.h
    include/ITradingEngineAPI.h
    include/ITradingListener.h
)

# Create executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})

# Link libraries (if needed)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)


# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
else()
    message(STATUS "Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()
