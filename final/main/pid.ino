#define pid_kp 0.1
#define pid_kd 0.01

double old_pid_error;

//Вычисление значения регулятора
void pidCalculate() {
    Line line45degrees = side_move_forward == RIGHT ? Line(Point(0, 0), Point(1, 1)) : Line(Point(0, 0), Point(-1, 1));
    double dist_to_border = (border[side_move_forward].line & line45degrees).getLen();

    if (side_move_forward == RIGHT) {
        pid_error = dist_to_border - sqrt(2) * target_distance;
    } else {
        pid_error = sqrt(2) * target_distance - dist_to_border;
    }
    pid_value = pid_error * pid_kp;
    
    if (begin_pid_move) {
        begin_pid_move = false;
    } else {
        pid_value += (pid_error - old_pid_error) * pid_kd;
    }
    old_pid_error = pid_error;
}
