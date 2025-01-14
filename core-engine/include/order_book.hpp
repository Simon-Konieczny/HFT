#ifndef ORDER_BOOK_HPP
#define ORDER_BOOK_HPP

#include "order.hpp"
#include <map>
#include <set>

struct OrderComparator {
  bool operator()(const Order& lhs, const Order& rhs) const {
    return lhs.timestamp < rhs.timestamp;
  }
};

class OrderBook {
  private:
    std::map<double, std::set<Order, OrderComparator>> buy_orders;
    std::map<double, std::set<Order, OrderComparator>> sell_orders;

  public:
    void addOrder(const Order& order);
    bool removeOrder(int order_id);
    bool contains(const Order& order);
    std::pair<Order, int> matchOrder(Order& incoming_order);
    OrderBook();
};

#endif