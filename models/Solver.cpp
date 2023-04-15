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
    std::cout << "start converting irregular tiles mirrored" << std::endl;
    for (auto tile: irregularTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        std::cout << "dual coord: <" << unsigned(dualCoord.first) << ", " << unsigned(dualCoord.second) << "> tile:" << unsigned(tile)
            << " dual: " << unsigned(dual[tile]) << " converted to <" << unsigned(dualCoord.second) << "," << unsigned(dualCoord.first)
            << "> or tile " << unsigned(getIndexFromCoordinate(dualCoord.second, dualCoord.first))
            << " with dual " << unsigned(dual[getIndexFromCoordinate(dualCoord.second, dualCoord.first)]) << std::endl;

        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.second, dualCoord.first)]);
    }
    heuristicValue += irregularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    std::cout << "irregular heuristic mirrored: " << unsigned(heuristicValue) << std::endl;
    convertedDuals.clear();
    std::cout << "start converting regular tiles down left mirror" << std::endl;
    std::vector<Short> downLeftTiles = {10, 15, 20, 11, 16, 21};
    for (auto &tile: downLeftTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        std::cout << "dual coord: <" << unsigned(dualCoord.first) << ", " << unsigned(dualCoord.second) << "> tile:" << unsigned(tile)
                  << " dual: " << unsigned(dual[tile]) << " converted to <" << unsigned(dualCoord.second) << "," << unsigned(dualCoord.first)
                  << "> or " << unsigned(dual[getIndexFromCoordinate(dualCoord.second, dualCoord.first)]) << std::endl;
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.second, dualCoord.first)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    std::cout << "regular heuristic down left mirrored: " << unsigned(heuristicValue) << std::endl;
    convertedDuals.clear();
    std::cout << "start converting regular tiles down right mirror" << std::endl;
    std::vector<Short> downRightTiles = {22, 23, 24, 17, 18, 19};
    for (auto &tile: downRightTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        std::cout << "dual coord: <" << unsigned(dualCoord.first) << ", " << unsigned(dualCoord.second) << "> tile:" << unsigned(tile)
                  << " dual: " << unsigned(dual[tile]) << " converted to <" << unsigned(dualCoord.first) << "," << unsigned(ZERO_BASED_SIZE - dualCoord.second)
                  << "> or " << unsigned(dual[getIndexFromCoordinate(dualCoord.first, ZERO_BASED_SIZE - dualCoord.second)]) << std::endl;
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.first, ZERO_BASED_SIZE - dualCoord.second)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    std::cout << "regular heuristic down right mirrored: " << unsigned(heuristicValue) << std::endl;
    convertedDuals.clear();
    std::cout << "start converting regular tiles top right mirror" << std::endl;
    std::vector<Short> topRightTiles = {14, 9, 4, 13, 8, 3};
    for (auto &tile: topRightTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        std::cout << "dual coord: <" << unsigned(dualCoord.first) << ", " << unsigned(dualCoord.second) << "> tile:" << unsigned(tile)
                  << " dual: " << unsigned(dual[tile]) << " converted to <" << unsigned(dualCoord.second) << "," << unsigned(ZERO_BASED_SIZE - dualCoord.first)
                  << "> or " << unsigned(dual[getIndexFromCoordinate(ZERO_BASED_SIZE - dualCoord.second, ZERO_BASED_SIZE - dualCoord.first)]) << std::endl;
        convertedDuals.push_back(dual[getIndexFromCoordinate(ZERO_BASED_SIZE - dualCoord.second, ZERO_BASED_SIZE - dualCoord.first)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    std::cout << "regular heuristic top right mirrored: " << unsigned(heuristicValue) << std::endl;
    convertedDuals.clear();
    return heuristicValue;
}

Short Solver::getHeuristicOfNormalTable(std::vector<Short> &dual) {
    Short heuristicValue = regularHeuristic->getHeuristic(dual);
    std::cout << "regular heuristic top left: " << unsigned(heuristicValue) << std::endl;
    heuristicValue += irregularHeuristic->getHeuristic(dual);
    std::cout << "irregular heuristic: " << unsigned(heuristicValue) << std::endl;
    std::vector<Short> convertedDuals;
    std::cout << "start converting regular tiles down right" << std::endl;
    std::vector<Short> downRightTiles = { 14, 19, 24, 13, 18, 23};
    for (auto &tile: downRightTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        std::cout << "dual coord: <" << unsigned(dualCoord.first) << ", " << unsigned(dualCoord.second) << "> tile:" << unsigned(tile)
                  << " dual: " << unsigned(dual[tile]) << " converted to <" << unsigned(dualCoord.second) << "," << unsigned(ZERO_BASED_SIZE - dualCoord.first)
                  << "> or " << unsigned(dual[getIndexFromCoordinate(dualCoord.second, ZERO_BASED_SIZE - dualCoord.first)]) << std::endl;
        convertedDuals.push_back(dual[getIndexFromCoordinate(dualCoord.second, ZERO_BASED_SIZE - dualCoord.first)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    std::cout << "regular heuristic down right: " << unsigned(heuristicValue) << std::endl;
    convertedDuals.clear();
    std::cout << "start converting regular tiles down left" << std::endl;
    std::vector<Short> downLeftTiles = {22, 21, 20, 17, 16, 15};
    for (auto &tile: downLeftTiles) {
        auto dualCoord = getCoordinateFromIndex(dual[tile]);
        std::cout << "dual coord: <" << unsigned(dualCoord.first) << ", " << unsigned(dualCoord.second) << "> tile:" << unsigned(tile)
                  << " dual: " << unsigned(dual[tile]) << " converted to <" << unsigned(ZERO_BASED_SIZE - dualCoord.first) << "," << unsigned(ZERO_BASED_SIZE - dualCoord.second)
                  << "> or " << unsigned(dual[getIndexFromCoordinate(ZERO_BASED_SIZE - dualCoord.first, ZERO_BASED_SIZE - dualCoord.second)]) << std::endl;
        convertedDuals.push_back(dual[getIndexFromCoordinate(ZERO_BASED_SIZE - dualCoord.first, ZERO_BASED_SIZE - dualCoord.second)]);
    }
    heuristicValue += regularHeuristic->getHeuristicOfSelectedDuals(convertedDuals);
    std::cout << "regular heuristic down left: " << unsigned(heuristicValue) << std::endl;
    return heuristicValue;
}

std::vector<Short> Solver::solve(State* state) {
    return std::vector<Short>();
}

inline Short Solver::getIndexFromCoordinate(Short x, Short y) const {
    return x + y * SIZE;
}

inline std::pair<Short, Short> Solver::getCoordinateFromIndex(Short index) const {
    return std::make_pair(index % SIZE, index / SIZE);
}