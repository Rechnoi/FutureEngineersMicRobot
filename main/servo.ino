#include <Servo.h>

const int CENTER_ANGLE = 102;
const int MAX_SERVO_ANGLE = 40;

const int PORT_MOTOR = 9;

Servo servo;

void servoSetup() {
    servo.attach(PORT_MOTOR);
    servoTurnCenter();
}

void servoTurnCenter() {
    servo.write(CENTER_ANGLE);
}

void servoWrite(int angle) {
    if (angle < -MAX_SERVO_ANGLE) {
        angle = -MAX_SERVO_ANGLE;
    } else if (angle > MAX_SERVO_ANGLE) {
        angle = MAX_SERVO_ANGLE;
    }
    angle = CENTER_ANGLE - angle;
    servo.write(angle);
}

void servoTurn(bool dir) {
    if (dir == DIR_LEFT) {
        servoWrite(-MAX_SERVO_ANGLE);
    } else {
        servoWrite(MAX_SERVO_ANGLE);
    }
}

void servoTurn() {
    servoTurn(dir_rotate);
}

int servoRead() {
    return CENTER_ANGLE - servo.read();
}
