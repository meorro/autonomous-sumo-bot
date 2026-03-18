#include <iostream>

const uint32_t IDLE_DURATION_MS = 5000;
const uint32_t ESCAPE_EDGE_DURATION_MS = 1500;
const float ATTACK_THRESHOLD_CM = 40.0;

enum State {
    IDLE,
    SEARCH,
    ATTACK,
    ESCAPE_EDGE_FRONT,
    ESCAPE_EDGE_BACK
};

class SumoBot {
private:
    State previous_state = IDLE;
    State current_state = IDLE;

    bool match_started = false;
    uint32_t start_button_pressed_time_ms = 0;

    uint32_t escape_edge_start_time_ms = 0;
    bool is_evading_front = false;
    bool is_evading_back = false;

    float distance_history[5] = {0};
    int history_index = 0;
    int valid_samples = 0; /* Tracks how many readings we currently have (max 5) */
    float running_sum = 0.0; /* Running sum of all valid samples */

    void executeState(uint32_t current_time_ms);
    void setMotors(int left_speed, int right_speed);

public:
    void update(float distance_enemy_cm, bool ring_edge_front_detected, bool ring_edge_back_detected,
                uint32_t current_time_ms, bool start_button_pressed);
    void printCurrentState();
    float getFilteredDistance(float new_reading);
};

std::ostream& operator<<(std::ostream& os, State state);