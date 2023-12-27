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
    for (unsigned i = 1; i < PDB_STATE_SIZE + 2; ++i)
        visitedPicks[i - 1] = pick(CAPACITY - i, PDB_STATE_SIZE - i + 1);
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

Int Heuristic::getRankOfSelectedDualsWithBlank(std::vector<Short>& dual) const {
    Short lehmer[PDB_STATE_SIZE + 1];
    std::bitset<CAPACITY> seen;
    lehmer[0] = dual[0];
    seen[CAPACITY - 1 - dual[0]] = true;
    Int rank = lehmer[0] * this->visitedPicks[0];
    for (unsigned i = 1; i < PDB_STATE_SIZE + 1; ++i)
    {
        seen[CAPACITY - 1 - dual[i]] = true;
        unsigned numOnes = this->onesCountLookup[seen.to_ulong() >> (CAPACITY - dual[i])];
        lehmer[i] = dual[i] - numOnes;
        rank += lehmer[i] * this->visitedPicks[i];
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

Int Heuristic::getRankWithBlank(std::vector<Short>& dual) const {
    std::vector<Short> selectedDuals;
    selectedDuals.reserve(tiles.size() + 1);
    selectedDuals.push_back(dual[0]);
    for (short tile: tiles) {
        selectedDuals.push_back(dual[tile]);
    }
    return getRankOfSelectedDualsWithBlank(selectedDuals);
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
    visited = std::vector<bool>(pick(CAPACITY, PDB_STATE_SIZE + 1));
    std::vector<Short> goalState(CAPACITY);
    for (Short tile: tiles)
        goalState[tile] = tile;
    std::vector<Short> goalDual = goalState;
    Int goalRank = getRank(goalDual);
    PDB[goalRank] = 0;
    queue.push_front(getRankOfState(goalDual));
    visited[getRankWithBlank(goalDual)] = true;
    Int count = 1;
    int creationPercentage = 1;
    int onePercentSize = int(PDB.size() / 100);
    while (count < PDB.size()) {
        if (queue.empty()) [[unlikely]] {
            std::cout << "Queue is empty" << std::endl;
            std::cout << "Visited: " << visited.size() << std::endl;
            break;
        }
        State* current = getStateOfRank(queue.front());
        queue.pop_front();
        std::vector<Short> state = std::move(current->getState());
        std::vector<Short> dual = std::move(current->getDual());
        Short currentH = PDB[getRank(dual)];
        Short currentBlank = dual[0];
        for (Short neighbor: neighborCache->getNeighbors(currentBlank)) {
            dual[state[neighbor]] = currentBlank;
            dual[0] = neighbor;
            Int visitedRank = getRankWithBlank(dual);
            if (visited[visitedRank]) {
                dual[state[neighbor]] = neighbor;
                dual[0] = currentBlank;
                continue;
            }
            visited[visitedRank] = true;
            Long rankOfChildState = getRankOfState(dual);
            if (state[neighbor] != 0) {
                queue.push_back(rankOfChildState);
                Int nextRank = getRank(dual);
                if (PDB[nextRank] == 0 && nextRank != goalRank) {
                    PDB[nextRank] = currentH + 1;
                    ++count;
                    if (count == creationPercentage * onePercentSize) {
                        std::cout << "Generated " << creationPercentage << "% of PDB" << std::endl;
                        ++creationPercentage;
                    }
                }
            } else {
                queue.push_front(rankOfChildState);
            }
            dual[state[neighbor]] = neighbor;
            dual[0] = currentBlank;
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
