#include <iostream>
#include <chrono>
#include <algorithm>
#include "inc/NeighborCache.h"
#include "inc/Heuristic.h"
#include "inc/Solver.h"

Heuristic* getHeuristic(std::vector<Short>& tiles, NeighborCache* neighborCache, const std::string& filename) {
    if (READ_PDB) {
        std::cout << "Starting loading PDB..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        auto* heuristic = new Heuristic(tiles, neighborCache, filename);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time difference for reading = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "[s]" << std::endl;
        return heuristic;
    }
    else {
        std::cout << "Starting PDB creation..." << std::endl;
        auto start = std::chrono::steady_clock::now();
        auto* heuristic = new Heuristic(tiles, neighborCache);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time difference for creating = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "[s]" << std::endl;
        return heuristic;
    }
}

State* getRoot() {
    std::vector<Short> state = {10, 3, 24, 12, 0, 7, 8, 11, 14, 21, 22, 23, 2, 1, 9, 17, 18, 6, 20, 4, 13, 15, 5, 19, 16};
    std::vector<Short> dual;
    for (Short i = 0; i < 25; i++) {
        dual.push_back(std::find(state.begin(), state.end(), i) - state.begin());
    }
    return new State(state, dual, dual[0]);
}

int main() {
    std::vector<Short> regularTiles = {2, 3, 4, 7, 8, 9};
    std::vector<Short> irregularTiles = {1, 5, 6, 10, 11, 12};
    auto* neighborCache = new NeighborCache();
    auto* regularHeuristic = getHeuristic(regularTiles, neighborCache, "pdb-regular.txt");
    auto* irregularHeuristic = getHeuristic(irregularTiles, neighborCache, "pdb-regular.txt");
    if (WRITE_PDB) {
        regularHeuristic->saveToFile("pdb-regular.txt");
        irregularHeuristic->saveToFile("pdb-irregular.txt");
    }
    State* root = getRoot();
    Solver* solver = new Solver(regularHeuristic, irregularHeuristic, neighborCache);
    solver->calculateHeuristic(*root);
    std::vector<Short> path = solver->solve(root);
    std::cout << "Path: ";
    for (auto& step: path) {
        std::cout << unsigned(step) << " ";
    }
    return 0;
}
