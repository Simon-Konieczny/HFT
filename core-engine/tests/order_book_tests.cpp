#include "../include/order_book.hpp"
#include <gtest/gtest.h>

TEST(OrderBookTest, AddOrder) {
  OrderBook order_book;
  Order order(1, Side::BUY, 100.0, 10, OrderType::LIMIT, 123456789);
  order_book.addOrder(order);

  // Assert order was added
  ASSERT_TRUE(order_book.contains(order));
}

TEST(OrderBookTest, MatchOrder) {
  OrderBook order_book;
  Order buy_order(1, Side::BUY, 100.0, 10, OrderType::LIMIT, 123456789);
  Order sell_order(1, Side::SELL, 100.0, 10, OrderType::LIMIT, 123456789);

  order_book.addOrder(buy_order);
  auto result = order_book.matchOrder(sell_order);

  ASSERT_TRUE(result.second == 10);
}