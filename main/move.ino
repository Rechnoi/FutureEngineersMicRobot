//Движение вперёд по стенке
void moveSide() {
    pidCalculate();
    servoWrite(pid_value);
}
