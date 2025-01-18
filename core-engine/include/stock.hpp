#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>
#include <unordered_map>

class Stock {
  private:
    std::unordered_map<std::string, Stock> stock_map;

  public:
    Stock();
    void addStock();
    void removeStock();

};

#endif