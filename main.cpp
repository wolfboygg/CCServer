#include <iostream>

#include "CStreamServer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    CStreamServer cStreamServer;
    cStreamServer.StartServer();
    return 0;
}
