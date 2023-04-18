#include <iostream>
#include <chrono>
#include <algorithm>
#include <sstream>
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

std::pair<std::vector<Short>, std::vector<std::vector<Short>>> readRootsFromFile(std::string filename) {
    std::ifstream infile(filename);
    std::string line;
    std::vector<Short> root;
    std::vector<Short> problemNumber;
    std::vector<std::vector<Short>> roots;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int number;
        iss >> number;
        problemNumber.push_back(number);
        while (iss >> number) {
            root.push_back(number);
        }
        roots.push_back(root);
        root.clear();
    }
    return std::make_pair(problemNumber, roots);
}

State* getRoot(std::vector<Short> state) {
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
    if (WRITE_PDB) {
        regularHeuristic->saveToFile("pdb-regular.txt");
    }
    auto* irregularHeuristic = getHeuristic(irregularTiles, neighborCache, "pdb-irregular.txt");
    if (WRITE_PDB) {
        irregularHeuristic->saveToFile("pdb-irregular.txt");
    }
    auto *solver = new Solver(regularHeuristic, irregularHeuristic, neighborCache);
    auto problems = readRootsFromFile("../res/problems.txt");
    int i = 0;
    for (const auto& rootState: problems.second) {
        State *root = getRoot(rootState);
        if (SOLVE) {
            std::cout << "Solving problem " << unsigned (problems.first[i++]) << std::endl;
            auto start = std::chrono::steady_clock::now();
            std::vector<Short> path = solver->solve(root);
            auto end = std::chrono::steady_clock::now();
            root->printState();
            std::cout << "Solution length: " << path.size() - 1 << std::endl;
            std::cout << "Time difference for solving = "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0 << "[s]"
                      << std::endl;
            std::cout << "Path: ";
            for (auto &step: path) {
                std::cout << unsigned(step) << " ";
            }
            std::cout << "\n\n\n" << std::endl;
        }
    }
    return 0;
}
