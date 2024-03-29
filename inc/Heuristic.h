//
// Created by reza on 12/04/23.
//

#ifndef CODE_HEURISTIC_H
#define CODE_HEURISTIC_H


#include <vector>
#include <unordered_map>
#include <bitset>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <cassert>
#include "Types.h"
#include "State.h"
#include "Constants.h"

class Heuristic {
private:
    std::vector<Short> tiles;
    std::vector<Short> PDB;
    NeighborCache *neighborCache;
    Int onesCountLookup[LARGEST_NUMBER];
    Int picks[PDB_STATE_SIZE];
    Int visitedPicks[PDB_STATE_SIZE + 1];
    std::deque<Long> queue;
    std::vector<bool> visited;
    Int pick(Short n, Short k);
    void InitializeRankingCalculationTables();
public:
    Heuristic(std::vector<Short> tiles, NeighborCache *neighborCache)
        : tiles(std::move(tiles)), neighborCache(neighborCache) {
        InitializeRankingCalculationTables();
        this->generatePDB();
    }

    Heuristic(std::vector<Short> tiles, NeighborCache *neighborCache, const std::string &filename)
        : tiles(std::move(tiles)), neighborCache(neighborCache) {
        InitializeRankingCalculationTables();
        std::cout << "Reading PDB from file: " << filename << std::endl;
        this->readFromFile(filename);
    };

    void saveToFile(const std::string &fileName);

    void readFromFile(const std::string &filename);

    Int getRank(std::vector<Short> &dual) const;

    void generatePDB();

    Int getRankOfSelectedDuals(std::vector<Short> &dual) const;

    Short getHeuristicOfSelectedDuals(std::vector<Short> &dual);

    Short getHeuristic(std::vector<Short> &dual);

    Long getRankOfState(std::vector<Short> &dual);

    State *getStateOfRank(Long rank);

    Int getRankWithBlank(std::vector<Short> &dual) const;

    Int getRankOfSelectedDualsWithBlank(std::vector<Short> &dual) const;
};


#endif //CODE_HEURISTIC_H
