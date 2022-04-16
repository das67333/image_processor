#include <iostream>

#include "application.h"

int main(int argc, const char* argv[]) {
    try {
        Application app;
        app.Configure();
        app.Run(argc, argv);
    } catch (std::exception& err) {
        std::cerr << err.what() << '\n';
    }
    return 0;
}
