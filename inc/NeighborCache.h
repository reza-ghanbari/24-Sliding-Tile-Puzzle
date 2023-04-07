//
// Created by reza on 07/04/23.
//

#ifndef CODE_NEIGHBORCACHE_H
#define CODE_NEIGHBORCACHE_H


#include "Constants.h"
#include <vector>
#include "Types.h"

class NeighborCache {
private:
    std::vector<std::vector<Short>> neighbors;
    void initNeighbors();
public:
    NeighborCache();
    [[nodiscard]] const std::vector<Short>& getNeighbors(Short index) const;
};


#endif //CODE_NEIGHBORCACHE_H
