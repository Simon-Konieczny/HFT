cmake_minimum_required(VERSION 3.30)
project(core_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/konie/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Add your main project files and tests
add_executable(core_engine
        src/order_book.cpp
        src/matching_engine.cpp
        include/order.hpp
        include/order_book.hpp
        include/matching_engine.hpp
        include/utils.hpp
        main.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(core_engine GTest::GTest GTest::Main)

# Enable testing and add the test executable
enable_testing()
add_test(NAME order_book_tests COMMAND core_engine)