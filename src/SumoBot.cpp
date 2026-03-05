#include "SumoBot.h"

void SumoBot::update(float distance_cm, bool edge_detected) {
    // State transition logic
    if (edge_detected) {
        setState(EVADE);
    } else if (distance_cm < 40.0) {
        setState(ATTACK);
    } else {
        setState(SEARCH);
    }

     // Action logic
    executeState();
}

void SumoBot::executeState() {
    switch(current_state) {
        case SEARCH:
            std::cout << "SEARCHING..." << std::endl;
            break;
        case ATTACK:
            std::cout << "ATTACKING..." << std::endl;
            break;
        case EVADE:
            std::cout << "EVADING..." << std::endl;
            break;
        default:
            std::cout << "UNKNOWN STATE..." << std::endl;
    }
}

void SumoBot::setState(State state) {
    current_state = state;
}

std::ostream& operator<<(std::ostream& os, State state) {
    switch (state) {
        case State::SEARCH: os << "SEARCH"; break;
        case State::ATTACK: os << "ATTACK"; break;
        case State::EVADE:  os << "EVADE"; break;
        default:            os << "Unknown"; break;
    }
    return os;
}

void SumoBot::printState() {
    std::cout << current_state << std::endl;
}