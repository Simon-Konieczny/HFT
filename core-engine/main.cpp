#include "../core-engine/include/matching_engine.hpp"
#include <iostream>
#include <chrono>

#include "include/trading_system.hpp"

int main() {
    std::vector<Order> orders;
    orders.emplace_back(1, 1001, Side::BUY, 150.0, 10, OrderType::LIMIT, "AAPL", 1632950400);
    orders.emplace_back(2, 1002, Side::SELL, 250.0, 5, OrderType::MARKET, "MSFT", 1632950500);
    orders.emplace_back(3, 1003, Side::BUY, 650.0, 2, OrderType::LIMIT, "TSLA", 1632950600);

    std::unordered_map<std::string, double> stocks = {
        {"AAPL", 100.0},
        {"MSFT", 100.0},
        {"TSLA", 100.0}
    };

    std::cout << "Starting core system...\n\n";
    TradingSystem trading_system;

    trading_system.start();
    trading_system.receiveStocks(stocks);

    trading_system.receiveOrders(orders);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "\n\nStopping core system...\n\n";
    trading_system.stop();

    return 0;
}
