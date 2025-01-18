#include "../core-engine/include/matching_engine.hpp"
#include <iostream>
#include <chrono>

int main() {
    int id = 1;
    Side side = Side::SELL;
    double price = 100.0;
    int quantity = 10;
    OrderType type = OrderType::MARKET;
    std::string symbol = "AAAA";
    long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    int client_id = 1;

    Order order1(id, client_id, side, price, quantity, type, symbol, timestamp);

    int newId = 2;
    Side new_side = Side::BUY;
    int new_quantity = 1;
    long new_timestamp = timestamp + 1;
    int client_id2 = 2;
    Order order2(newId, client_id2, new_side, price, new_quantity, type, symbol, new_timestamp);

    std::cout << "\nBeginning execution\n\n";
    MatchingEngine matching_engine;
    matching_engine.processOrder(order1);
    matching_engine.printOrderBook(order1.identifier);
    matching_engine.processOrder(order2);
    matching_engine.printOrderBook(order1.identifier);
    return 0;
}