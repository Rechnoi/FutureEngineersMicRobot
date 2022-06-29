// Riding on the wall
void moveBorder() {
    if (pid_is_ready) {
        pidCalculate();
        servoWrite(round(pid_value));
        pid_is_ready = false;
    } else if (turnover_is_ready) {
        servoWrite(0);
    }
    turnover_is_ready = false;
}

void rotate() {
    servoTurn(dir_rotate);
    
    bool flag = true;
    while (flag) {
        lidarRead();
        if ((!border[BACK].empty()) && (border[BACK].line.distToCenter() < 1500)) {
            Vector v = border[BACK].getVector();
            double angle = abs(atan2(v.y, v.x));
            if (min(angle, PI - angle) < radians(35)) {
                flag = false;
            }
        }
    }
    servoTurnCenter();

    unsigned long long time_begin = millis();
    
    pid_is_ready = false;
    turnover_is_ready = false;
    begin_pid_move = true;
    ++cnt_rotates;
    

    while ((border[BACK].empty()) || (border[BACK].line.distToCenter() < 1100)) {
        lidarRead();
        moveBorder();
    }
}

const int dist_to_rotate = 760;

// Riding to the first turn
void moveStart() {
    if ((border[LEFT].empty()) || (border[RIGHT].empty())) {
        side_move_forward = !border[RIGHT].empty() ? RIGHT : LEFT;
    } else {
        side_move_forward = border[RIGHT].line.distToCenter() > border[LEFT].line.distToCenter() ? RIGHT : LEFT;
    }
    
    int distances[2];
    for (int side = 0; side < 2; ++side) {
        if (border[side].empty()) {
            distances[side] = default_middle_target_distance;
        } else {
            distances[side] = border[side].line.distToCenter();
        }
    }
    target_distance = distances[side_move_forward];
    
    while (dir_rotate == -1) {
        lidarRead();
        moveBorder();
    }
    if (dir_rotate == side_move_forward) {
        side_move_forward = !side_move_forward;
        target_distance = distances[side_move_forward];
    }
    updateMainTargetDistance();
    moveToRotate();
}

// Riding to the turn
void moveToRotate() {
    while ((border[FORWARD].empty()) || (border[FORWARD].line.distToCenter() > dist_to_rotate)) {
        lidarRead();
        moveBorder();
    }
}

void moveFinish() {}

// Solving the task
void solve() {
    motorWrite(170);
    moveStart();
    target_distance = middle_target_distance;
    rotate();
    for (int i = 0; i < 11; ++i) {
        moveToRotate();
        rotate();
    }
    moveFinish();
    
    
    motorStop();
    lidarStop();
}

void updateMainTargetDistance() {
    if (side_move_forward == LEFT) {
        middle_target_distance = left_middle_target_distance;
    } else {
        middle_target_distance = right_middle_target_distance;
    }
}
