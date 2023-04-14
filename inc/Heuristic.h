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

    Int getRank(const State &state) const;

    Int getRank(std::vector<Short> &dual) const;

    Short getHeuristic(const State &state) const;

    void generatePDB();

    std::vector<Short> getGoal() const;
};


#endif //CODE_HEURISTIC_H
