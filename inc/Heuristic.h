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
    std::unordered_map<Long, Short> PDB;
    NeighborCache neighborCache;
public:
    Heuristic(std::vector<Short> tiles): tiles(std::move(tiles)) {
        this->neighborCache = NeighborCache();
        this->generatePDB();
        this->saveToFile("pdb.txt");
    };
    void saveToFile(const std::string& fileName);
    void readFromFile(const std::string& filename);
    Long getRank(const State& state) const;
    Short getHeuristic(const State& state) const;
    void generatePDB();
};


#endif //CODE_HEURISTIC_H
