//1 см = 190 тиков энкодера

#define DIR_LEFT false
#define DIR_RIGHT true

#define LEFT_ULTRASONIC DIR_LEFT
#define RIGHT_ULTRASONIC DIR_RIGHT
#define FORWARD_ULTRASONIC 2

#define DIR_BACK false
#define DIR_FORWARD true

//#define DIR_CLOCKWISE false
//#define DIR_COUNTERCLOCKWISE true

#define MODE_NOT_SIGN
#define MODE_CHANGE_ULTRASONIC
#define MODE_NOT_CHANGE_ULTRASONIC

bool is_ultrasonic_ready = false;

bool begin_pid_move = true;

#define LED_RED 0       //good color
#define LED_GREEN 1     //good color
#define LED_BLUE 2      //good color
#define LED_CYAN 3
#define LED_YELLOW 4
#define LED_MAGENTA 5
#define LED_WHITE 6
#define LED_OFF 7

#define INSIDE_SIDE true
#define OUTSIDE_SIDE false

#define NOT_SIGN -1

const int short_distance_to_side = 35;
const int long_distance_to_side = 52;

const int distance_to_rotate_outside = 50;
const int distance_to_rotate_inside = 70;

bool now_side, old_side;

#define DEBUG_WRITE 1

#define DEBUG_LED_CHANGE 0
#define DEBUG_LED_CAMERA 1
#define DEBUG_LED_STATES 0

#if DEBUG_LED_CHANGE + DEBUG_LED_CAMERA + DEBUG_STATES > 1
    #error "Conflict debug led"
#endif

#define MODE_NOT_SIGN 0
#define MODE_CHANGE_ULTRASONIC 1
#define MODE_NOT_CHANGE_ULTRASONIC 2

int pid_value = 0;

const int ULTRASONIC_NUM = 3;
volatile int distance[ULTRASONIC_NUM];

int ultrasonic_move_forward;
bool dir_rotate;

volatile long encoder_value = 0;

#define GREEN_SIGN DIR_RIGHT
#define RED_SIGN DIR_LEFT

int target_distance;

bool is_begin = true;

bool is_sign[2] = {false, false};

void setup() {
    servoSetup();
    
    motorSetup();
    ultrasonicSetup();
    cameraSetup();
    encoderSetup();
    ledSetup();
    btnSetup();
#if DEBUG_WRITE
    debugWriteSetup();
#endif
}

void sensorsCheckTimer() {
    ultrasonicCheckTimer();
    cameraCheckTimer();
}

const int time_wait_stabilization = 10000;

unsigned long time_begin_wait;

void wait(int time_wait) {
    time_begin_wait = millis();
    while (millis() - time_begin_wait < time_wait) {
        sensorsCheckTimer();
    }
}

void stopRobot() {
    motorStop();
    servoTurnCenter();
}

void loop() {
    is_begin = true;
    ledLight(LED_RED);
    wait(time_wait_stabilization);
    ledLight(LED_GREEN);
    while (!readStateBtn()) {
        sensorsCheckTimer();
    }
    is_begin = false;
    solve();
    
    
    //cameraCheckTimer();
    //encoderTest();
}

bool dir_to_side(bool dir) {
    if (dir_rotate == dir) {
        return INSIDE_SIDE;
    }
    return OUTSIDE_SIDE;
}

bool side_to_dir(bool side) {
    if (side == INSIDE_SIDE) {
        return dir_rotate;
    } else {
        return !dir_rotate;
    }
}

void test_encoder_cm() {
    int begin_dist = distance[FORWARD_ULTRASONIC];
    motorWrite(100);
    moveSideToDistance(50, NOT_SIGN);
    stopRobot();
    wait(2000);
    int end_dist = distance[FORWARD_ULTRASONIC];
    Serial.println(begin_dist - end_dist);
    Serial.println(encoder_value);
    while (true) {}
}

void solve() {
    isSignClear();
    
    dir_rotate = DIR_RIGHT;
    ultrasonic_move_forward = !dir_rotate;

    targetDistanceUpdate();

    motorWrite(100);
    setMainTargetDistance();
    
    rebuildToLine(DIR_RIGHT);
    //moveSideToDistance(50, MODE_NOT_CHANGE_ULTRASONIC);
    stopRobot();
    while (true) {}
    
    //qual();
    //return;

    now_side = OUTSIDE_SIDE;
    targetDistanceUpdate();
    motorWrite(100);
    while (true) {
        straightSection();
        rotate();
    }
    
    stopRobot();
    while (true) {}

    /*
    motorWrite(100);
    moveSideEncoder(4 * 190, MODE_NOT_CHANGE_ULTRASONIC);
    evasion(DIR_RIGHT);
    moveSideEncoder(4 * 190, MODE_NOT_CHANGE_ULTRASONIC);
    
    stopRobot();
    while (true) {}
    */
    motorWrite(100);
    moveSideToDistance(100, MODE_NOT_CHANGE_ULTRASONIC);
    rotate();
    //moveSideToDistance(50, MODE_NOT_CHANGE_ULTRASONIC);
    //ledLight(LED_MAGENTA);
    //moveSideToDistance(100, MODE_NOT_CHANGE_ULTRASONIC);
    stopRobot();
    while (true) {}
    return;
    

    //moveSideToDistance(50, MODE_NOT_CHANGE_ULTRASONIC);

    //stopRobot();
    //while (true) {}

    //start();
    while (true) {
        straightSection();
        rotate();
    }
    
    stopRobot();
    while (true) {}
}

int begin_distance[2];





unsigned long timer_debug = 0;

void encoderTest() {
    if (millis() - timer_debug >= 100) {
        timer_debug = millis();
        Serial.println(encoder_value);
    }
}
