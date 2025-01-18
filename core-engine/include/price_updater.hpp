#ifndef price_updater_hpp

#define price_updater_hpp

#include <thread>
#include <random>
#include <unordered_map>
#include <mutex>

class PriceUpdater {
  public:
    PriceUpdater(std::unordered_map<std::string, double>& stockPrices, std::mutex& priceMutex, double mu, double sigma, double dt)
        : stockPrices(stockPrices), priceMutex(priceMutex), mu(mu), sigma(sigma), dt(dt), running(false), distribution(0.0,1.0){};
    void start();
    void stop();
    ~PriceUpdater();

  private:
    bool running;
    std::thread priceThread;

    //GBM params
    std::unordered_map<std::string, double>& stockPrices;
    std::mutex& priceMutex;
    double mu, sigma, dt;

    //Random number generators
    std::default_random_engine generator;
    std::normal_distribution<double> distribution;

    void updatePrices();
};

#endif