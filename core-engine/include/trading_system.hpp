#ifndef TRADING_SYSTEM_HPP
#define TRADING_SYSTEM_HPP

#include <condition_variable>

#include "price_updater.hpp"
#include "matching_engine.hpp"

#include <unordered_map>
#include <mutex>
#include <queue>
#include <string>
#include <zmq.hpp>

class TradingSystem {
    public:
        TradingSystem() : running(false), priceUpdater(stockPrices, priceMutex, 0.05, 0.2, 0.01) {}

        void start();
        void stop();
        void receiveOrders(std::vector<Order>& orders);
        void receiveStocks(const std::unordered_map<std::string, double>& incomingStocks);

    private:
        std::unordered_map<std::string, double> stockPrices;
        std::mutex priceMutex;
        PriceUpdater priceUpdater;
        std::thread priceUpdaterThread;
        MatchingEngine matchingEngine;
        zmq::socket_t socket;
        std::queue<MatchedOrder> matchedOrderQueue;
        std::mutex queueMutex;
        std::condition_variable queueCondition;
        const size_t BATCH_SIZE = 50;
        const std::chrono::milliseconds BATCH_INTERVAL = std::chrono::milliseconds(2);
        std::atomic<bool> running;
        void batchProcessor();
        void publishBatch(const std::vector<MatchedOrder>& batch);
        void serializeBatch(const std::vector<MatchedOrder>& batch);
};

#endif