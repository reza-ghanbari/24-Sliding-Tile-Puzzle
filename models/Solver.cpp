//
// Created by reza on 14/04/23.
//

#include <functional>
#include <iostream>
#include "../inc/Solver.h"

Short Solver::calculateHeuristic(State &state) {
    auto dual = state.getDual();
    return std::max(getHeuristicOfNormalTable(dual), getHeuristicOfMirroredTable(dual));
}

Short Solver::getHeuristicOfMirroredTable(std::vector<Short>& dual) {
    Short heuristicValue = 0;
    std::vector<Short> convertedDuals;
    std::vector<Short> irregularTiles = {5, 1, 6, 2, 7, 12};
    for (auto tile: irregularTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.second, dualCoord.first)]);
    }
    heuristicValue += irregularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> downLeftTiles = {10, 15, 20, 11, 16, 21};
    for (auto &tile: downLeftTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.second, dualCoord.first)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> downRightTiles = {22, 23, 24, 17, 18, 19};
    for (auto &tile: downRightTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.first, ZERO_BASED_SIZE - dualCoord.second)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> topRightTiles = {14, 9, 4, 13, 8, 3};
    for (auto &tile: topRightTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        convertedDuals.push_back(dual[getIndexFromCoordinate(ZERO_BASED_SIZE - dualCoord.second, ZERO_BASED_SIZE - dualCoord.first)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    return heuristicValue;
}

Short Solver::getHeuristicOfNormalTable(std::vector<Short> &dual) {
    Short heuristicValue = regularHeuristic->getHeuristic(dual);
    heuristicValue += irregularHeuristic->getHeuristic(dual);
    std::vector<Short> convertedDuals;
    std::vector<Short> downRightTiles = { 14, 19, 24, 13, 18, 23};
    for (auto &tile: downRightTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.second, ZERO_BASED_SIZE - dualCoord.first)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    convertedDuals.clear();
    std::vector<Short> downLeftTiles = {22, 21, 20, 17, 16, 15};
    for (auto &tile: downLeftTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        convertedDuals.push_back(dual[getIndexFromCoordinate(ZERO_BASED_SIZE - dualCoord.first, ZERO_BASED_SIZE - dualCoord.second)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    return heuristicValue;
}

Short Solver::iterate(State *state, Int limit, Int gCost, Short previousBlank, std::vector<Short>& path) {
    Short hCost = calculateHeuristic(*state);
    Short fCost = hCost + gCost;
    if (fCost > limit)
        return fCost;
    if (state->isGoal())
        return 0;
    Short min = MAX_INT;
    Short currentBlank = state->getBlank();
    for (auto neighbor: neighborCache->getNeighbors(currentBlank)) {
        if (neighbor == previousBlank) {
            continue;
        }
        path.push_back(neighbor);
        state->swap(neighbor);
        auto result = iterate(state, limit, gCost + 1, neighbor, path);
        if (result == 0) {
            return 0;
        }
        if (result < min) {
            min = result;
        }
        path.pop_back();
        state->swap(currentBlank);
    }
    return min;
}

std::vector<Short> Solver::solve(State* state) {
    Short limit = calculateHeuristic(*state);
    std::vector<Short> path = {state->getBlank()};
    while (true) {
        Short result = iterate(state, limit, 0, state->getBlank(), path);
        if (result == 0) {
            return path;
        }
        limit = result;
    }
}

inline Short Solver::getIndexFromCoordinate(Short x, Short y) const {
    return x + y * SIZE;
}

inline std::pair<Short, Short> Solver::getCoordinateFromIndex(Short index) const {
    return std::make_pair(index % SIZE, index / SIZE);
}