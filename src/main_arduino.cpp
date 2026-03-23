#include "SumoBot.h"

/* Start button */
#define PIN_START_SEL 2
#define PIN_KILL 3

/* Motors */
#define PIN_EN_A A6
#define PIN_IN1 7
#define PIN_IN2 6
#define PIN_IN3 5
#define PIN_IN4 4
#define PIN_EN_B A7

/* Front infrared sensor */
#define PIN_IR_OUT1 9

/* Back infrared sensor */
#define PIN_IR_OUT0 8

/* Ultrasonic sensor */
#define PIN_TRIG 12
#define PIN_ECHO 11

/* Speed of sound is 343 m/s --> 0.0343 cm/us */
const float SPEED_OF_SOUND_CM_US = 0.0343;

SumoBot sumobot;

void setup() {
    /* Start serial monitor for debugging */
    Serial.begin(9600);
    
    /* Configure pins as INPUT/INPUT_PULLUP or OUTPUT */
    /* 
     * Start button. Set to INPUT_PULLUP to avoid a
     * floating state when the button isn't pressed.
     */
    pinMode(PIN_START_SEL, INPUT_PULLUP);
    pinMode(PIN_KILL, INPUT_PULLUP);
    
    /* Motors */
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);
    pinMode(PIN_IN3, OUTPUT);
    pinMode(PIN_IN4, OUTPUT);

    /* Front infrared sensor */
    pinMode(PIN_IR_OUT1, INPUT);

    /* Back infrared sensor */
    pinMode(PIN_IR_OUT0, INPUT);

    /* Ultrasonic sensor */
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
}

void loop() {
    uint32_t current_time = millis();

    /*
     * Fire the sound wave:
     *  - Set PIN_TRIG to LOW to ensure clean state and no noise.
     *    Wait 2 microseconds for the electrons to drain out of the pin. 
     *  - To start measurement, PIN_TRIG must receive a pulse of HIGH for at least 10 microseconds.
     *    [ https://cdn.awsli.com.br/945/945993/arquivos/HCSR04.pdf ]
     *  - Set PIN_TRIG to LOW to end the trigger pulse.
     */
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    /* Get the duration of the sound wave travelling */
    unsigned long duration = pulseIn(PIN_ECHO, HIGH);
    /*
     *  If pulseIn times out, it returns 0.
     *  Default to 100cm (safe) if that happens.
     */
    float distance_cm = 100.0; 
    if (duration > 0) {
        distance_cm = duration * SPEED_OF_SOUND_CM_US / 2.0; /* Divide by 2 because the sound does a roundtrip */
    }

    /*
     * Read the start button and IR sensors.
     * The PIN_START_SEL pin is INPUT_PULLUP.
     * So LOW (0) means "button pressed". Invert it to true/false
     * boolean to be more intuitive and match the SumoBot.cpp logic.
     */
    bool start_pressed = (digitalRead(PIN_START_SEL) == LOW);
    bool front_edge_seen = digitalRead(PIN_IR_OUT1);
    bool back_edge_seen = digitalRead(PIN_IR_OUT0);

    sumobot.update(distance_cm, front_edge_seen, back_edge_seen, current_time, start_pressed);

    delay(10); 
}