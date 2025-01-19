#include "../include/trading_system.hpp"

void TradingSystem::start() {
    running = true;
    priceUpdaterThread = std::thread(&PriceUpdater::start, &priceUpdater);
    socket.bind("tcp://localhost:5555");

    std::thread(&TradingSystem::batchProcessor, this).detach();
}

void TradingSystem::stop() {
    running = false;
    queueCondition.notify_all();  // Wake up any waiting threads
    // Stop the PriceUpdater thread safely
    priceUpdater.stop();
    if (priceUpdaterThread.joinable()) {
        priceUpdaterThread.join();
    }
}

// This will come from ZeroMq
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

