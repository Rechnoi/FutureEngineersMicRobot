#define USE_BLUETOOTH 1

#define DEBUG 1
#define BUTTON 1

#define LEFT 0
#define RIGHT 1
#define BACK 2
#define FORWARD 3

#define FORWARD_LEFT 0
#define FORWARD_RIGHT 1
#define BACK_LEFT 2
#define BACK_RIGHT 3

#define NEAR 0
#define REMOTE 1

#define Vector Point

bool side_move_forward;
int16_t dir_rotate;

const int default_middle_target_distance = 530;
const int left_middle_target_distance = 530;
const int right_middle_target_distance = 530;

const int DIST_EVASION = 50;
const int DIST_EVASION_TURN_FORWARD = 100;
const int DIST_EVASION_TURN_BACK = 150;

int middle_target_distance;
int target_distance;

int cnt_signs;
int cnt_good_signs;

int cnt_rotates;

bool exist_signs[2][4];

struct Point {
    int16_t x;
    int16_t y;

    Point();
    Point(int16_t x, int16_t y);

    int getSqrLen() const;
    double getLen() const;
    Vector getPerp() const;

    Point operator - () const;
};

// The line is given by the equation A * x + B * y + C = 0
struct Line {
    int A;
    int B;
    int C;

    Line();
    Line(const Point& A, const Point& B);
    
    double getX(int16_t y) const;
    double getY(int16_t x) const;
    
    double distToCenter() const;
    int sqrDistToPoint(const Point& P) const;
    int distToPoint(const Point& P) const;
};

struct Segment {
    Point A;
    Point B;
    Line line;

    Segment();
    Segment(const Point& A, const Point& B);

    void clear();
    bool empty() const;

    Vector getVector() const;
};

Point signs[100];

bool is_sign[2][3];
bool is_rotate_sign[2][2];

double pid_error;
double pid_value;

const int MAX_CNT_POINTS = 2000;

Point points[MAX_CNT_POINTS];
int cnt_points;

int cnt_segments;

int16_t buff[4000];

Segment border[4];

bool pid_is_ready;
bool turnover_is_ready;

bool begin_pid_move;

void loop() {
    initVariables();
#if DEBUG
    debugWriteSetup();
#endif
    servoSetup();
    motorSetup();
    lidarSetup();

#if BUTTON
    btnSetup();
    while (!btnReadState()) {
        lidarRead();
    }
#endif
    unsigned long long time_begin = millis();
    while (millis() - time_begin < 500) {
        lidarRead();
    }
    dir_rotate = -1;
    memset(is_sign, 0, 6);
    memset(is_rotate_sign, 0, 4);
    time_begin = millis();
    while (millis() - time_begin < 500) {
        lidarRead();
    }
    
    solve();
#ifndef DEBUG
    while (true) {}
#endif
    delay(2000);
}

void setup() {}

int time_turnover;

unsigned long long time_begin;
int diff_turnover;
unsigned long long time_old_turnover;

// Processing of information received during lidar turnover
void processingTurnover() {
    diff_turnover = millis() - time_old_turnover;
    time_old_turnover = millis();
    time_begin = millis();
    splitComponent();
    
    if (dir_rotate == -1) {
        for (int dir = 0; dir < 2; ++dir) {
            if (!border[dir].empty()) {
                if (border[dir].line.distToCenter() > 1500) {
                    dir_rotate = dir;
                }
                Segment seg = border[dir];
                Vector v = seg.getVector();
                if (v.y < 0) {
                    v = -v;
                }
                int cnt[2] = {0, 0};
                for (int i = 0; i < cnt_points; ++i) {
                    int dist = seg.line.distToPoint(points[i]);
                    if (dist > 100) {
                        if ((seg.A ^ points[i]) * v > 0) {
                            ++cnt[RIGHT];
                        } else {
                            ++cnt[LEFT];
                        }
                    }
                }
                if (cnt[dir] >= 3) {
                    dir_rotate = dir;
                }
            }
        }
    }
    pid_is_ready = !border[side_move_forward].empty();
    turnover_is_ready = true;
    time_turnover = millis() - time_begin;
    if ((pid_is_ready) && ((!border[FORWARD].empty()) || (!border[BACK].empty()))) {
        for (int i = 0; i < cnt_signs; ++i) {
            processingSign(signs[i]);
        }
        updateTargetDistance();
    }
}

// Initializing variables
void initVariables() {
    side_move_forward = RIGHT;
    dir_rotate = -1;
    middle_target_distance = default_middle_target_distance;
    target_distance = middle_target_distance;
    cnt_rotates = 0;

    
    pid_is_ready = false;
    turnover_is_ready = false;
    begin_pid_move = true;

    cnt_points = 0;
    cnt_segments = 0;

    pid_value = 0;
    time_turnover = 0;

    memset(is_sign, 0, 6);
    memset(is_rotate_sign, 0, 4);
    memset(exist_signs, 0, 8);
}
