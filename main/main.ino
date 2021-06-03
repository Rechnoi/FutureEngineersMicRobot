#define DIR_LEFT false
#define DIR_RIGHT true

#define LEFT_ULTRASONIC DIR_LEFT
#define RIGHT_ULTRASONIC DIR_RIGHT
#define FORWARD_ULTRASONIC 2

#define DIR_BACK false
#define DIR_FORWARD true

#define STATE_WAIT_STABILIZATION -1
#define STATE_ROTATE 0
#define STATE_STOP_ROBOT 1
#define STATE_MOVE_FORWARD 2
#define STATE_FINISH 3
#define STATE_GET_DIR_MOVE 4

//#define DIR_CLOCKWISE false
//#define DIR_COUNTERCLOCKWISE true

#define DEBUG_WRITE 1

int pid_value = 0;

const int ULTRASONIC_NUM = 3;
volatile int dist[ULTRASONIC_NUM];

int current_state;

int ultrasonic_move_forward;
bool dir_rotate;

void setup() {
    servoSetup();
    
    motorLeaderSetup();
    ultrasonicSetup();
    cameraSetup();
    moveSetup();
#if DEBUG_WRITE
    debugWriteSetup();
#endif
    delay(1500);
    
    ultrasonic_move_forward = RIGHT_ULTRASONIC;
    waitStabilizationBegin();
}

unsigned long previousMillis = 0;

void loop() {
    globalMain();
    //cameraDebugBlobs();
    //while (millis() - previousMillis < 33) {}
    //previousMillis = millis();
}

//Итерация основного цикла программы
void globalMain() {
    ultrasonicCheckTimer();
    if (current_state == STATE_WAIT_STABILIZATION) {
        waitStabilization();
    } else if (current_state == STATE_GET_DIR_MOVE) {
        getDirMove();
    } else if (current_state == STATE_MOVE_FORWARD) {
        moveForward();
    } else if (current_state == STATE_ROTATE) {
        rotate();
    } else if (current_state == STATE_FINISH) {
        finish();
    }
}

//Итерация дебага
void globalDebug() {
    ultrasonicCheckTimer();
}





/*
//Задача напрвления движения (по часовой стрелке или против часовой)
void setDirMove(bool dir) {
    if (dir == DIR_CLOCKWISE) {
        ultrasonic_move_forward = LEFT_ULTRASONIC;
        dir_rotate = DIR_RIGHT;
    } else {
        ultrasonic_move_forward = RIGHT_ULTRASONIC;
        dir_rotate = DIR_LEFT;
    }
}
*/

/*
void globalMoveForward() {
    ultrasonicCheckTimer();
    if (current_state == STATE_WAIT_STABILIZATION) {
        waitStabilization();
    } else if (current_state == STATE_MOVE_FORWARD) {
        if (dist[RIGHT_ULTRASONIC] == 0) {
            stopRobot();
        } else {
            moveForward();
        }
    }
}

void globalTest() {
    ultrasonicCheckTimer();
    if (current_state == STATE_WAIT_STABILIZATION) {
        waitStabilization();
    } else if (current_state == STATE_MOVE_FORWARD) {
        if ((dist[FORWARD_ULTRASONIC] <= 40) && (dist[FORWARD_ULTRASONIC])){
            stopRobot();
        } else {
            moveForward();
        }
    }
}
*/
