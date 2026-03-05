#include <iostream>
#include <ostream>

const uint32_t EVADE_DURATION_MS = 1500;
const float ATTACK_THRESHOLD_CM = 40.0;

enum State {
    SEARCH,
    ATTACK,
    EVADE
};

class SumoBot {
private:
    State current_state = SEARCH;

    uint32_t evade_start_time = 0;
    bool is_evading = false;

public:
    void update(float distance_enemy_cm, bool ring_edge_detected, uint32_t current_time_ms);
    void executeState();
    void printState();
};

std::ostream& operator<<(std::ostream& os, State state);