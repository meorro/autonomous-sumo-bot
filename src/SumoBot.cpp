#include "SumoBot.h"

void SumoBot::update(float distance_enemy_cm, bool ring_edge_detected, uint32_t current_time_ms) {
    // Bot remains in the ESCAPE_EDGE state for 1.5 seconds
    // in order to have time get away from the edge. Otherwise,
    // it would switch to another state the moment the white line
    // is no longer sensed and thus going to the edge again
    if (is_evading && current_time_ms - escape_edge_start_time < ESCAPE_EDGE_DURATION_MS) {
        current_state = ESCAPE_EDGE;
    }
    else {
        is_evading = false;
        if (ring_edge_detected) {
            escape_edge_start_time = current_time_ms;
            current_state = ESCAPE_EDGE;
            is_evading = true;
        } else if (getFilteredDistance(distance_enemy_cm) < ATTACK_THRESHOLD_CM) {
            current_state = ATTACK;
        } else {
            current_state = SEARCH;
        }
    }

    if (current_state != previous_state) {
        std::cout << "[Time: " << current_time_ms << "ms] State changed to: ";
        printState();
        previous_state = current_state;
    }
}

std::ostream& operator<<(std::ostream& os, State state) {
    switch (state) {
        case State::SEARCH: os << "SEARCH"; break;
        case State::ATTACK: os << "ATTACK"; break;
        case State::ESCAPE_EDGE:  os << "ESCAPE_EDGE"; break;
        default:            os << "Unknown"; break;
    }
    return os;
}

void SumoBot::printState() {
    std::cout << current_state << std::endl;
}

// Moving average filter
float SumoBot::getFilteredDistance(float new_reading) {
    // Replace the oldest reading in the array with the new_reading
    distance_history[history_index] = new_reading;

    // Increment the history_index (and wrap it back to 0 if it hits 5)
    history_index = (history_index + 1) % 5;

    // Keep track of how many valid samples we have
    if (valid_samples < 5) {
        ++valid_samples;
    }

    // Sum all valid samples
    float distance_sum = 0;
    for (int i = 0; i < valid_samples; ++i) {
        distance_sum += distance_history[i];
    }

    // Return the average distance of all valid samples
    return distance_sum / valid_samples;
}