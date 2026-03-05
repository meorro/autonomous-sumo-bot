#include <iostream>
#include <ostream>

enum State {
    SEARCH,
    ATTACK,
    EVADE
};

class SumoBot {
private:
    State current_state = SEARCH;
    void setState(State state);

public:
    void update(float distance_cm, bool edge_detected);
    void executeState();
    void printState();
};

std::ostream& operator<<(std::ostream& os, State state);