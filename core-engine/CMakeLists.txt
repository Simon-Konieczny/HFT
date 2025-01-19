cmake_minimum_required(VERSION 3.30)
project(core_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/konie/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Add your main project files and tests
add_executable(core_engine
        src/order_book.cpp
        src/matching_engine.cpp
        src/price_updater.cpp
        src/trading_system.cpp
        include/order.hpp
        include/order_book.hpp
        include/matching_engine.hpp
        include/utils.hpp
        include/price_updater.hpp
        include/trading_system.hpp
        main.cpp
)

find_package(ZeroMQ CONFIG REQUIRED)
target_link_libraries(core_engine PRIVATE libzmq libzmq-static)

find_package(cppzmq CONFIG REQUIRED)
target_link_libraries(core_engine PRIVATE cppzmq cppzmq-static)