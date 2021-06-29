const int main_target_distance = 43; //33

#define pid_kp 4
#define pid_kd 2

int pid_error = 0;
int old_pid_error = 0;
int old_pid_value = -1;

//Установка целевой дистанции на основную целевую дистанцию
void setMainTargetDistance() {
    target_distance = main_target_distance;
}

//Установка целевой дистанции
void setTargetDistance(int dist) {
    target_distance = dist;
}

void changeTargetDistance() {
    target_distance ^= short_distance_to_side ^ long_distance_to_side;
}

void targetDistanceUpdate() {
    if (now_side == INSIDE_SIDE) {
        target_distance = long_distance_to_side;
    } else {
        target_distance = short_distance_to_side;
    }
    /*
    if ((now_side == INSIDE_SIDE) ^ (ultrasonic_move_forward == dir_rotate)) {
        target_distance = long_distance_to_side;
    } else {
        target_distance = short_distance_to_side;
    }
    */
}

//Вычисление значения регулятора
void pidCalculate() {
    int dist = distance[ultrasonic_move_forward];
    if (ultrasonic_move_forward == RIGHT_ULTRASONIC) {
        pid_error = dist - target_distance;
    } else {
        pid_error = target_distance - dist;
    }
    pid_value = pid_error * pid_kp;
    if (begin_pid_move) {
        begin_pid_move = false;
    } else {
        pid_value += (pid_error - old_pid_error) * pid_kd;
    }
    old_pid_error = pid_error;
}
