#include "SumoBot.h"

int main() {
    SumoBot sumo_bot;
    
    std::cout << "--- Simulation Start ---\n";

    // Simulate 3 seconds of a match, ticking every 50 milliseconds
    for (uint32_t current_time_ms = 0; current_time_ms <= 3000; current_time_ms += 50) {
        
        // Default environment: Safe, nothing around, resets at every tick
        float distance_enemy_cm = 100.0; 
        bool ring_edge_detected = false;   

        // At 500ms, an enemy appears in front
        if (current_time_ms >= 500 && current_time_ms < 1000) {
            distance_enemy_cm = 20.0;
        }
        
        // At 1000ms, we get pushed to the edge
        if (current_time_ms == 1000) {
            ring_edge_detected = true;
        }

        // At 2000ms, enemy appears again
        if (current_time_ms >= 2000 && current_time_ms <= 2500) {
            distance_enemy_cm = 15.0;
        }

        sumo_bot.update(distance_enemy_cm, ring_edge_detected, current_time_ms);
    }

    std::cout << "--- Simulation End ---\n";
    return 0;
}