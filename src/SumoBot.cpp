#include "SumoBot.h"

void SumoBot::setMotors(int left_speed, int right_speed) {
#ifndef ARDUINO
    std::cout << "L: " << left_speed << " | R: " << right_speed << std::endl;
#else
    Serial.print("L: ");
    Serial.print(left_speed);
    Serial.print(" | R: ");
    Serial.println(right_speed);
#endif
}

void SumoBot::update(float distance_enemy_cm, bool ring_edge_front_detected, bool ring_edge_back_detected,
                     uint32_t current_time_ms, bool start_button_pressed) {
    /*
     * The bot remains in the IDLE state if not in a match
     */
    if (!match_started) {
        if (start_button_pressed) {
            start_button_pressed_time_ms = current_time_ms;
            match_started = true;
        } else {
            current_state = IDLE;
        }
    }
    if (match_started) {
        /*
         * Per the mini-sumo rules, the bot should in the IDLE state
         * for at least IDLE_DURATION_MS after being started
         */
        if (current_time_ms - start_button_pressed_time_ms < IDLE_DURATION_MS) {
            current_state = IDLE;
        }

        /* 
         *  The bot remains in the ESCAPE_EDGE_FRONT/ESCAPE_EDGE_BACK state
         *  for ESCAPE_EDGE_DURATION_MS in order to have time get away from
         *  the edge. Otherwise, it would switch to another state the moment
         *  the white line is no longer sensed and thus going to the edge again.
         */
        else {
            if (is_evading_front && current_time_ms - escape_edge_start_time_ms < ESCAPE_EDGE_DURATION_MS) {
                current_state = ESCAPE_EDGE_FRONT;
            } else if (is_evading_back && current_time_ms - escape_edge_start_time_ms < ESCAPE_EDGE_DURATION_MS) {
                current_state = ESCAPE_EDGE_BACK;
            } else {
                is_evading_front = false;
                is_evading_back = false;
                if (ring_edge_front_detected) {
                    escape_edge_start_time_ms = current_time_ms;
                    current_state = ESCAPE_EDGE_FRONT;
                    is_evading_front = true;
                } else if (ring_edge_back_detected) {
                    escape_edge_start_time_ms = current_time_ms;
                    current_state = ESCAPE_EDGE_BACK;
                    is_evading_back = true;
                } else if (getFilteredDistance(distance_enemy_cm) < ATTACK_THRESHOLD_CM) {
                    current_state = ATTACK;
                } else {
                    current_state = SEARCH;
                }
            }
        }
    }

    if (current_state != previous_state) {
#ifndef ARDUINO
        std::cout << "[Time: " << current_time_ms << "ms] State changed from " << previous_state << " to " << current_state << std::endl;
#else
    Serial.print("[Time: ");
    Serial.print(current_time_ms);
    Serial.print("ms] State changed from ");
    Serial.print(stateToString(previous_state));
    Serial.print(" to ");
    Serial.println(stateToString(current_state));
#endif
        previous_state = current_state;
    }

    /* Action logic */
    executeState(current_time_ms);
}

void SumoBot::executeState(uint32_t current_time_ms) {
    switch(current_state) {
        case IDLE:
#ifndef ARDUINO
            std::cout << "[Time: " << current_time_ms << "ms] IDLE: ";
#else
            Serial.print("[Time: ");
            Serial.print(current_time_ms);
            Serial.print("ms] IDLE: ");
#endif
            setMotors(0, 0);
            break;
        case SEARCH:
#ifndef ARDUINO
            std::cout << "[Time: " << current_time_ms << "ms] SEARCH: ";
#else
            Serial.print("[Time: ");
            Serial.print(current_time_ms);
            Serial.print("ms] SEARCH: ");
#endif
            setMotors(150, -150);
            break;
        case ATTACK:
#ifndef ARDUINO
            std::cout << "[Time: " << current_time_ms << "ms] ATTACK: ";
#else
            Serial.print("[Time: ");
            Serial.print(current_time_ms);
            Serial.print("ms] ATTACK: ");
#endif
            setMotors(255, 255);
            break;
        case ESCAPE_EDGE_FRONT:
#ifndef ARDUINO
            std::cout << "[Time: " << current_time_ms << "ms] ESCAPE_EDGE_FRONT: ";
#else
            Serial.print("[Time: ");
            Serial.print(current_time_ms);
            Serial.print("ms] ESCAPE_EDGE_FRONT: ");
#endif
            setMotors(-255, -255);
            break;
        case ESCAPE_EDGE_BACK:
#ifndef ARDUINO
            std::cout << "[Time: " << current_time_ms << "ms] ESCAPE_EDGE_BACK: ";
#else
            Serial.print("[Time: ");
            Serial.print(current_time_ms);
            Serial.print("ms] ESCAPE_EDGE_BACK: ");
#endif
            setMotors(255, 255);
            break;
        default:
#ifndef ARDUINO
            std::cout << "[Time: " << current_time_ms << "ms] UNKNOWN STATE" << std::endl;
#else
            Serial.print("[Time: ");
            Serial.print(current_time_ms);
            Serial.print("ms] UNKNOWN STATE: ");
#endif
            break;
    }
}

#ifndef ARDUINO
std::ostream& operator<<(std::ostream& os, State state) {
    switch (state) {
        case State::IDLE:               os << "IDLE"; break;
        case State::SEARCH:             os << "SEARCH"; break;
        case State::ATTACK:             os << "ATTACK"; break;
        case State::ESCAPE_EDGE_FRONT:  os << "ESCAPE_EDGE_FRONT"; break;
        case State::ESCAPE_EDGE_BACK:   os << "ESCAPE_EDGE_BACK"; break;
        default:                        os << "UNKNOWN"; break;
    }
    return os;
}
#else
const char* stateToString(State state) {
  switch (state) {
    case State::IDLE:              return "IDLE";
    case State::SEARCH:            return "SEARCH";
    case State::ATTACK:            return "ATTACK";
    case State::ESCAPE_EDGE_FRONT: return "ESCAPE_EDGE_FRONT";
    case State::ESCAPE_EDGE_BACK:  return "ESCAPE_EDGE_BACK";
    default:                       return "UNKNOWN";
  }
}
#endif

void SumoBot::printCurrentState() {
#ifndef ARDUINO
    std::cout << current_state << std::endl;
#else
    Serial.println(stateToString(current_state));
#endif
}

/* Moving average filter */
float SumoBot::getFilteredDistance(float new_reading) {
    /* Subtract the oldest value that is about to be overwritten */
    running_sum -= distance_history[history_index];

    /* Add the new value */
    running_sum += new_reading;
    
    /* Replace the oldest reading in the array with the new_reading */
    distance_history[history_index] = new_reading;

    /* Increment the history_index (and wrap it back to 0 if it hits 5) */
    history_index = (history_index + 1) % 5;

    /* Keep track of how many valid samples we have */
    if (valid_samples < 5) {
        ++valid_samples;
    }

    /* Return the average distance of all valid samples */
    return running_sum / valid_samples;
}