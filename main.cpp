#include <iostream>
#include <chrono>
#include "inc/NeighborCache.h"
#include "inc/Heuristic.h"

int main() {
    std::vector<Short> tiles = {2, 3, 4, 7, 8, 9};
//    get the execution time of the following line
    std::cout << "Starting PDB creation..." << std::endl;
    auto start = std::chrono::steady_clock::now();
    Heuristic* heuristic = new Heuristic(tiles);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time difference for creating = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "[s]" << std::endl;
    auto start2 = std::chrono::steady_clock::now();
    heuristic->saveToFile("pdb.txt");
    auto end2 = std::chrono::steady_clock::now();
    std::cout << "Time difference for saving = " << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() / 1000.0 << "[s]" << std::endl;

    return 0;
}
