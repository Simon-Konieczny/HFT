#include "../include/matching_engine.hpp"

#include <memory>

void MatchingEngine::processOrder(const Order& order){
    std::cout<<"processing order: " << order.id <<std::endl;
    if (orderBooks.find(order.identifier) == orderBooks.end()) {
        orderBooks[order.identifier] = std::make_unique<OrderBook>();
    }

    orderBooks[order.identifier]->processOrder(order);
}

void MatchingEngine::printOrderBook(const std::string &symbol) {
    if (orderBooks.find(symbol) == orderBooks.end()) {
        std::cout << "Symbol " << symbol << " not found" << std::endl;
        return;
    }

    std::cout << "Symbol: " << symbol << std::endl;
    std::cout << *orderBooks[symbol] << std::endl;
}

