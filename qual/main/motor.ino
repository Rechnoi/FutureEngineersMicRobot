#include "analogWrite.h"

const byte port_motor_dir = 19;
const byte port_motor_pwm = 18;

// Инициализация мотора
void motorSetup() {
    pinMode(port_motor_dir, OUTPUT);
}

// Устанока скорости мотора
void motorWrite(int speed_motor) {
    digitalWrite(port_motor_dir, HIGH);
    analogWrite(port_motor_pwm, 255 - speed_motor, 10000);
}

//Остановка мотора
void motorStop() {
    motorWrite(0);
}
