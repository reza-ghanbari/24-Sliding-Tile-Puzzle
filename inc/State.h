//
// Created by reza on 07/04/23.
//

#ifndef CODE_STATE_H
#define CODE_STATE_H


#include <utility>
#include <vector>
#include "Types.h"

class State {
private:
    std::vector<Short> state;
    Short blank;
public:
    State(std::vector<Short>& state, Short blank): state(std::move(state)) {};
    std::vector<Short> getBlankNeighbors() const;


};


#endif //CODE_STATE_H
