#include <ESP32Servo.h>

const int port_servo = 26;

const int CENTER_ANGLE = 98;
const int MAX_SERVO_ANGLE = 50;
const int TURN_ANGLE = 30;

Servo servo;

// Initializing the servo
void servoSetup() {
    ESP32PWM::allocateTimer(2);
    servo.attach(port_servo);
    servoTurnCenter();
}

// Turning the servo to the center
void servoTurnCenter() {
    servo.write(CENTER_ANGLE);
}

// Rotation of the servo by a given angle
void servoWrite(int angle) {
    if (angle < -MAX_SERVO_ANGLE) {
        angle = -MAX_SERVO_ANGLE;
    } else if (angle > MAX_SERVO_ANGLE) {
        angle = MAX_SERVO_ANGLE;
    }
    angle = CENTER_ANGLE - angle;
    servo.write(angle);
}

// Turning the servo on the turn
void servoTurn(bool dir) {
    if (dir == LEFT) {
        servoWrite(-TURN_ANGLE);
    } else {
        servoWrite(TURN_ANGLE);
    }
}

// Servo angle reading
int servoRead() {
    return CENTER_ANGLE - servo.read();
}
