#include <ESP32Servo.h>

const int port_servo = 26;

const int CENTER_ANGLE = 98;
const int MAX_SERVO_ANGLE = 50;

Servo servo;

// Инициализация сервопривода
void servoSetup() {
    ESP32PWM::allocateTimer(2);
    servo.attach(port_servo);
    servoTurnCenter();
}

// Поворот сервопривода в центр
void servoTurnCenter() {
    servo.write(CENTER_ANGLE);
}

// Поворот сервопривода на заданный угол
void servoWrite(int angle) {
    if (angle < -MAX_SERVO_ANGLE) {
        angle = -MAX_SERVO_ANGLE;
    } else if (angle > MAX_SERVO_ANGLE) {
        angle = MAX_SERVO_ANGLE;
    }
    angle = CENTER_ANGLE - angle;
    servo.write(angle);
}

// Поворот сервопривода на максимальный угол направо или налево
void servoTurn(bool dir) {
    if (dir == LEFT) {
        servoWrite(-MAX_SERVO_ANGLE);
    } else {
        servoWrite(MAX_SERVO_ANGLE);
    }
}

// Считывание угла сервопривода
int servoRead() {
    return CENTER_ANGLE - servo.read();
}
