#include "../include/trading_system.hpp"

void TradingSystem::start() {
    price_updater_thread = std::thread(&PriceUpdater::run, &priceUpdater);
    main_thread = std::thread([this]() {
        while (running) {
            zmq::message_t msg;
            if (dealerSocket.recv(msg, zmq::recv_flags::dontwait)) {
                std::string event_data(static_cast<char*>(msg.data()), msg.size());
                std::cout << "Received event: " << event_data << std::endl;
                DispatchEvent(event_data);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Prevent busy-wait
        }
    });
}

void TradingSystem::stop() {
    // Stop the PriceUpdater thread safely
    priceUpdater.stop();
    if (price_updater_thread.joinable()) {
        price_updater_thread.join();
    }
    dealerSocket.close();
    std::cout << "Stopping Trading System" << std::endl;
    running = false;
    if (main_thread.joinable()) {
        main_thread.join();
    }
}

// This will come from ZeroMQ
void TradingSystem::receiveOrders(std::vector<Order>& orders) {
    std::vector<MatchedOrder> matched_orders;
    for (auto& order : orders) {
        auto ma = matchingEngine.processOrder(order);
        for (auto& matched_order : ma) {
            matched_orders.push_back(matched_order);
        }
    }
}

// This will come from ZeroMQ
void TradingSystem::receiveStocks(const std::unordered_map<std::string, double>& incomingStocks) {
    for (auto&[fst, snd] : incomingStocks) {
        stockPrices.insert(std::pair<std::string, double>(fst, snd));
    }
}

void TradingSystem::DispatchEvent(const std::string& event_data) {
    if (std::string event_type = ParseEventType(event_data); event_type == "PRICE UPDATE"){
        std::cout << "Passing event to Price Updater: " << event_data << std::endl;
        priceUpdater.addEvent(event_data);
    } else {
        std::cout << "Received unknown event: " << event_data << std::endl;
    }
}

std::string TradingSystem::ParseEventType(const std::string &event_data) {
    size_t pos = event_data.find('|');
    return (pos != std::string::npos) ? event_data.substr(0, pos) : "UNKNOWN";
}



