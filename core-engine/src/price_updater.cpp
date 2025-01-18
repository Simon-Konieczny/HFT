#include "../include/price_updater.hpp"

#include <algorithm>
#include <iostream>

void PriceUpdater::updatePrices() {
    std::lock_guard<std::mutex> lock(priceMutex);

    for (auto& pair : stockPrices) {
        std::string symbol = pair.first;
        double currentPrice = pair.second;

        //simulate price movement
        double randomFactor = mu * dt + sigma + sqrt(dt) * ((rand() % 1000) / 1000.0); //random walk
        stockPrices[symbol] = currentPrice * exp(randomFactor);
        std:: cout << symbol << " " << currentPrice << " " << randomFactor << std::endl;
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
}

