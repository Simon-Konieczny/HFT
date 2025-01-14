#include "../include/order_book.hpp"
#include <stdexcept>

OrderBook::OrderBook() {
    buy_orders = std::map<double, std::set<Order, OrderComparator>>();
    sell_orders = std::map<double, std::set<Order, OrderComparator>>();
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
        auto order_it = std::ranges::find_if(order_set.begin(), order_set.end(), [order_id](const Order& o) {
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
        auto order_it = std::ranges::find_if(order_set.begin(), order_set.end(), [order_id](const Order& o) {
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

bool OrderBook::contains(const struct Order &order) {
    for (auto it = buy_orders.begin(); it != buy_orders.end(); ++it) {
        if (it->second.contains(order)) {
            return true;
        }
    }
    for (auto it = sell_orders.begin(); it != sell_orders.end(); ++it) {
        if (it->second.contains(order)) {
            return true;
        }
    }
    return false;
}


std::pair<Order, int> OrderBook::matchOrder(Order &incoming_order) {
    auto& opposite_orders = incoming_order.side == Side::BUY ? (sell_orders) : buy_orders;

    while (incoming_order.quantity > 0 && !opposite_orders.empty()) {
        auto best_price_it = opposite_orders.begin();
        auto& best_order_set = best_price_it->second;
        auto best_order_it = best_order_set.begin();

        Order best_order = *best_order_it;

        // check price condition for limit orders
        if (incoming_order.type == OrderType::LIMIT &&
            ((incoming_order.side == Side::BUY && incoming_order.price < best_order.price) ||
                (incoming_order.side == Side::SELL && incoming_order.price > best_order.price))) {
            break;
                }

        // calculate the matched quantity
        int matched_quantity = std::min(incoming_order.quantity, best_order.quantity);
        incoming_order.quantity -= matched_quantity;

        // update or remove the matched order
        best_order.quantity -= matched_quantity;
        best_order_set.erase(best_order_it);
        if (best_order.quantity > 0) {
            best_order_set.insert(best_order);
        }
        if (best_order_set.empty()) {
            opposite_orders.erase(best_price_it);
        }

        return {best_order, matched_quantity};
    }

    return {Order(-1, Side::BUY, 0.0, 0, OrderType::MARKET, 0), 0}; // no match
}



