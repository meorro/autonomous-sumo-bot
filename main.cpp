#include "SumoBot.h"

int main() {
    SumoBot sumo_bot;

    std::cout << "--- Simulation Start ---\n";
    sumo_bot.update(100.0, false, 100); // Enemy far, no edge
    sumo_bot.update(30.0, false, 125);  // Enemy spotted!
    sumo_bot.update(10.0, true, 150);   // Pushed to the edge!

    
    sumo_bot.update(20.0, true, 175);
    sumo_bot.update(40.0, false, 200);
    sumo_bot.update(70.0, true, 225);
    sumo_bot.update(30.0, false, 1649);
    sumo_bot.update(30.0, false, 1650);

    return 0;
}