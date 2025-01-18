#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>
#include <iostream>

enum class OrderType { MARKET, LIMIT };
enum class Side { BUY, SELL };

struct Order {
  int id;
  int client_id;
  Side side;
  double price;
  mutable int quantity;
  OrderType type;
  long timestamp;
  std::string identifier;

  Order(int id, int client_id, Side side, double price, int quantity, OrderType type, std::string stock_symbol, int timestamp)
  : id(id), client_id(client_id), side(side), price(price), quantity(quantity), type(type), identifier(stock_symbol), timestamp(timestamp) {}

  friend std::ostream& operator<<(std::ostream& os, const Order& order);
};

inline std::ostream& operator<<(std::ostream& os, const Order& order) {
  os << "Order Details:\n"
       << "ID: " << order.id << " "
       << "Client ID: " << order.client_id << " "
       << "Side: " << (order.side == Side::BUY ? "BUY" : "SELL") << " "
       << "Price: " << order.price << " "
       << "Quantity: " << order.quantity << " "
       << "Type: " << (order.type == OrderType::LIMIT ? "LIMIT" : "MARKET") << " "
       << "Timestamp: " << order.timestamp << std::endl;
    return os;
}

#endif