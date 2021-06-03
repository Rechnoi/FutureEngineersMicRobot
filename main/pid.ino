const int main_target_distance = 20;

#define pid_kp 1.5
#define pid_kd 0

int pid_error = 0;
int old_pid_error = 0;
int old_pid_value = -1;

int target_distance;

//Установка целевой дистанции на основную целевую дистанцию
void setMainTargetDistance() {
    target_distance = main_target_distance;
}

//Установка целевой дистанции
void setTargetDistance(int dist) {
    target_distance = dist;
}

//Вычисление значения регулятора
void pidCalculate() {
    int distance = dist[ultrasonic_move_forward];
    if (ultrasonic_move_forward == RIGHT_ULTRASONIC) {
        pid_error = distance - target_distance;
    } else {
        pid_error = target_distance - distance;
    }
    pid_value = pid_error * pid_kp + (pid_error - old_pid_error) * pid_kd;
    
    if (pid_value != old_pid_value) {
#if DEBUG_WRITE
//        Serial.println(pid_value);
#endif
        old_pid_value = pid_value;
    }
}
