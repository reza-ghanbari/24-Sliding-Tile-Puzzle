#include <iostream>
#include <chrono>
#include "inc/NeighborCache.h"
#include "inc/Heuristic.h"

Heuristic* getHeuristic(std::vector<Short>& tiles, const std::string& filename) {
    if (READ_PDB) {
        std::cout << "Starting loading PDB..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        Heuristic* heuristic = new Heuristic(tiles, filename);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time difference for reading = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "[s]" << std::endl;
        return heuristic;
    }
    else {
        std::cout << "Starting PDB creation..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        Heuristic* heuristic = new Heuristic(tiles);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time difference for creating = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "[s]" << std::endl;
        return heuristic;
    }
}

int main() {
    std::vector<Short> regularTiles = {2, 3, 4, 7, 8, 9};
    std::vector<Short> irregularTiles = {1, 5, 6, 10, 11, 12};
    auto* regularHeuristic = getHeuristic(regularTiles, "pdb-regular.txt");
    auto* irregularHeuristic = getHeuristic(irregularTiles, "pdb-regular.txt");

    if (WRITE_PDB) {
        regularHeuristic->saveToFile("pdb-regular.txt");
        irregularHeuristic->saveToFile("pdb-irregular.txt");
    }

    return 0;
}
