#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <iostream>

enum class OrderType { MARKET, LIMIT };
enum class Side { BUY, SELL };

struct Order {
  int id;
  Side side;
  double price;
  int quantity;
  OrderType type;
  long timestamp;

  Order(int id, Side side, double price, int quantity, OrderType type, int timestamp)
  : id(id), side(side), price(price), quantity(quantity), type(type), timestamp(timestamp) {}
};

#endif