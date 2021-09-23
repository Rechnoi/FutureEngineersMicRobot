//Point

Point::Point() {}

Point::Point(int16_t x, int16_t y) : x(x), y(y) {}

// Сложение двух векторов
Point operator + (const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

// Разность двух векторов
Point operator - (const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

// Деление вектора на число
Point operator / (const Point& a, int k) {
    return Point(a.x / k, a.y / k);
}

// Получение вектора с началом в первой точке и концом во второй точке
Point operator ^ (const Point& A, const Point& B) {
    return B - A;
}

// Унарный оператор минус от вектора
Point Point::operator - () const {
    return Point(-x, -y);
}

// Векторное произведение
int operator * (const Vector& a, const Vector& b) {
    return (int)a.x * b.y - (int)a.y * b.x;
}

// Скалярное произведение
int operator , (const Vector& a, const Vector& b) {
    return (int)a.x * b.x + (int)a.y * b.y;
}

// Проверка на равенство двух векторов
bool operator == (const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

// Проверка на неравенство двух векторов
bool operator != (const Point& a, const Point& b) {
    return a.x != b.x || a.y != b.y;
}

bool operator < (const Point& a, const Point& b) {
    if (a.y != b.y) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

// Получение квадрата длины вектора
int Vector::getSqrLen() const {
    return (int)x * x + (int)y * y;
}

// Получение длины вектора
double Vector::getLen() const {
    return sqrt(getSqrLen());
}

// Получение перпендикуляроного вектора
Vector Vector::getPerp() const {
    return Vector(y, -x);
}

// Получение ориентированного угла между векторами
double getAngle(const Vector& a, const Vector& b) {
    return atan2(a * b, (a, b));
}

Line::Line() {}

Line::Line(const Point& A, const Point& B) {
    this->A = A.y - B.y;
    this->B = B.x - A.x;
    this->C = A * B;
}

// Получение координаты x точки на прямой при заданном y
double Line::getX(int16_t y) const {
    return -1. * (B * y + C) / A;
}

// Получение координаты y точки на прямой при заданном x
double Line::getY(int16_t x) const {
    return -1. * (A * x + C) / B;
}

// Получение расстояние прямой до точки (0; 0)
double Line::distToCenter() const {
    return abs(C) / sqrt(A * A + B * B);
}

// Получение квадрата расстояния до заданной точки
int Line::sqrDistToPoint(const Point& P) const {
    int tmp = A * P.x + B * P.y + C;
    return (long long)tmp * tmp / (A * A + B * B);
}

// Получение расстояния до заданной точки
int Line::distToPoint(const Point& P) const {
    return sqrt(sqrDistToPoint(P));
}

// Пересечение двух прямых
Point operator & (const Line& a, const Line& b) {
    int16_t x = ((long long)b.B * a.C - (long long)a.B * b.C) / ((long long)b.A * a.B - (long long)a.A * b.B);
    int16_t y = ((long long)b.A * a.C - (long long)a.A * b.C) / ((long long)a.A * b.B - (long long)b.A * a.B);
    return Point(x, y);
}

//SEGMENT

Segment::Segment() {}

Segment::Segment(const Point& A, const Point& B) : A(A), B(B), line(A, B) {}

// Учтановка отрезка в пустой отрезок
void Segment::clear() {
    A = Point(0, 0);
    B = Point(0, 0);
}

// Провекрка отрезка на пустоту
bool Segment::empty() const {
    return (A == Point(0, 0)) && (B == Point(0, 0));
}

// Получение вектора отрезка
Vector Segment::getVector() const {
    return A ^ B;
}
