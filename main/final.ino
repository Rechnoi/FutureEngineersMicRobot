void straightSection() {
    isSignClear();
    ledStatesLight(LED_OFF);
    moveSideEncoder(20 * 190, MODE_NOT_CHANGE_ULTRASONIC);
    ledStatesLight(LED_GREEN);
    if (is_sign[side_to_dir(now_side)]) {
        ledStatesLight(LED_MAGENTA);
        evasion(!side_to_dir(now_side));
        ledStatesLight(LED_GREEN);
        //stopRobot();
        //while (true) {}
        //moveSideEncoder(3000);
    } else {
        ledStatesLight(LED_BLUE);
        moveSideEncoder(40 * 190, MODE_NOT_CHANGE_ULTRASONIC);
        if (is_sign[side_to_dir(now_side)]) {
            ledStatesLight(LED_MAGENTA);
            evasion(!side_to_dir(now_side));
            ledStatesLight(LED_GREEN);
        }
    }
    ledStatesLight(LED_WHITE);
    distance[FORWARD_ULTRASONIC] = 1000;
    moveSideToDistance(100, MODE_NOT_CHANGE_ULTRASONIC);
}

void rotate() {
    ledStatesLight(LED_OFF);
    isSignClear();
    moveSideToDistance(distance_to_rotate_inside, MODE_NOT_CHANGE_ULTRASONIC);
    //moveToDistance(distance_to_rotate_inside);
    
    
    bool new_side = is_sign[side_to_dir(OUTSIDE_SIDE)] ? INSIDE_SIDE : OUTSIDE_SIDE;
    if (new_side == OUTSIDE_SIDE) {
        ledStatesLight(LED_BLUE);
        //moveToDistance(distance_to_rotate_outside);
        servoTurnCenter();
        moveEncoder(190 * (distance_to_rotate_inside - distance_to_rotate_outside));
    }
    
    ledStatesLight(LED_GREEN);
    rotate90();
    ledStatesLight(LED_BLUE);
    old_side = now_side;
    now_side = new_side;
    targetDistanceUpdate();
    /*
    if (old_side == OUTSIDE_SIDE) {
        moveSideEncoder(10 * 190, MODE_NOT_CHANGE_ULTRASONIC);
    }
    moveEncoder(14 * 190);
    */
    
    if (old_side == OUTSIDE_SIDE) {
        moveSideOrEncoder(24 * 190);
    } else {
        moveSideOrEncoder(14 * 190);
    }
}

void moveSideOrEncoder(int tick_encoder) {
    if (now_side == OUTSIDE_SIDE) {
        moveSideEncoder(tick_encoder, MODE_NOT_CHANGE_ULTRASONIC);
    } else {
        moveEncoder(tick_encoder);
    }
}

/*
void start() {
    setMainTargetDistance();
    moveSideEncoder(10 * 190, MODE_NOT_CHANGE_ULTRASONIC);
    
    bool dir_rebuild = is_sign[DIR_RIGHT] ? DIR_LEFT : DIR_RIGHT;
    rebuildToLine(dir_rebuild);
    //moveSide
    if (sign != NOT_SIGN) {
        ledStatesLight(LED_RED);
        /rebuildToLine(!sign);
    } else {
        //--moveSideEncoder(30 << 7);
        int sign = getSign();
        if (sign != NOT_SIGN) {
            evasion(!sign);
        }
    }

    while ((distance[LEFT_ULTRASONIC]) && (distance[RIGHT_ULTRASONIC])) {
        moveSide(MODE_NOT_SIGN);
    }
    
    if (!distance[ultrasonic_move_forward]) {
        ultrasonic_move_forward = !ultrasonic_move_forward;
        setTargetDistance(begin_distance[ultrasonic_move_forward]);
        dir_rotate = !dir_rotate;
        begin_pid_move = true;
    }
    now_side = dir_to_side(ultrasonic_move_forward);
    //--moveSideToDistance(100);
}
*/
