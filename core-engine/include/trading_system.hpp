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
        TradingSystem() : running(true), context(1), dealerSocket(context, zmq::socket_type::router), priceUpdater(stockPrices, priceMutex, orderBooks), matchingEngine(orderBooks) {
            dealerSocket.bind("tcp://localhost:5555");
            std::cout << "Trading System started" << std::endl;
        }

        void start();
        void stop();
        void receiveOrders(std::vector<Order>& orders);
        void receiveStocks(const std::unordered_map<std::string, double>& incomingStocks);

    private:
        std::thread main_thread;
        std::unordered_map<std::string, double> stockPrices;
        std::atomic<bool> running;

        std::thread price_updater_thread;
        PriceUpdater priceUpdater;


        std::mutex priceMutex;

        MatchingEngine matchingEngine;
        std::unordered_map<std::string, std::unique_ptr<OrderBook>> orderBooks;

        zmq::context_t context;
        zmq::socket_t dealerSocket;

        void DispatchEvent(const std::string& event_data);
        std::string ParseEventType(const std::string& event_data);
};

#endif