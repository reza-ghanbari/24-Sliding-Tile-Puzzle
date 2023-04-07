#include <iostream>
#include "inc/NeighborCache.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    NeighborCache neighborCache = NeighborCache();
    for (int i = 0; i < CAPACITY; ++i) {
        for (auto j: neighborCache.getNeighbors(i)) {
            std::cout << unsigned(j) << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
