cmake_minimum_required(VERSION 3.30)
project(core_engine)

set(CMAKE_CXX_STANDARD 20)

# Add your main project files and tests
add_executable(core_engine
        src/order_book.cpp
        include/order.hpp
        include/order_book.hpp
        include/matching_engine.hpp
        include/utils.hpp
        main.cpp
        tests/order_book_tests.cpp
)

# Include directories for GoogleTest
target_include_directories(core_engine PRIVATE
        "C:/Users/konie/Documents/CPP Lib/googletest/googlemock/include"
        "C:/Users/konie/Documents/CPP Lib/googletest/googletest/include"
)

# Link directories for GoogleTest
target_link_directories(core_engine PRIVATE
        "C:/Users/konie/Documents/CPP Lib/googletest/build/lib"
)

# Link GoogleTest and GoogleMock libraries
target_link_libraries(core_engine PRIVATE
        gtest
        gmock
)

# Enable testing and add the test executable
enable_testing()
add_test(NAME order_book_test COMMAND core_engine)