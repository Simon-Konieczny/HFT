#include "../include/order_book.hpp"
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <vector>

OrderBook::OrderBook() {
    buy_orders = std::map<double, std::set<Order, OrderComparator>>();
    sell_orders = std::map<double, std::set<Order, OrderComparator>, std::greater<>>();
}

void OrderBook::addOrder(const Order& order) {
    if (order.side == Side::BUY) {
        buy_orders[order.price].insert(order);
    } else if (order.side == Side::SELL) {
        sell_orders[order.price].insert(order);
    } else {
        throw std::invalid_argument("Invalid order side");
    }
}

bool OrderBook::removeOrder(int order_id) {
    for (auto it = buy_orders.begin(); it != buy_orders.end(); ++it) {
       auto& order_set = it->second;
        auto order_it = std::find_if(order_set.begin(), order_set.end(), [order_id](const Order& o) {
            return o.id == order_id;
        });
        if (order_it != order_set.end()) {
            order_set.erase(order_it);
            if (order_set.empty()) {
                buy_orders.erase(it);
            }
            return true;
        }
    }

    for (auto it = sell_orders.begin(); it != sell_orders.end(); ++it) {
        auto& order_set = it->second;
        auto order_it = std::find_if(order_set.begin(), order_set.end(), [order_id](const Order& o) {
            return o.id == order_id;
        });
        if (order_it != order_set.end()) {
            order_set.erase(order_it);
            if (order_set.empty()) {
                sell_orders.erase(it);
            }
            return true;
        }
    }

    return false;
}

MatchedOrder OrderBook::processOrder(const Order& order) {
    if (order.side == Side::BUY) {
        return matchBuyOrder(order);
    }
    if (order.side == Side::SELL) {
        return matchSellOrder(order);
    }
    throw std::invalid_argument("Invalid order side");
}

std::vector<MatchedOrder> OrderBook::matchBuyOrder(const Order &order) {
    std::vector<MatchedOrder> matched_orders;
    while (order.quantity > 0 && !sell_orders.empty()) {
        auto& [price, sellQueue] = *sell_orders.begin();

        if (price > order.price) {
            break; // no match possible
        }

        auto& sellOrder = *sellQueue.begin();
        int matchQuantity = std::min(order.quantity, sellOrder.quantity);

        order.quantity -= matchQuantity;
        sellOrder.quantity -= matchQuantity;

        std:: cout << "Matched: " << matchQuantity << " at price : " << price << std::endl;
        MatchedOrder matched_order(order.id, sellOrder.id, order.price, matchQuantity, order.identifier, order.type);
        matched_orders.push_back(matched_order);

        if (sellOrder.quantity == 0) sellQueue.erase(sellQueue.begin());
        if (sellQueue.empty()) sell_orders.erase(order.price);
    }

    if (order.quantity > 0) {
        buy_orders[order.price].insert(order); // add remaining order
    }

    return matched_orders;
}

std::vector<MatchedOrder> OrderBook::matchSellOrder(const Order &order) {
    while (order.quantity > 0 && !buy_orders.empty()) {
        auto& [price, buyQueue] = *buy_orders.begin();

        if (price < order.price) break; // no match possible

        auto& sellOrder = *buyQueue.begin();
        int matchQuantity = std::min(order.quantity, sellOrder.quantity);

        order.quantity -= matchQuantity;
        sellOrder.quantity -= matchQuantity;

        std:: cout << "Matched: " << matchQuantity << " at price : " << price << std::endl;

        if (sellOrder.quantity == 0) buyQueue.erase(buyQueue.begin());
        if (buyQueue.empty()) sell_orders.erase(order.price);
    }

    if (order.quantity > 0) {
        sell_orders[order.price].insert(order); // add remaining order
    }
}




