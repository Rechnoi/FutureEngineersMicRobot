const byte port_motor_pwm = 5;
const byte port_motor_dir = 6;

//Стартовая настойка моторов
void motorSetup() {
    pinMode(port_motor_pwm, OUTPUT);
    pinMode(port_motor_dir, OUTPUT);

    motorStop();
}

void motorWrite(int speed_motor) {
    digitalWrite(port_motor_dir, LOW);
    analogWrite(port_motor_pwm, speed_motor);
}

//Остановка мотора
void motorStop() {
    digitalWrite(port_motor_pwm, false);
    digitalWrite(port_motor_dir, false);
}

//Включение мотора на полную мощность
void motorWriteHigh() {
    digitalWrite(port_motor_dir, LOW);
    digitalWrite(port_motor_pwm, HIGH);
}
