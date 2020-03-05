#include "Platform.h"
#include "eeprom.h"

Platform::Platform() {
    isCalibrated = false;
    isSet = false;
    isReady = false;
    translation.Set(0, 0, 0);
    rotation.Set(0, 0, 0);
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        l[i].Set(0, 0, 0);
    }
}

void Platform::setup() {
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        actuators[i].setup();
    }
    initialHeight.Set(0, 0, INITIAL_HEIGHT);
    // loadConfig(*this);
    noCalibrate();


    Serial.println("finish platform setup");
}

void Platform::loop() {
    if (!isPlatformSet()) { // if platform hasn't been set at the starting height
        Serial.println("setting initial height");
        for (int i = 0; i < NUM_ACTUATORS; i++) {
          // set height for actuators
          actuators[i].setInitialHeight();
      }
    }
    else if (isPlatformReady()) { // if platform has reached the end of the movement loop then restart the loop
//        Serial.println("end of platform loop");

        t_start = millis();
        t_end = t_start + duration;

        for (int i = 0; i < NUM_ACTUATORS; i++) {
            actuators[i].restartLoop(t_start, t_end);
        }
    }
    else {
        for (int i = 0; i < NUM_ACTUATORS; i++) {
            actuators[i].loop();
        }
    }
}

void Platform::calibrate() {
    Serial.println("standard platform calibration!");
    for (int stage = 0; stage <= NUM_CALIB_STAGES; stage++) {
        Serial.print("calibration stage: ");
        Serial.println(stage);
        for (int i = 0; i < NUM_ACTUATORS; i++) {
            actuators[i].calibrate();
            // Serial.print("actuator ");
            // Serial.print(i+1);
            // Serial.println(" is calibating...");
        }

        switch (stage) {
            case 0:
            case 5: Serial.println("first delay...");
                    delay(ACTUATOR_STROKE_LENGTH / SPEED * 1000 + EXTRA_SECONDS * 1000); // wait for actuators to fully extend/retract
                    break;

            case 2:
            case 7: Serial.println("second delay...");
                    delay(SECOND_PROBE_LENGTH * ACTUATOR_STROKE_LENGTH / SPEED * 1000); // wait for actuators to extend/retract to prepare second check
                    break;

            case 3:
            case 8: Serial.println("third delay...");
                    delay(SECOND_PROBE_LENGTH * ACTUATOR_STROKE_LENGTH / (SPEED * CALIB_SPEED_RATIO) * 1000 + EXTRA_SECONDS * 1000); // wait for actuators to extend/retract to make second check
                    break;

            default: break;
        }
    }
    isCalibrated = true;
    isReady = true;
}

void Platform::calibrate(uint16_t (&settings)[NUM_ACTUATORS][2]) {
    Serial.println("custom platform calibration!");
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        actuators[i].calibrate(settings[i]);
    }
    isCalibrated = true;
    isReady = true;
}

void Platform::retract() {
    for (int i = 0; i < NUM_ACTUATORS; i++){
        actuators[i].retract();
    }
}

void Platform::setLoopDuration(int travelTime) {
    duration = ((unsigned long) travelTime)*1000;
    Serial.print("loop duration set: "); Serial.println(duration);
}

bool Platform::setActuatorMovements(int actuatorMovements[6][NUM_MAX_SWITCHES]) {
    Serial.println("setting actuator movements");
    for (int i = 0; i < NUM_ACTUATORS; i++){
      actuators[i].setMovements(actuatorMovements[i]);
    }
}

//clip the value to be in range of -1 to 1
float Platform::clip(float f) {
    return max(-1, min(1, f));
}

int Platform::getActuatorRawPosition(int actuator) {
    if (actuatorIsValid(actuator)) {
        return actuators[actuator].getRawPosition();
    }
    else {
        return 0;
    }
}

int Platform::getActuatorPosition(int actuator) {
    if (actuatorIsValid(actuator)) {
        return actuators[actuator].getPosition();
    }
    else {
        return 0;
    }
}

int Platform::getActuatorMaxPosition(int actuator) {
    if (actuatorIsValid(actuator)) {
        return actuators[actuator].getMaxPosition();
    }
    else {
        return 0;
    }
}

int Platform::getActuatorMinPosition(int actuator) {
    if (actuatorIsValid(actuator)) {
        return actuators[actuator].getMinPosition();
    }
    else {
        return 0;
    }
}

bool Platform::isActuatorReady(int actuator) {
    if (actuatorIsValid(actuator)) {
        return actuators[actuator].isActuatorReady();
    }
    else {
        return 0;
    }
}

bool Platform::isPlatformReady() {
    isReady = true;
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        isReady = isReady && actuators[i].isActuatorReady();
    }
    return isReady;
}

bool Platform::actuatorIsValid(int actuator) {
    return actuator >= 0 && actuator < NUM_ACTUATORS;
}

bool Platform::isPlatformSet() {
    isSet = true;
    for (int i = 0; i < NUM_ACTUATORS; i++) {
        isSet = isSet && actuators[i].isActuatorSet();
    }
    return isSet;
}

void Platform::noCalibrate(){
    actuators[0].noCalibrate(A1_MIN,A1_MAX);
    actuators[1].noCalibrate(A2_MIN,A2_MAX);
    actuators[2].noCalibrate(B1_MIN,B1_MAX);
    actuators[3].noCalibrate(B2_MIN,B2_MAX);
    actuators[4].noCalibrate(C1_MIN,C1_MAX);
    actuators[5].noCalibrate(C2_MIN,C2_MAX);
    isCalibrated = true;
    isReady = true;
}
