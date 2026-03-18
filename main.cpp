#include "SumoBot.h"

int main() {
    SumoBot sumo_bot;
    
    std::cout << "--- Simulation Start ---\n";

    /* Simulate 15 seconds of a match, ticking every 100 milliseconds */
    for (uint32_t current_time_ms = 0; current_time_ms <= 15000; current_time_ms += 100) {
        
        /* Default environment:
         *      match not started,
         *      safe,
         *      nothing around,
         *      resets at every tick
         */
        bool start_button_pressed = false;
        float distance_enemy_cm = 100.0; 
        bool ring_edge_front_detected = false;
        bool ring_edge_back_detected = false; 

        /* At 2000ms, start the match */
        if (current_time_ms == 2000) {
            start_button_pressed = true;
        }

        /* At 8000ms, an enemy appears in front */
        if (current_time_ms >= 8000 && current_time_ms < 8500) {
            distance_enemy_cm = 20.0;
        }
        
        /* At 9000ms, we get pushed to the front edge */
        if (current_time_ms == 9000) {
            ring_edge_front_detected = true;
        }

        /* At 11000ms, enemy appears again */
        if (current_time_ms >= 11000 && current_time_ms <= 11500) {
            distance_enemy_cm = 15.0;
        }

        /* At 13000ms, we get pushed to the back edge */
        if (current_time_ms == 13000) {
            ring_edge_back_detected = true;
        }

        sumo_bot.update(distance_enemy_cm, ring_edge_front_detected, ring_edge_back_detected,
                        current_time_ms, start_button_pressed);
        start_button_pressed = false;
    }

    std::cout << "--- Simulation End ---\n";
    return 0;
}