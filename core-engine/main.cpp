#include "../core-engine/include/matching_engine.hpp"
#include <iostream>
#include <chrono>
#include <zmq.hpp>
#include <iostream>

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

    std::string event_data = "PRICE UPDATE|AAPL|101|1|1";

    std::cout << "Starting core system...\n\n";
    TradingSystem trading_system;
    trading_system.receiveStocks(stocks);
    trading_system.receiveOrders(orders);

    trading_system.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::dealer);
    socket.connect("tcp://localhost:5555");
    std::cout << "Sending event...: " << event_data << std::endl;
    socket.send(zmq::buffer(event_data), zmq::send_flags::none);

    std::this_thread::sleep_for(std::chrono::seconds(10));


    std::cout << "\n\nStopping core systems...\n\n";
    trading_system.stop();

    return 0;
}
