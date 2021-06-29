//Движение вперёд по стенке
void moveSide(int mode_sign) {
    sensorsCheckTimer();
    if ((distance[ultrasonic_move_forward] < 20) && (mode_sign != MODE_NOT_SIGN)) {
        begin_pid_move = true;
        if (mode_sign == MODE_CHANGE_ULTRASONIC) {
            changeTargetDistance();
            ultrasonic_move_forward = !ultrasonic_move_forward;
            
            if (is_ultrasonic_ready) {
                pidCalculate();
                servoWrite(pid_value);
                is_ultrasonic_ready = false;
            }

            changeTargetDistance();
            ultrasonic_move_forward = !ultrasonic_move_forward;
        } else {
            servoTurnCenter();
        }
    } else {
        if (is_ultrasonic_ready) {
            pidCalculate();
            servoWrite(pid_value);
            is_ultrasonic_ready = false;
        }
    }
}

/*
void moveSideAndEncoder() {
    sensorsCheckTimer();
    if (distance[ultrasonic_move_forward] <= 20) {
        servoTurnCenter();
    } else {
        pidCalculate();
        servoWrite(pid_value);
    }
}
*/

//Движение по стенке, пока передний ультросоник не увидет расстояние, <= заданного
void moveSideToDistance(int dist, int mode_sign) {
    while (distance[FORWARD_ULTRASONIC] > dist) {
        moveSide(mode_sign);
    }
}

void moveEncoder(int tick_encoder) {
    encoder_value = 0;
    while (encoder_value < tick_encoder) {
        sensorsCheckTimer();
    }
}

const int rotate_tick_encoder = 30 * 190;

void rotate90() {
    servoTurn();
    encoder_value = 0;
    while (encoder_value < rotate_tick_encoder) {
        sensorsCheckTimer();
    }
    servoTurnCenter();
    begin_pid_move = true;
}

void moveSideEncoder(int tick_encoder, int mode_sign) {
    encoder_value = 0;
    while (encoder_value < tick_encoder) {
        moveSide(mode_sign);
    }
}

void moveToDistance(int dist) {
    servoTurnCenter();
    while (distance[FORWARD_ULTRASONIC] > dist) {
        sensorsCheckTimer();
    }
}

void evasion(bool dir) { //Примерно 40 см
    servoTurn(dir);
    moveEncoder(19 * 190);
    servoTurn(!dir);
    moveEncoder(23 * 190);
    servoTurnCenter();
    
    now_side = !now_side;
    targetDistanceUpdate();
    begin_pid_move = true;
}

void rebuildToLine(bool dir) {
    servoTurn(dir);
    moveEncoder(13 * 190);
    servoTurn(!dir);
    moveEncoder(16 * 190);
    servoTurnCenter();
    
    now_side = !now_side;
    targetDistanceUpdate();
    begin_pid_move = true;
}
