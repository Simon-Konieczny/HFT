#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <iostream>
#include <map>
#include <set>
#include <utility>
#include "order.hpp"

struct OrderComparator {
  bool operator()(const Order& lhs, const Order& rhs) const {
    return lhs.timestamp < rhs.timestamp;
  }
};

class OrderBook {
  private:
    std::map<double, std::set<Order, OrderComparator>> buy_orders;
    std::map<double, std::set<Order, OrderComparator>, std::greater<>> sell_orders;
    void matchBuyOrder(const Order& order);
    void matchSellOrder(const Order& order);

  public:
    void addOrder(const Order& order);
    bool removeOrder(int order_id);
    void processOrder(const Order& order);
    OrderBook();

  friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);
};

inline std::ostream& operator<<(std::ostream& os, const OrderBook& book) {
  os << "Order book:\n";
  os << "Buy orders:\n";
  for (const auto& [price, orders] : book.buy_orders) {
    os << "Price: " << price << "\n";
    for (const auto& order : orders) {
      os << order << std::endl;
    }
  }

  os << "Sell orders:\n";
  for (const auto& [price, orders] : book.sell_orders) {
    os << "Price: " << price << "\n";
    for (const auto& order : orders) {
      os << order << std::endl;
    }
  }

  return os;
}

#endif