//
// Created by reza on 12/04/23.
//

#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "../inc/Heuristic.h"

Int Heuristic::pick(Short n, Short k)
{
    Int result = 1;
    for (Short i = n; i > n - k; --i)
        result *= i;
    return result;
}

void Heuristic::InitializeRankingCalculationTables() {
    for (unsigned i = 0; i < (1 << CAPACITY) - 1; ++i)
    {
        std::bitset<CAPACITY> bits(i);
        onesCountLookup[i] = bits.count();
    }
    for (unsigned i = 1; i < PDB_STATE_SIZE + 1; ++i)
        picks[i - 1] = pick(CAPACITY - i, PDB_STATE_SIZE - i);
};

Int Heuristic::getRankOfSelectedDuals(std::vector<Short>& dual) const {
    Short lehmer[PDB_STATE_SIZE];
    std::bitset<CAPACITY> seen;
    lehmer[0] = dual[0];
    seen[CAPACITY - 1 - dual[0]] = true;
    for (unsigned i = 1; i < PDB_STATE_SIZE; ++i)
    {
        seen[CAPACITY - 1 - dual[i]] = true;
        unsigned numOnes = this->onesCountLookup[seen.to_ulong() >> (CAPACITY - dual[i])];
        lehmer[i] = dual[i] - numOnes;
    }
    Int rank = 0;
    for (size_t i = 0; i < PDB_STATE_SIZE; ++i)
        rank += lehmer[i] * this->picks[i];
    return rank;
}

Int Heuristic::getRank(std::vector<Short>& dual) const {
    std::vector<Short> selectedDuals;
    selectedDuals.reserve(tiles.size());
    for (short tile: tiles) {
        selectedDuals.push_back(dual[tile]);
    }
    return getRankOfSelectedDuals(selectedDuals);
}

Int Heuristic::getRank(State* state) const {
    auto dual = state->getDual();
    return getRank(dual);
}

void Heuristic::generatePDB() {
    PDB = std::vector<Short>(pick(CAPACITY, PDB_STATE_SIZE));
    std::vector<Short> goalState;
    goalState.reserve(CAPACITY);
    for (int i = 0; i < CAPACITY; ++i) {
        goalState.push_back((std::count(tiles.begin(), tiles.end(), i) > 0) ? i : 0);
    }
    std::vector<Short> goalDual = goalState;
    auto* goal = new State(goalState, goalDual, 0);
    std::queue<State*> queue;
    queue.push(goal);
    Int goalRank = getRank(goal);
    PDB[goalRank] = 0;
    while (!queue.empty()) {
        State* current = queue.front();
        queue.pop();
        std::vector<Short> state = current->getState();
        std::vector<Short> dual = current->getDual();
        Short currentRank = PDB[getRank(dual)];
        for (Short tileNumber : tiles) {
            Short tile = dual[tileNumber];
            for (Short neighbor: neighborCache->getNeighbors(tile)) {
                if (state[neighbor] != 0) {
                    continue;
                }
                auto newState = state;
                auto newDual = dual;
                std::swap(newState[tile], newState[neighbor]);
                newDual[tileNumber] = neighbor;
                Int nextRank = getRank(newDual);
                if (PDB[nextRank] == 0 && nextRank != goalRank) {
                    auto* next = new State(newState, newDual);
                    PDB[nextRank] = currentRank + 1;
                    queue.push(next);
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
        file << unsigned(i) << std::endl;
    }
    file.close();
}

void Heuristic::readFromFile(const std::string& filename) {
    PDB.reserve(pick(CAPACITY, PDB_STATE_SIZE));
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    unsigned int num;
//    std::vector<Int> distribution(36);
    while (file >> num) {
        PDB.push_back(num);
//        distribution[num]++;
    }
//    for (int i = 0; i < 36; ++i) {
//        std::cout << unsigned (i) << ": " << unsigned (distribution[i]) << std::endl;
//    }
//    std::cout << "Total: " << PDB.size() << std::endl;
    file.close();
}

Short Heuristic::getHeuristic(std::vector<Short>& dual) {
    return PDB[getRank(dual)];
}

Short Heuristic::getHeuristicOfSelectedDuals(std::vector<Short>& dual) {
    return PDB[getRankOfSelectedDuals(dual)];
}
