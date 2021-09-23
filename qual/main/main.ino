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

#define Vector Point

bool side_move_forward = RIGHT;
int16_t dir_rotate = -1;

int16_t debug_data = 0;

const int main_target_distance = 360;

int target_distance = main_target_distance;

int cnt_signs;
int cnt_good_signs;

// Класс точки
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

// Класс прямой
// Прямая задаётся уравнением A * x + B * y + C = 0
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

// Класс отрезка
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

Point signs[15];

bool is_sign[2][3] = {{0, 0, 0}, {0, 0, 0}};

double pid_error;
double pid_value = 0;

const int MAX_CNT_POINTS = 2000;

Point points[MAX_CNT_POINTS];
int cnt_points = 0;

int cnt_segments = 0;

int16_t buff[4000];

Segment border[4];

bool pid_is_ready = false;

bool begin_pid_move = true;

// Ожидание стабилизации лидара
void waitStabilization() {
    unsigned long time_begin = millis();
    while (millis() < time_begin + 2000) {
        lidarRead();
    }
}

void setup() {
#if DEBUG
    debugWriteSetup();
#endif
    servoSetup();
    motorSetup();
    cameraSetup();
#if BUTTON
    btnSetup();
    while (!btnReadState()) {}
#endif
    lidarSetup();
    waitStabilization();
    
    solve();
}

void loop() {}

// Обработка информации, полученной за оборот лидара
void processingTurnover() {
    splitComponent();

    pid_is_ready = !border[side_move_forward].empty();
}
