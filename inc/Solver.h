//
// Created by reza on 14/04/23.
//

#ifndef CODE_SOLVER_H
#define CODE_SOLVER_H

#include "Heuristic.h"

class Solver {
private:
    Heuristic *regularHeuristic;
    Heuristic *irregularHeuristic;
    NeighborCache *neighborCache;
    Long expandedNodes = 0;
    Long generatedNodes = 0;
    std::vector<std::vector<Short>> reflectedDisjointTables = {
            {5, 1, 6, 2, 7, 12},
            {10, 15, 20, 11, 16, 21},
            {22, 23, 24, 17, 18, 19},
            {14, 9, 4, 13, 8, 3}
    };
    std::vector<std::vector<Short>> normalDisjointTables = {
            {14, 19, 24, 13, 18, 23},
            {22, 21, 20, 17, 16, 15}
    };

public:

    Solver(Heuristic *regularHeuristic, Heuristic *irregularHeuristic, NeighborCache *neighborCache)
            : regularHeuristic(regularHeuristic), irregularHeuristic(irregularHeuristic),
              neighborCache(neighborCache) {};

    std::vector<Short> solve(State *state);

    Short calculateHeuristic(State &state);

    Short getHeuristicOfNormalTable(std::vector<Short> &dual);

    Short getHeuristicOfReflectedTable(std::vector<Short>& dual);

    Short iterate(State *state, Int limit, Int gCost, Short previousBlank, std::vector<Short> &path);
};


#endif //CODE_SOLVER_H
