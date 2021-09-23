// Езда по стенке
void moveBorder() {
    if (pid_is_ready) {
        pidCalculate();
        servoWrite(round(pid_value));
        pid_is_ready = false;
    }
}

// Поворот
void rotate() {
    servoTurn(dir_rotate);
    bool flag = true;
    while (flag) {
        lidarRead();
        if ((!border[BACK].empty()) && (border[BACK].line.distToCenter() < 1500)) {
            Vector v = border[BACK].getVector();
            double angle = abs(atan2(v.y, v.x));
            if (min(angle, PI - angle) < radians(30)) {
                flag = false;
            }
        }
    }
    servoTurnCenter();

    memset(is_sign, 0, 6);
    
    pid_is_ready = false;
    begin_pid_move = true;
}

const int dist_to_rotate = 550;

int target_distances[2];

// Езда до первого поворота
void moveStart() {
    int target_distances[2];
    for (int side = 0; side < 2; ++side) {
        if (!border[side].empty()) {
            target_distances[side] = border[side].line.distToCenter();
        }
    }
    if ((border[LEFT].empty()) || (border[RIGHT].empty())) {
        side_move_forward = !border[RIGHT].empty() ? RIGHT : LEFT;
    } else {
        side_move_forward = border[RIGHT].line.distToCenter() > border[LEFT].line.distToCenter() ? RIGHT : LEFT;
    }
    
    target_distance = target_distances[side_move_forward];
    
    while (dir_rotate == -1) {
        lidarRead();
        moveBorder();
        for (int dir = 0; dir < 2; ++dir) {
            if (border[dir].line.distToCenter() > 1500) {
                dir_rotate = dir;
            }
        }
    }
    if (dir_rotate == side_move_forward) {
        side_move_forward = !side_move_forward;
        target_distance = target_distances[side_move_forward];
    }
    moveToRotate();
    target_distance = main_target_distance;
}

// Езда до поворота
void moveToRotate() {
    while ((border[FORWARD].empty()) || (border[FORWARD].line.distToCenter() > dist_to_rotate)) {
        lidarRead();
        moveBorder();
    }
}

// Финиш
void moveFinish() {
    while ((border[FORWARD].empty()) || (border[FORWARD].line.distToCenter() > 1500)) {
        lidarRead();
        moveBorder();
    }
}

//Решение задачи
void solve() {
    motorWrite(128);
    moveStart();
    rotate();
    motorWrite(190);
    for (int i = 0; i < 11; ++i) {
        moveToRotate();
        rotate();
    }
    moveFinish();
    
    
    motorStop();
    lidarStop();
    while (true) {}
}
