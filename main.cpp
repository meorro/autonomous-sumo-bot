#include "SumoBot.h"

int main() {
    SumoBot sumo_bot;

    std::cout << "--- Simulation Start ---\n";
    sumo_bot.update(100.0, false); // Enemy far, no edge
    sumo_bot.update(30.0, false);  // Enemy spotted!
    sumo_bot.update(10.0, true);   // Pushed to the edge!

}