const byte PORT_MOTOR_PWM = 3;
const byte PORT_MOTOR_DIR = 12;

//Стартовая настойка моторов
void motorLeaderSetup() {
    pinMode(PORT_MOTOR_PWM, OUTPUT);
    pinMode(PORT_MOTOR_DIR, OUTPUT);
}

//Функцию, которая сейчас не работает)
void motorLeaderWrite(int speed_motor, bool dir) {
    digitalWrite(PORT_MOTOR_DIR, dir);
    analogWrite(PORT_MOTOR_PWM, speed_motor);
}

//Остановка мотора
void motorLeaderStop() {
    digitalWrite(PORT_MOTOR_PWM, false);
}

//Включение мотора на полную мощность
void motorLeaderWriteHigh(bool dir) {
    digitalWrite(PORT_MOTOR_DIR, dir);
    digitalWrite(PORT_MOTOR_PWM, HIGH);
}
