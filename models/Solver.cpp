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
    std::vector<Short> convertedDuals;
    convertedDuals.reserve(PDB_STATE_SIZE);
    std::vector<Short> irregularTiles = {5, 1, 6, 2, 7, 12};
    for (auto tile: irregularTiles) {
        Short dualTile = dual[tile];
        convertedDuals.push_back(dual[SIZE * (dualTile % SIZE) + dualTile / SIZE]);
    }
    heuristicValue += irregularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> downLeftTiles = {10, 15, 20, 11, 16, 21};
    for (auto &tile: downLeftTiles) {
        Short dualTile = dual[tile];
        convertedDuals.push_back(dual[SIZE * (dualTile % SIZE) + dualTile / SIZE]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> downRightTiles = {22, 23, 24, 17, 18, 19};
    for (auto &tile: downRightTiles) {
        Short dualTile = dual[tile];
        convertedDuals.push_back(dual[dualTile % SIZE + SIZE * (ZERO_BASED_SIZE - (dualTile / SIZE))]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> topRightTiles = {14, 9, 4, 13, 8, 3};
    for (auto &tile: topRightTiles) {
        Short dualTile = dual[tile];
        convertedDuals.push_back(dual[(ZERO_BASED_SIZE - (dualTile / SIZE)) + SIZE * (ZERO_BASED_SIZE - (dualTile % SIZE))]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    return heuristicValue;
}

Short Solver::getHeuristicOfNormalTable(std::vector<Short> &dual) {
    Short heuristicValue = regularHeuristic->getHeuristic(dual);
    heuristicValue += irregularHeuristic->getHeuristic(dual);
    std::vector<Short> convertedDuals;
    std::vector<Short> downRightTiles = { 14, 19, 24, 13, 18, 23};
    for (auto &tile: downRightTiles) {
        Short& dualTile = dual[tile];
        convertedDuals.push_back(dual[(dualTile / SIZE) + SIZE * (ZERO_BASED_SIZE - (dualTile % SIZE))]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> downLeftTiles = {22, 21, 20, 17, 16, 15};
    for (auto &tile: downLeftTiles) {
        Short& dualTile = dual[tile];
        convertedDuals.push_back(dual[(ZERO_BASED_SIZE - (dualTile % SIZE)) + SIZE * (ZERO_BASED_SIZE - (dualTile / SIZE))]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    return heuristicValue;
}

Short Solver::iterate(State *state, Int limit, Int gCost, Short previousBlank, std::vector<Short>& path) {
    this->expandedNodes++;
    bool goalIsNotChecked = true;
    Short min = MAX_INT;
    Short currentBlank = state->getBlank();
    for (auto neighbor: neighborCache->getNeighbors(currentBlank)) {
        if (neighbor == previousBlank) {
            continue;
        }
        this->generatedNodes++;
        path.push_back(neighbor);
        state->swap(neighbor);
        Short fCost = gCost + calculateHeuristic(*state);
        if (fCost > limit) {
            state->swap(currentBlank);
            path.pop_back();
            if (goalIsNotChecked) {
                if (state->isGoal()) {
                    return 0;
                } else goalIsNotChecked = false;
            }
            if (fCost < min) {
                min = fCost;
            }
            continue;
        }
        auto result = iterate(state, limit, gCost + 1, neighbor, path);
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
    Short limit = calculateHeuristic(*state);
    std::vector<Short> path = {state->getBlank()};
    while (true) {
        Short result = iterate(state, limit, 0, state->getBlank(), path);
        std::cout << "Limit: " << unsigned(limit) << ", expanded: " << this->expandedNodes << ", generated: " << this->generatedNodes << std::endl;
        if (result == 0) {
            return path;
        }
        limit = result;
    }
}
