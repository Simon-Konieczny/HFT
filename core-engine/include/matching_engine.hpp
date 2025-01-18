#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include <memory>

#include "order_book.hpp"
#include <string>
#include <unordered_map>

class MatchingEngine {
  private:
    std::unordered_map<std::string, std::unique_ptr<OrderBook>> orderBooks;
    void matchBuyOrder();

  public:
    void processOrder(const Order& order);
    void printOrderBook(const std::string& symbol);
};

#endif