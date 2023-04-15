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

    inline Short getIndexFromCoordinate(Short x, Short y) const;

    inline std::pair<Short, Short> getCoordinateFromIndex(Short index) const;

public:

    Solver(Heuristic *regularHeuristic, Heuristic *irregularHeuristic, NeighborCache *neighborCache)
            : regularHeuristic(regularHeuristic), irregularHeuristic(irregularHeuristic),
              neighborCache(neighborCache) {};

    std::vector<Short> solve(State *state);

    Short calculateHeuristic(State &state);

    Short getHeuristicOfNormalTable(std::vector<Short> &dual);

    Short getHeuristicOfMirroredTable(std::vector<Short>& dual);
};


#endif //CODE_SOLVER_H
