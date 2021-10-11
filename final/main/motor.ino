#include "analogWrite.h"

const byte port_motor_dir = 19;
const byte port_motor_pwm = 18;

// Initialization of the motor
void motorSetup() {
    pinMode(port_motor_dir, OUTPUT);
}

// Setting the motor speed
void motorWrite(int speed_motor) {
    digitalWrite(port_motor_dir, HIGH);
    analogWrite(port_motor_pwm, 255 - speed_motor, 10000);
}

// Stopping the motor
void motorStop() {
    motorWrite(0);
}
