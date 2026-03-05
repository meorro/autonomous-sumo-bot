#include "SumoBot.h"

void SumoBot::update(float distance_enemy_cm, bool ring_edge_detected, uint32_t current_time_ms) {
    // State transition logic
    if (is_evading && current_time_ms - evade_start_time < EVADE_DURATION_MS) {
        current_state = EVADE;
    }
    else {
        is_evading = false;
        if (ring_edge_detected) {
            evade_start_time = current_time_ms;
            current_state = EVADE;
            is_evading = true;
        } else if (distance_enemy_cm < ATTACK_THRESHOLD_CM) {
            current_state = ATTACK;
        } else {
            current_state = SEARCH;
        }
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