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
