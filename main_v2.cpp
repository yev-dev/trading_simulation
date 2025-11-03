#include <iostream>
#include <string>
#include "include/Order.h"
#include "include/Portfolio.h"
#include "include/Market.h"
#include "include/TradingEngine.h"

using namespace std;

int add(int a, int b)
{
    return a + b;
}

int main()
{
    cout << "Welcome to the Trading Simulation!" << endl;
    cout << add(12, 4) << endl;
}
