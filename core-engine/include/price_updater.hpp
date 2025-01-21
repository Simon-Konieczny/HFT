#ifndef price_updater_hpp

#define price_updater_hpp

#include <condition_variable>
#include <iostream>
#include <thread>
#include <random>
#include <unordered_map>
#include <mutex>
#include <queue>

#include "order_book.hpp"

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable condVar;

public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
        condVar.notify_one();
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return false;
        }
        value = queue.front();
        queue.pop();
        return true;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};

class PriceUpdater {
  public:
    PriceUpdater(std::unordered_map<std::string, double>& stockPrices, std::mutex& priceMutex, std::unordered_map<std::string, std::unique_ptr<OrderBook>>& orderBooks)
        : running(true), stockPrices(stockPrices), priceMutex(priceMutex), distribution(0.0,1.0), orderBooks(orderBooks) {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    };
    void stop();
    void run();
    void addEvent(const std::string& event_data);

  private:
    bool running;
    ThreadSafeQueue<std::string> eventQueue;

    //GBM params
    std::unordered_map<std::string, double>& stockPrices;
    std::mutex& priceMutex;
    double baseDrift = 0.001;
    double baseVolatility = 0.01;
    double lambda = 0.1;
    double beta = 0.05;
    double alpha = 0.1;

    //Random number generators
    std::mt19937 generator;
    std::normal_distribution<double> distribution;

    void updatePrices(std::string& event_data);

    static std::tuple<std::string, std::string, std::string, std::string, std::string> parsePriceUpdateEvent(const std::string& event_data);

    std::unordered_map<std::string, std::unique_ptr<OrderBook>>& orderBooks;

    double randomGaussian() {
        return distribution(generator);
    }
};

#endif