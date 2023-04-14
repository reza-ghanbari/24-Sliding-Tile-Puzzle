//
// Created by reza on 12/04/23.
//

#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include "../inc/Heuristic.h"

Long Heuristic::getRank(const State &state) const {
    std::vector<Short> selectedState;
    std::vector<Short> dual = state.getDual();
    Long rank = 0;
    for (int i = 0; i < PDB_STATE_SIZE; ++i) {
        rank <<= 5;
        rank |= dual[tiles[i]];
    }
    return rank;
}

void Heuristic::generatePDB() {
    std::vector<Short> goalState = {0, 0, 2, 3, 4,
                                    0, 0, 7, 8, 9,
                                    0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0,
                                    0, 0, 0, 0, 0};
    std::vector<Short> goalDual = goalState;
    auto* goal = new State(goalState, goalDual, 0);
// implement a BFS to generate the PDB
    std::queue<State*> queue;
    queue.push(goal);
    PDB[getRank(*goal)] = 0;
    while (!queue.empty()) {
        State* current = queue.front();
        queue.pop();
        Short currentRank = PDB[getRank(*current)];
        std::vector<Short> state = current->getState();
        std::vector<Short> dual = current->getDual();
        for (int i = 0; i < CAPACITY; ++i) {
            if (state[i] == 0) {
                for (Short neighbor: neighborCache.getNeighbors(i)) {
                    if (state[neighbor] == 0) {
                        continue;
                    }
                    auto newState = state;
                    auto newDual = dual;
                    std::swap(newState[i], newState[neighbor]);
                    std::swap(newDual[newState[i]], newDual[newState[neighbor]]);
                    State* next = new State(newState, newDual);
                    Long nextRank = getRank(*next);
                    if (PDB.find(nextRank) == PDB.end()) {
                        PDB[nextRank] = currentRank + 1;
                        queue.push(next);
                    }
                    else {
                        delete next;
                    }
                }
            }
        }
        delete current;
    }
}

void Heuristic::saveToFile(const std::string& fileName) {
    std::ofstream file;
    file.open(fileName);
    for (auto &i : PDB) {
        file << i.first << " " << unsigned(i.second) << std::endl;
    }
    file.close();
}

void Heuristic::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        uint64_t key;
        uint8_t value;
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            key = std::stoull(line.substr(0, spacePos));
            value = std::stoi(line.substr(spacePos + 1));
            PDB[key] = value;
        } else {
            std::cerr << "Failed to parse line: " << line << std::endl;
            break;
        }
    }
    file.close();
}
