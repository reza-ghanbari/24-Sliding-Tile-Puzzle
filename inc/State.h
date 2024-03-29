//
// Created by reza on 07/04/23.
//

#ifndef CODE_STATE_H
#define CODE_STATE_H


#include <utility>
#include <vector>
#include "Types.h"
#include "NeighborCache.h"

class State {
private:
    std::vector<Short> state;
    std::vector<Short> dual;
    Short blank;
public:
    State(std::vector<Short>& state, std::vector<Short>& dual, Short blank): state(std::move(state)), dual(std::move(dual)), blank(blank) {};
    std::vector<Short> getState() { return state; };
    std::vector<Short> getDual() { return dual; };
    void swap(Short to);
    [[nodiscard]] Short getBlank() const { return blank; };

    bool isGoal() const;

    void printState() const;
};


#endif //CODE_STATE_H
