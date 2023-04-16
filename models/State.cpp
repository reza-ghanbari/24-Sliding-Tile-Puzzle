//
// Created by reza on 07/04/23.
//

#include <iostream>
#include <iomanip>
#include "../inc/State.h"

void State::swap(Short to) {
    std::swap(state[to], state[blank]);
    std::swap(dual[state[to]], dual[BLANK]);
    this->blank = to;
}

bool State::isGoal() const {
    for (Short i = 0; i < CAPACITY; i++) {
        if (this->state[i] != i) {
            return false;
        }
    }
    return true;
}

void State::printState() const {
    std::cout << "State:" << std::endl;
    for (Short i = 0; i < SIZE; i++) {
        for (Short j = 0; j < SIZE; j++) {
            if ((i * SIZE + j) == blank)
                std::cout << " * ";
            else
                std::cout << std::setw(2) << unsigned(this->state[i * SIZE + j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Dual:" << std::endl;
    for (Short i = 0; i < SIZE; i++) {
        for (Short j = 0; j < SIZE; j++) {
            std::cout << std::setw(2) << unsigned(this->dual[i * SIZE + j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Blank: " << unsigned(this->blank) << std::endl;
    std::cout << "------------------" << std::endl;
}