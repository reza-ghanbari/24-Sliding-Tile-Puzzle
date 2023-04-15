//
// Created by reza on 07/04/23.
//

#include "../inc/NeighborCache.h"

void NeighborCache::initNeighbors() {
    neighbors = std::vector<std::vector<Short>>(CAPACITY);
    for (int index = 0; index < CAPACITY; ++index) {
        Short row = index / SIZE;
        Short col = index % SIZE;
        if (row > 0) {
            neighbors[index].push_back(index - SIZE);
        }
        if (row < ZERO_BASED_SIZE) {
            neighbors[index].push_back(index + SIZE);
        }
        if (col > 0) {
            neighbors[index].push_back(index - 1);
        }
        if (col < ZERO_BASED_SIZE) {
            neighbors[index].push_back(index + 1);
        }
    }
}

NeighborCache::NeighborCache() {
    initNeighbors();
}

const std::vector<Short>& NeighborCache::getNeighbors(Short index) const {
    return neighbors[index];
}

