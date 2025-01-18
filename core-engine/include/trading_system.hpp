#ifndef TRADING_SYSTEM_HPP
#define TRADING_SYSTEM_HPP

#include "price_updater.hpp"
#include "matching_engine.hpp"

#include <unordered_map>
#include <mutex>
#include <string>

class TradingSystem {
  private:
    std::unordered_map<std::string, double> stockPrices;
    std::mutex priceMutex;
    bool running;
    PriceUpdater priceUpdater;
    std::thread priceUpdaterThread;
    MatchingEngine matchingEngine;

  public:
    TradingSystem() : running(false), priceUpdater(stockPrices, priceMutex, 0.05, 0.2, 0.01) {}
    void start();
    void stop();
    void receiveOrders(std::vector<Order>& orders);
    void receiveStocks(const std::unordered_map<std::string, double>& incomingStocks);
};

#endif