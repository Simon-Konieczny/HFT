#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

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
    std::vector<MatchedOrder> matchBuyOrder(const Order& order);
    std::vector<MatchedOrder> matchSellOrder(const Order& order);
    double buyVolume = 0;
    double sellVolume = 0;
    int orderCount = 0;

  public:
    void addOrder(const Order& order);
    bool removeOrder(int order_id);
    std::vector<MatchedOrder> processOrder(const Order& order);
    OrderBook();
    double getBuyVolume() const;
    double getSellVolume() const;
    double getAverageVolume() const;
    double calculateImbalanceVolatility() const;


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