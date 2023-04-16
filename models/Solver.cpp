//
// Created by reza on 14/04/23.
//

#include <functional>
#include <iostream>
#include "../inc/Solver.h"

Short Solver::calculateHeuristic(State &state) {
    auto dual = state.getDual();
    return std::max(getHeuristicOfNormalTable(dual), getHeuristicOfReflectedTable(dual));
}

Short Solver::getHeuristicOfReflectedTable(std::vector<Short>& dual) {
    Short heuristicValue = 0;
    std::vector<Short> convertedDuals(PDB_STATE_SIZE);
    int i = 0;
    convertedDuals.reserve(PDB_STATE_SIZE);
    for (auto tile: reflectedDisjointTables[0]) {
        Short dualTile = dual[tile];
        convertedDuals[i++] = SIZE * (dualTile % SIZE) + dualTile / SIZE;
    }
    heuristicValue += irregularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    i = 0;
    for (auto &tile: reflectedDisjointTables[1]) {
        Short dualTile = dual[tile];
        convertedDuals[i++] = SIZE * (dualTile % SIZE) + dualTile / SIZE;
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    i = 0;
    for (auto &tile: reflectedDisjointTables[2]) {
        Short dualTile = dual[tile];
        convertedDuals[i++] = dualTile % SIZE + SIZE * (ZERO_BASED_SIZE - (dualTile / SIZE));
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    i = 0;
    for (auto &tile: reflectedDisjointTables[3]) {
        Short dualTile = dual[tile];
        convertedDuals[i++] = (ZERO_BASED_SIZE - (dualTile / SIZE)) + SIZE * (ZERO_BASED_SIZE - (dualTile % SIZE));
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    return heuristicValue;
}

Short Solver::getHeuristicOfNormalTable(std::vector<Short> &dual) {
    Short heuristicValue = regularHeuristic->getHeuristic(dual);
    heuristicValue += irregularHeuristic->getHeuristic(dual);
    std::vector<Short> convertedDuals(6);
    int i = 0;
    for (auto &tile: normalDisjointTables[0]) {
        Short dualTile = dual[tile];
        convertedDuals[i++] = (dualTile / SIZE) + SIZE * (ZERO_BASED_SIZE - (dualTile % SIZE));
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    i = 0;
    for (auto &tile: normalDisjointTables[1]) {
        Short dualTile = dual[tile];
        convertedDuals[i++] = (ZERO_BASED_SIZE - (dualTile % SIZE)) + SIZE * (ZERO_BASED_SIZE - (dualTile / SIZE));
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    return heuristicValue;
}

Short Solver::iterate(State *state, Int limit, Short previousBlank, std::vector<Short>& path) {
    this->expandedNodes++;
    if (state->isGoal())
        return 0;
    Short min = MAX_INT;
    Short currentBlank = state->getBlank();
    for (auto neighbor: neighborCache->getNeighbors(currentBlank)) {
        this->generatedNodes++;
        if (neighbor == previousBlank) {
            continue;
        }
        path.push_back(neighbor);
        state->swap(neighbor);
        Short childFCost = calculateHeuristic(*state) + path.size() - 1;
        if (childFCost > limit) {
            if (childFCost < min)
                min = childFCost;
            state->swap(currentBlank);
            path.pop_back();
            continue;
        }
        auto result = iterate(state, limit, currentBlank, path);
        if (result == 0) {
            return 0;
        }
        if (result < min) {
            min = result;
        }
        state->swap(currentBlank);
        path.pop_back();
    }
    return min;
}

std::vector<Short> Solver::solve(State* state) {
    this->expandedNodes = 0;
    this->generatedNodes = 0;
    Short limit = calculateHeuristic(*state);
    std::vector<Short> path = {state->getBlank()};
    while (true) {
        Short result = iterate(state, limit, state->getBlank(), path);
        std::cout << "limit: " << unsigned(limit) << ", expanded: " << this->expandedNodes << ", generated: " << this->generatedNodes << std::endl;
        if (result == 0) {
            return path;
        }
        limit = result;
    }
}
