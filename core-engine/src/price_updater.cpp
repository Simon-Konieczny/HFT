#include "../include/price_updater.hpp"

#include <algorithm>
#include <iostream>

void PriceUpdater::updatePrices() {
    std::lock_guard<std::mutex> lock(priceMutex);

    for (auto& pair : stockPrices) {
        std::string symbol = pair.first;
        double currentPrice = pair.second;

        //simulate price movement using Geometric Brownian Motion
        stockPrices[symbol] = gbm(currentPrice);
        std:: cout << symbol << " " << stockPrices[symbol] << std::endl;
    }
}

PriceUpdater::~PriceUpdater() {
    stop();
}

void PriceUpdater::start() {
    running = true;
    priceThread = std::thread([this]() {
        while (running) {
            updatePrices();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    priceThread.detach();
}

void PriceUpdater::stop() {
    running = false;
    if (priceThread.joinable()) {
        priceThread.join();
    }
}

double PriceUpdater::gbm(double currentPrice) {
    double Z = distribution(generator);
    return (currentPrice * exp((mu - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Z));
}

