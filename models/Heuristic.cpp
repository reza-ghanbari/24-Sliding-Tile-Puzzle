//
// Created by reza on 12/04/23.
//

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
    Int rank = lehmer[0] * this->picks[0];
    for (unsigned i = 1; i < PDB_STATE_SIZE; ++i)
    {
        seen[CAPACITY - 1 - dual[i]] = true;
        unsigned numOnes = this->onesCountLookup[seen.to_ulong() >> (CAPACITY - dual[i])];
        lehmer[i] = dual[i] - numOnes;
        rank += lehmer[i] * this->picks[i];
    }
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

Long Heuristic::getRankOfState(std::vector<Short>& dual) {
    Long rank = 0;
    rank |= dual[0];
    for (Short tile: tiles) {
        rank <<= 5;
        rank |= dual[tile];
    }
    return rank;
}

State* Heuristic::getStateOfRank(Long rank) {
    std::vector<Short> dual(CAPACITY);
    for (int i = tiles.size() - 1; i >= 0; --i) {
        dual[tiles[i]] = rank & 31;
        rank >>= 5;
    }
    dual[0] = rank & 31;
    std::vector<Short> state(CAPACITY);
    for (Short tile: tiles) {
        state[dual[tile]] = tile;
    }
    return new State(state, dual, dual[0]);
}

void Heuristic::generatePDB() {
    PDB = std::vector<Short>(pick(CAPACITY, PDB_STATE_SIZE));
    std::vector<Short> goalState(CAPACITY);
    for (Short tile: tiles)
        goalState[tile] = tile;
    std::vector<Short> goalDual = goalState;
    Int goalRank = getRank(goalDual);
    PDB[goalRank] = 0;
    Long goalStateRank = getRankOfState(goalDual);
    queue.push(goalStateRank);
    visited.insert(goalStateRank);
    Int count = 1;
    while (count < PDB.size()) {
        if (queue.empty()) {
            std::cout << "Queue is empty" << std::endl;
            break;
        }
        std::cout << "Queue Size: " << queue.size() << std::endl;
        State* current = getStateOfRank(queue.front());
        queue.pop();
        std::vector<Short> state = std::move(current->getState());
        std::vector<Short> dual = std::move(current->getDual());
        Short currentH = PDB[getRank(dual)];
        Short currentBlank = current->getBlank();
        std::cout << "Current Blank: " << unsigned (currentBlank) << std::endl;
        for (Short neighbor: neighborCache->getNeighbors(currentBlank)) {
            std::swap(dual[0], dual[state[neighbor]]);
            Long newRank = getRankOfState(dual);
            if (visited.find(newRank) != visited.end()) {
                std::swap(dual[0], dual[state[neighbor]]);
                continue;
            }
            visited.insert(newRank);
            assert(newRank <= 1ULL<<35);
            queue.push(newRank);
            if (state[neighbor] != 0) {
                Int nextRank = getRank(dual);
                if (PDB[nextRank] == 0 && nextRank != goalRank) {
                    PDB[nextRank] = currentH + 1;
                    ++count;
                    std::cout << count << " " << visited.size() << std::endl;
                }
            }
            std::swap(dual[0], dual[state[neighbor]]);
        }
        delete current;
    }
    visited.clear();
    queue = {};
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
    while (file >> num) {
        PDB.push_back(num);
    }
    file.close();
}

Short Heuristic::getHeuristic(std::vector<Short>& dual) {
    return PDB[getRank(dual)];
}

Short Heuristic::getHeuristicOfSelectedDuals(std::vector<Short>& dual) {
    return PDB[getRankOfSelectedDuals(dual)];
}
