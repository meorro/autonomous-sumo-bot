#include "SumoBot.h"

int main() {
    SumoBot sumo_bot;
    
    std::cout << "--- Simulation Start ---\n";

    /* Simulate 10 seconds of a match, ticking every 100 milliseconds */
    for (uint32_t current_time_ms = 0; current_time_ms <= 10000; current_time_ms += 100) {
        
        /* Default environment:
         *      match not started,
         *      safe,
         *      nothing around,
         *      resets at every tick
         */
        bool start_button_pressed = false;
        float distance_enemy_cm = 100.0; 
        bool ring_edge_detected = false; 

        /* At 2000ms, start the match */
        if (current_time_ms == 2000) {
            start_button_pressed = true;
        }

        /* At 5500ms, an enemy appears in front */
        if (current_time_ms >= 5500 && current_time_ms < 6000) {
            distance_enemy_cm = 20.0;
        }
        
        /* At 6000ms, we get pushed to the edge */
        if (current_time_ms == 6000) {
            ring_edge_detected = true;
        }

        /* At 7000ms, enemy appears again */
        if (current_time_ms >= 7000 && current_time_ms <= 8500) {
            distance_enemy_cm = 15.0;
        }

        sumo_bot.update(distance_enemy_cm, ring_edge_detected, current_time_ms, start_button_pressed);
        start_button_pressed = false;
    }

    std::cout << "--- Simulation End ---\n";
    return 0;
}