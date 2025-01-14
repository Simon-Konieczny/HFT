#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include "order_book.hpp"

class MatchingEngine {
  private:
    OrderBook order_book;

  public:
    void processOrder(const Order& order);
};

#endif