#include "../include/price_updater.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>

void PriceUpdater::updatePrices(std::string& event_data) {
    std::cout << "Price Updater received: " << event_data << std::endl;
    std::lock_guard<std::mutex> lock(priceMutex);
    auto [event, identifier, price_str, quantity_str, isBuy_str] = parsePriceUpdateEvent(event_data);
    int quantity = std::stoi(quantity_str);
    bool isBuy = std::stoi(isBuy_str);

    // calculate order imbalance
    double buyVolume = orderBooks[identifier]->getBuyVolume();
    double sellVolume = orderBooks[identifier]->getSellVolume();
    double imbalance = (buyVolume - sellVolume) / (buyVolume + sellVolume);

    // calculate matched order impact
    double avgVolume = orderBooks[identifier]->getAverageVolume();
    double matchedImpact = (quantity / avgVolume) * (isBuy ? 1 : -1);

    // update drift & volatility
    double drift = baseDrift + (lambda * imbalance) + (alpha * matchedImpact);
    double volatility = baseVolatility + (beta * orderBooks[identifier]->calculateImbalanceVolatility());

    // simulate GBM price update
    double dw = randomGaussian();
    double dp = drift * stockPrices[identifier] * volatility * stockPrices[identifier] * dw;
    stockPrices[identifier] = stockPrices[identifier] + dp;
}

void PriceUpdater::run() {
    std::cout << "Running Price Updater" << std::endl;
    while (running) {
        std::string event_data;
        if (eventQueue.try_pop(event_data)) {
            if (event_data.empty()) {
                break;
            }
            std::cout << "Processing event: " << event_data << std::endl;
            updatePrices(event_data);
            std::cout << "Updated stocks: " << std::endl;
            for (auto [id, price] : stockPrices) {
                std::cout << id << ", " << price << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

void PriceUpdater::stop() {
    std::cout << "Stopping Price Updater" << std::endl;
    running = false;
    eventQueue.push("");
}

void PriceUpdater::addEvent(const std::string &event_data) {
    std::cout << "Adding event: " << event_data << std::endl;
    eventQueue.push(event_data);
}

std::tuple<std::string, std::string, std::string, std::string, std::string> PriceUpdater::parsePriceUpdateEvent(const std::string &event_data) {
    std::tuple<std::string> tokens;
    std::stringstream ss(event_data);
    std::string type, identifier, price_str, quantity_str, isBuy_str;

    if (!std::getline(ss, type, '|') ||
            !std::getline(ss, identifier, '|') ||
            !std::getline(ss, price_str, '|') ||
            !std::getline(ss, quantity_str, '|') ||
            !std::getline(ss, isBuy_str, '|')) {
        throw std::invalid_argument("Invalid event structure: " + event_data);
            }

    return {type, identifier, price_str, quantity_str, isBuy_str};
}



