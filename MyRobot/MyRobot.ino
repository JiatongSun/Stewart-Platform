/*
 * Stewart Platform Mechatronics Robot
 * Authors: Sanjana Rao, Sydney Miller, Mona Lee
 * Work done under Professor Michelle Lopez, Penn Design
 * Heavily references code by Michelle Rosa (https://github.com/michaeljrosa/stewart-platform) and Leroy Sibanda
 */


#include "configuration.h"
#include "Actuator.h"
#include "Platform.h"
#include "eeprom.h"

// amount of time to loop through
int travelTime = 26;
// controls and switches extend/retract for each actuator based on time, starts on extend
int actuatorMovements[6][NUM_MAX_SWITCHES] = {
    {7, 21, 28},   // extend first
    {0, 7, 21},  // retract first
    {0, 7, 21},
    {7, 21, 28},
    {0, 7, 21},
    {7, 21, 28}
};

// int actuatorMovements[6][NUM_MAX_SWITCHES] = {
//     {0, 6, 21},
//     {0, 6, 21},
//     {5, 15, 25},
//     {6, 14, 23},
//     {10, 25, 30},
//     {9, 22, 28}
// };

Platform platform;

void setup() {
    Serial.begin(9600);
    analogReference(EXTERNAL);
    pinMode(SHDN_BTN, INPUT_PULLUP);

    platform.setup();
    platform.setLoopDuration(travelTime);
    platform.setActuatorMovements(actuatorMovements);
}

void loop() {
    if(digitalRead(SHDN_BTN) == LOW) {
        platform.retract();
    }
    else {
        platform.loop();
    }
}
