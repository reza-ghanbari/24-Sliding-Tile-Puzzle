//
// Created by reza on 12/04/23.
//

#ifndef CODE_HEURISTIC_H
#define CODE_HEURISTIC_H


#include <vector>
#include <unordered_map>
#include "Types.h"
#include "State.h"
#include "Constants.h"

class Heuristic {
private:
    std::vector<Short> tiles;
    std::unordered_map<Int, Short> PDB;
    NeighborCache *neighborCache;
public:
    Heuristic(std::vector<Short> tiles, NeighborCache *neighborCache)
        : tiles(std::move(tiles)), neighborCache(neighborCache) {
        this->generatePDB();
    };

    Heuristic(std::vector<Short> tiles, NeighborCache *neighborCache, const std::string &filename)
        : tiles(std::move(tiles)), neighborCache(neighborCache) {
        this->readFromFile(filename);
    };

    void saveToFile(const std::string &fileName);

    void readFromFile(const std::string &filename);

    Int getRank(std::vector<Short> &dual) const;

    void generatePDB();

    State* getGoal() const;

    Int getRankOfSelectedDuals(std::vector<Short> &dual) const;

    Short getHeuristicOfSelectedDuals(std::vector<Short> &dual);

    Short getHeuristic(std::vector<Short> &dual);

    Int getRank(State *state) const;
};


#endif //CODE_HEURISTIC_H
