#include "SumoBot.h"

int main() {
    SumoBot sumo_bot;
    
    std::cout << "--- Simulation Start ---\n";

    // Simulate 3 seconds of a match, ticking every 50 milliseconds
    for (uint32_t time_ms = 0; time_ms <= 3000; time_ms += 50) {
        
        // Default environment: Safe, nothing around, resets at every tick
        float enemy_dist_cm = 100.0; 
        bool edge_seen = false;   

        // At 500ms, an enemy appears in front
        if (time_ms >= 500 && time_ms < 1000) {
            enemy_dist_cm = 20.0; 
        }
        
        // At 1000ms, we get pushed to the edge
        if (time_ms == 1000) {
            edge_seen = true;
        }

        // At 2000ms, enemy appears again
        if (time_ms >= 2000 && time_ms <= 2500) {
            enemy_dist_cm = 15.0;
        }

        sumo_bot.update(enemy_dist_cm, edge_seen, time_ms);
    }

    std::cout << "--- Simulation End ---\n";
    return 0;
}