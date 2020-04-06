#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "configuration.h"

class Actuator {
    private:
        const byte in1Pin;
        const byte in2Pin;
        const byte enaPin;
        const byte feedbackPin;

        int maxPosition;
        int minPosition;
        int rawPosition;
        int filtPosition;
        int targetPosition;
        bool isCalibrated;
        int calibrationStage;

        bool isReady;
        int readings[SMOOTH];
        int total;
        int index;

        bool isSet;

        void off();
        void brake();

        void readPosition();

        unsigned long t_start;
        unsigned long t_end;
        unsigned long t_now;

        int pwm;
        void setPWM(float k);

        unsigned long movement[NUM_MAX_SWITCHES];
        int currPos; // current step in movement
        bool isExtending; // whether extending or retracting currently


    public:
        Actuator(byte attachToIn1Pin, byte attachToIn2Pin, byte attachToEnaPin, byte attachToFeedbackPin);

        void setup();
        void loop(float actuatorMaxPWM);
        void setInitialHeight(float initialHeight);

        void calibrate();
        void calibrate(uint16_t (&settings)[2]);
        void setMovements(int movements[]);
        void restartLoop(unsigned long t_startPlat, unsigned long t_endPlat);

        bool isActuatorReady();
        bool isActuatorSet();

        void extend();
        void extend(uint8_t pwm);
        void retract();
        void retract(uint8_t pwm);

        int getRawPosition();
        int getPosition();
        int getMaxPosition();
        int getMinPosition();

        void noCalibrate(int min_config, int max_config);
};

#endif
