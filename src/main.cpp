#include "app.hpp"

#include <exception>
#include <iostream>


int main() {
    firstGame::app app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}