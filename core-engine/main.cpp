#include "../core-engine/include/matching_engine.hpp"
#include <iostream>
#include <chrono>
#include <zmq.hpp>
#include <iostream>

#include "include/trading_system.hpp"

int main() {
    // std::vector<Order> orders;
    // orders.emplace_back(1, 1001, Side::BUY, 150.0, 10, OrderType::LIMIT, "AAPL", 1632950400);
    // orders.emplace_back(2, 1002, Side::SELL, 250.0, 5, OrderType::MARKET, "MSFT", 1632950500);
    // orders.emplace_back(3, 1003, Side::BUY, 650.0, 2, OrderType::LIMIT, "TSLA", 1632950600);
    //
    // std::unordered_map<std::string, double> stocks = {
    //     {"AAPL", 100.0},
    //     {"MSFT", 100.0},
    //     {"TSLA", 100.0}
    // };
    //
    // std::cout << "Starting core system...\n\n";
    // TradingSystem trading_system;
    //
    // trading_system.start();
    // trading_system.receiveStocks(stocks);
    //
    // trading_system.receiveOrders(orders);
    //
    // std::this_thread::sleep_for(std::chrono::seconds(10));
    //
    // std::cout << "\n\nStopping core system...\n\n";
    // trading_system.stop();
    //
    // return 0;

    //Test if ZeroMQ is working
    try {
        // Create the context (required for any ZeroMQ socket)
        zmq::context_t context(1);

        // Create the server (ZMQ_REP)
        zmq::socket_t server_socket(context, ZMQ_REP);
        server_socket.bind("tcp://*:5555");  // Bind to port 5555

        // Create the client (ZMQ_REQ)
        zmq::socket_t client_socket(context, ZMQ_REQ);
        client_socket.connect("tcp://localhost:5555");  // Connect to the server

        // Server will wait for a request and send a response
        std::thread server_thread([&server_socket]() {
            zmq::message_t request;
            while (true) {
                // Wait for the client message
                server_socket.recv(request, zmq::recv_flags::none);
                std::cout << "Server received: " << request.to_string() << std::endl;

                // Send a reply to the client
                zmq::message_t reply(5);
                memcpy(reply.data(), "World", 5);
                server_socket.send(reply, zmq::send_flags::none);
            }
        });

        // Client sends a message and waits for a reply
        zmq::message_t request(5);
        memcpy(request.data(), "Hello", 5);
        client_socket.send(request, zmq::send_flags::none);
        std::cout << "Client sent: Hello" << std::endl;

        zmq::message_t reply;
        client_socket.recv(reply, zmq::recv_flags::none);
        std::cout << "Client received: " << reply.to_string() << std::endl;

        // Wait for the server to finish
        server_thread.join();

    } catch (const zmq::error_t& e) {
        std::cerr << "ZeroMQ Error: " << e.what() << std::endl;
    }

    return 0;
}
