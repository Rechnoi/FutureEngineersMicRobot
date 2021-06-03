//ВСЯКИЕ КОНСТАНТЫ

const int distance_to_turn = 40;

int time_rotate[2];

void moveSetup() {
    time_rotate[DIR_LEFT] = 800;
    time_rotate[DIR_RIGHT] = 800;
}

//ОЖИДАНИЕ СТАБИЛИЗАЦИИ ЗНАЧЕНИЙ С УЛЬТРАСОНИКОВ

long time_start;

void waitStabilizationBegin() {
    current_state = STATE_WAIT_STABILIZATION;
    time_start = millis();
}

void waitStabilization() {
    if (millis() - time_start >= 1000) {
        motorLeaderWriteHigh(DIR_FORWARD);
        getDirMoveBegin();
    }
}

//ПОЛУЧЕНИЕ НАПРАВЛЕНИЯ ДВИЖЕНИЯ

bool is_known_dir = false;

int begin_dist[2];

void getDirMoveBegin() {
    current_state = STATE_GET_DIR_MOVE;
    for (int i = 0; i < 2; ++i) {
        begin_dist[i] = dist[i];
    }
    setTargetDistance(dist[ultrasonic_move_forward]);
}

void getDirMove() {
    if (!is_known_dir) {
        for (int i = 0; i < 2; ++i) {
            if (!dist[i]) {
                if (ultrasonic_move_forward == i) {
                    ultrasonic_move_forward = !ultrasonic_move_forward;
                    setTargetDistance(begin_dist[ultrasonic_move_forward]);
                }
                dir_rotate = i;
                is_known_dir = true;
            }
        }
        moveSide();
    } else if ((dist[FORWARD_ULTRASONIC] <= distance_to_turn) && (!dist[!ultrasonic_move_forward])) {
        setMainTargetDistance();
        rotateBegin();
    } else {
        moveSide();
    }
}

//ЕЗДА ВПЕРЁД

void moveForwardBegin() {
    current_state = STATE_MOVE_FORWARD;
}

void moveForward() {
    if ((dist[FORWARD_ULTRASONIC] <= distance_to_turn) && (!dist[!ultrasonic_move_forward])) {
        rotateBegin();
    } else {
        moveSide();
    }
}

//ПОВОРОТ

unsigned long time_begin_rotate;

int count_rotate = 0;

void rotateBegin() {
    current_state = STATE_ROTATE;
    ++count_rotate;
    servoTurn();
    time_begin_rotate = millis();
}

void rotate() {
    if (millis() - time_begin_rotate >= time_rotate[dir_rotate]) {
        servoWrite(0);
        if (count_rotate == 12) {
            finishBegin();
        } else {
            moveForwardBegin();
        }
    }
}

//ФИНИШ

long time_begin_finish;

void finishBegin() {
    current_state = STATE_FINISH;
    time_begin_finish = millis();
}

void finish() {
    if ((dist[FORWARD_ULTRASONIC] <= 150) && (dist[!ultrasonic_move_forward])
        && (millis() - time_begin_finish >= 2000)) {
        stopRobot();
    } else {
        moveSide();
    }
}

//Остановка робота
void stopRobot() {
    current_state = STATE_STOP_ROBOT;
    servoWrite(0);
    motorLeaderStop();
}
