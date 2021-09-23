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
    target_distance = middle_target_distance;
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
    while ((border[BACK].empty()) || (border[BACK].line.distToCenter() < 1100)) {
        lidarRead();
        moveBorder();
    }
}

const int dist_to_rotate = 690;

// Езда до первого поворота
void moveStart() {
    while (dir_rotate == -1) {
        lidarRead();
        moveBorder();
        for (int dir = 0; dir < 2; ++dir) {
            if ((!border[dir].empty()) && (border[dir].line.distToCenter() > 1500)) {
                dir_rotate = dir;
            }
        }
    }
    if (dir_rotate == side_move_forward) {
        side_move_forward = !side_move_forward;
        begin_pid_move = true;
    }
    moveToRotate();

    if (dir_rotate == LEFT) {
        middle_target_distance = left_middle_target_distance;
    } else {
        middle_target_distance = right_middle_target_distance;
    }
}

// Езда до поворота
void moveToRotate() {
    while ((border[dir_rotate].empty()) || (border[dir_rotate].line.distToCenter() < 2000)) {
        lidarRead();
        moveBorder();
    }
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
    for (int i = 0; i < 11; ++i) {
        moveToRotate();
        rotate();
    }
    moveFinish();
    
    
    motorStop();
    lidarStop();
    while (true) {}
}

//Обновление целевого расстояния
void updateTargetDistance() {
    Point robot = conv_rect_coord(Point(0, 0));
    int i = -1;
    if (robot.y < 1000) {
        i = 0;
    } else if (robot.y < 1500) {
        i = 1;
    } else if (robot.y < 2000) {
        i = 2;
    }
    if (i != -1) {
        int dir = -1;
        if ((!is_sign[LEFT][i]) && (!is_sign[RIGHT][i])) {   
            if (i != 2) {
                if (is_sign[LEFT][i + 1]) {
                    dir = LEFT;
                } else if (is_sign[RIGHT][i + 1]) {
                    dir = RIGHT;
                }
            }
        } else {
            dir = is_sign[LEFT][i] ? LEFT : RIGHT;
        }
        if (dir != -1) {
            if (dir == side_move_forward) {
                target_distance = middle_target_distance + DIST_EVASION;
            } else {
                target_distance = middle_target_distance - DIST_EVASION;
            }
        }
    }
}
