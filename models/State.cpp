//
// Created by reza on 07/04/23.
//

#include <iostream>
#include "../inc/State.h"

void State::swap(Short to) {
    Short from = this->blank;
    this->dual[this->state[to]] = from;
//    TODO: remove the next line as it is unnecessary
    this->dual[BLANK] = to;
    std::swap(this->state[to], this->state[from]);
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
            std::cout << unsigned(this->state[i * SIZE + j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Dual:" << std::endl;
    for (Short i = 0; i < SIZE; i++) {
        for (Short j = 0; j < SIZE; j++) {
            std::cout << unsigned(this->dual[i * SIZE + j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Blank: " << unsigned(this->blank) << std::endl;
    std::cout << "------------------" << std::endl;
}