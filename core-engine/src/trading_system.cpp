#include "../include/trading_system.hpp"

void TradingSystem::start() {
    running = true;
    priceUpdaterThread = std::thread(&PriceUpdater::start, &priceUpdater);
}

void TradingSystem::stop() {
    running = false;
    // Stop the PriceUpdater thread safely
    priceUpdater.stop();
    if (priceUpdaterThread.joinable()) {
        priceUpdaterThread.join();
    }
}

void TradingSystem::receiveOrders(std::vector<Order>& orders) {
    for (auto& order : orders) {
        matchingEngine.processOrder(order);
    }
}

void TradingSystem::receiveStocks(const std::unordered_map<std::string, double>& incomingStocks) {
    for (auto&[fst, snd] : incomingStocks) {
        stockPrices.insert(std::pair<std::string, double>(fst, snd));
    }
}

