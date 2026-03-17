#include <iostream>

const uint32_t ESCAPE_EDGE_DURATION_MS = 1500;
const float ATTACK_THRESHOLD_CM = 40.0;

enum State {
    SEARCH,
    ATTACK,
    ESCAPE_EDGE
};

class SumoBot {
private:
    State previous_state = SEARCH;
    State current_state = SEARCH;

    uint32_t escape_edge_start_time = 0;
    bool is_evading = false;

    float distance_history[5] = {0};
    int history_index = 0;
    float running_sum = 0.0;
    int valid_samples = 0; // Tracks how many readings we currently have (max 5)

public:
    void update(float distance_enemy_cm, bool ring_edge_detected, uint32_t current_time_ms);
    void printState();
    float getFilteredDistance(float new_reading);
};

std::ostream& operator<<(std::ostream& os, State state);