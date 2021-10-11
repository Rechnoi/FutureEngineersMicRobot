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
    if (cnt_rotates < 4) {
        for (int side = 0; side < 2; ++side) {
            for (int i = 0; i < 3; ++i) {
                exist_signs[side][cnt_rotates] |= is_sign[side][i];
            }
        }
    }
    
    servoTurn(dir_rotate);
    //motorWrite(85);
    
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
    //motorWrite(170);

    unsigned long long time_begin = millis();

    memset(is_sign, 0, 6);
    memset(is_rotate_sign, 0, 4);
    
    pid_is_ready = false;
    turnover_is_ready = false;
    begin_pid_move = true;
    ++cnt_rotates;
    
    target_distance = middle_target_distance;
    

    while ((border[BACK].empty()) || (border[BACK].line.distToCenter() < 1100)) {
        lidarRead();
        moveBorder();
    }
}

const int dist_to_rotate = 860; //760;

// Riding to the first turn
void moveStart() {
    if ((border[LEFT].empty()) || (border[RIGHT].empty())) {
        side_move_forward = !border[RIGHT].empty() ? RIGHT : LEFT;
    } else {
        side_move_forward = border[RIGHT].line.distToCenter() > border[LEFT].line.distToCenter() ? RIGHT : LEFT;
    }
    updateMainTargetDistance();
    
    while (dir_rotate == -1) {
        lidarRead();
        moveBorder();
    }
    if (dir_rotate == side_move_forward) {
        side_move_forward = !side_move_forward;
        updateMainTargetDistance();
    }
    moveToRotate();
    target_distance = middle_target_distance;
}

// Riding to the turn
void moveToRotate() {
    bool flag = true;
    while (flag) {
        lidarRead();
        moveBorder();
        if (!border[FORWARD].empty()) {
            if (is_rotate_sign[dir_rotate][NEAR] == is_rotate_sign[dir_rotate][REMOTE]) {
                if (border[FORWARD].line.distToCenter() <= dist_to_rotate) {
                    flag = false;
                }
            } else if (is_rotate_sign[dir_rotate][NEAR] > is_rotate_sign[dir_rotate][REMOTE]) {
                if (border[FORWARD].line.distToCenter() <= dist_to_rotate - DIST_EVASION_TURN_FORWARD) {
                    flag = false;
                }
            } else {
                if (border[FORWARD].line.distToCenter() <= dist_to_rotate + DIST_EVASION_TURN_BACK) {
                    flag = false;
                }
            }
        }
    }
}

void moveFinish() {}

// Solving the task
void solve() {
    motorWrite(170);
    moveStart();
    rotate();
    for (int i = 0; i < 11; ++i) {
        moveToRotate();
        rotate();
    }
    moveFinish();
    
    
    motorStop();
    lidarStop();
}

// Updating the target distance to the wall
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

void updateMainTargetDistance() {
    if (side_move_forward == LEFT) {
        middle_target_distance = left_middle_target_distance;
    } else {
        middle_target_distance = right_middle_target_distance;
    }
}
