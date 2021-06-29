void qualStart() {
    for (int i = 0; i < 2; ++i) {
        begin_distance[i] = distance[i];
    }
    setTargetDistance(begin_distance[ultrasonic_move_forward]);
    while ((distance[LEFT_ULTRASONIC]) && (distance[RIGHT_ULTRASONIC])) {
        moveSide(MODE_NOT_SIGN);
    }
    
    if (!distance[ultrasonic_move_forward]) {
        ultrasonic_move_forward = !ultrasonic_move_forward;
        setTargetDistance(begin_distance[ultrasonic_move_forward]);
        dir_rotate = !dir_rotate;
        begin_pid_move = true;
    }
    moveSideToDistance(distance_to_rotate_outside, MODE_NOT_SIGN);
    rotate90();
    setTargetDistance(short_distance_to_side);
}

void qual() {
    motorWrite(100);
    qualStart();
    for (int i = 0; i < 11; ++i) {
        moveSideToDistance(distance_to_rotate_outside, MODE_NOT_SIGN);
        rotate90();
    }
    moveSideEncoder(90 * 190, MODE_NOT_SIGN);
    
    stopRobot();
    while (true) {}
}
