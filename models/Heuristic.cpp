//
// Created by reza on 12/04/23.
//

#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "../inc/Heuristic.h"

Int Heuristic::getRank(const State &state) const {
    std::vector<Short> dual = state.getDual();
    Int rank = 0;
    for (Short tile: tiles) {
        rank <<= 5;
        rank |= dual[tile];
    }
    return rank;
}

Int Heuristic::getRank(std::vector<Short>& dual) const {
    Int rank = 0;
    for (short tile: tiles) {
        rank <<= 5;
        rank |= dual[tile];
    }
    return rank;
}

void Heuristic::generatePDB() {
    std::vector<Short> goalState = getGoal();
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
        for (Short tileNumber : tiles) {
            Short tile = dual[tileNumber];
            for (Short neighbor: neighborCache.getNeighbors(tile)) {
                if (state[neighbor] != 0) {
                    continue;
                }
                auto newState = state;
                auto newDual = dual;
                std::swap(newState[tile], newState[neighbor]);
                newDual[tileNumber] = neighbor;
                Int nextRank = getRank(newDual);
                if (PDB.find(nextRank) == PDB.end()) {
                    auto* next = new State(newState, newDual);
                    PDB[nextRank] = currentRank + 1;
                    queue.push(next);
                }
            }
        }
        delete current;
    }
}

std::vector<Short> Heuristic::getGoal() const {
    std::vector<Short> goalState;
    for (int i = 0; i < CAPACITY; ++i) {
        if (std::count(tiles.begin(), tiles.end(), i) > 0)
            goalState.push_back(i);
        else
            goalState.push_back(0);
    }
    return goalState;
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
