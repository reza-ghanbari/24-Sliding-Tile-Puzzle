//
// Created by reza on 07/04/23.
//

#include "../inc/NeighborCache.h"

void NeighborCache::initNeighbors() {
    neighbors = std::vector<std::vector<Short>>(CAPACITY);
    for (int blankIdx = 0; blankIdx < CAPACITY; ++blankIdx) {
        Short row = blankIdx / SIZE;
        Short col = blankIdx % SIZE;
        if (row > 0) {
            neighbors[blankIdx].push_back(blankIdx - SIZE);
        }
        if (row < SIZE - 1) {
            neighbors[blankIdx].push_back(blankIdx + SIZE);
        }
        if (col > 0) {
            neighbors[blankIdx].push_back(blankIdx - 1);
        }
        if (col < SIZE - 1) {
            neighbors[blankIdx].push_back(blankIdx + 1);
        }
    }
}

NeighborCache::NeighborCache() {
    initNeighbors();
}

const std::vector<Short>& NeighborCache::getNeighbors(Short index) const {
    return neighbors[index];
}

