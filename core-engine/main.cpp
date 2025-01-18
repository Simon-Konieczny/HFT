#include "../core-engine/include/matching_engine.hpp"
#include <iostream>
#include <chrono>

#include "include/trading_system.hpp"

int main() {
    // int id = 1;
    // Side side = Side::SELL;
    // double price = 100.0;
    // int quantity = 10;
    // OrderType type = OrderType::MARKET;
    // std::string symbol = "AAAA";
    // long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // int client_id = 1;
    //
    // Order order1(id, client_id, side, price, quantity, type, symbol, timestamp);
    //
    // int newId = 2;
    // Side new_side = Side::BUY;
    // int new_quantity = 1;
    // long new_timestamp = timestamp + 1;
    // int client_id2 = 2;
    // Order order2(newId, client_id2, new_side, price, new_quantity, type, symbol, new_timestamp);
    //
    // std::cout << "\nBeginning execution\n\n";
    // MatchingEngine matching_engine;
    // matching_engine.processOrder(order1);
    // matching_engine.printOrderBook(order1.identifier);
    // matching_engine.processOrder(order2);
    // matching_engine.printOrderBook(order1.identifier);

    std::vector<Order> orders;
    orders.emplace_back(1, 1001, Side::BUY, 150.0, 10, OrderType::LIMIT, "AAPL", 1632950400);
    orders.emplace_back(2, 1002, Side::SELL, 250.0, 5, OrderType::MARKET, "MSFT", 1632950500);
    orders.emplace_back(3, 1003, Side::BUY, 650.0, 2, OrderType::LIMIT, "TSLA", 1632950600);

    std::unordered_map<std::string, double> stocks = {
        {"AAPL", 150.0},
        {"MSFT", 250.0},
        {"TSLA", 650.0}
    };

    std::cout << "Starting core system...\n\n";
    TradingSystem trading_system;

    trading_system.start();
    trading_system.receiveStocks(stocks);

    // trading_system.receiveOrders(orders);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "\n\nStopping core system...\n\n";
    trading_system.stop();

    return 0;
}
