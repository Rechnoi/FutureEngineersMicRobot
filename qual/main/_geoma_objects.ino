Point::Point() {}

Point::Point(int16_t x, int16_t y) : x(x), y(y) {}

// Sum of two vectors
Point operator + (const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

// The difference of two vectors
Point operator - (const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

// Dividing a vector by a number
Point operator / (const Point& a, int k) {
    return Point(a.x / k, a.y / k);
}

// Getting a vector with a start at the first point and an end at the second point
Point operator ^ (const Point& A, const Point& B) {
    return B - A;
}

// Unary operator minus from vector
Point Point::operator - () const {
    return Point(-x, -y);
}

// Vector product
int operator * (const Vector& a, const Vector& b) {
    return (int)a.x * b.y - (int)a.y * b.x;
}

// Scalar product
int operator , (const Vector& a, const Vector& b) {
    return (int)a.x * b.x + (int)a.y * b.y;
}

// Checking for equality of two vectors
bool operator == (const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

// Checking for the inequality of two vectors
bool operator != (const Point& a, const Point& b) {
    return a.x != b.x || a.y != b.y;
}

// Getting the square of the vector length
int Vector::getSqrLen() const {
    return (int)x * x + (int)y * y;
}

// Getting the length of the vector
double Vector::getLen() const {
    return sqrt(getSqrLen());
}

// Getting a perpendicular vector
Vector Vector::getPerp() const {
    return Vector(y, -x);
}

// Getting the oriented angle between vectors
double getAngle(const Vector& a, const Vector& b) {
    return atan2(a * b, (a, b));
}

Line::Line() {}

Line::Line(const Point& A, const Point& B) {
    this->A = A.y - B.y;
    this->B = B.x - A.x;
    this->C = A * B;
}

// Getting the x coordinate of a point on a straight line at a given y
double Line::getX(int16_t y) const {
    return -1. * (B * y + C) / A;
}

// Getting the y coordinate of a point on a straight line for a given x
double Line::getY(int16_t x) const {
    return -1. * (A * x + C) / B;
}

// Getting the distance of a straight line to a point (0; 0)
double Line::distToCenter() const {
    return abs(C) / sqrt(A * A + B * B);
}

// Getting the square of the distance to a given point
int Line::sqrDistToPoint(const Point& P) const {
    int tmp = A * P.x + B * P.y + C;
    return (long long)tmp * tmp / (A * A + B * B);
}

// Getting the distance to a given point
int Line::distToPoint(const Point& P) const {
    return sqrt(sqrDistToPoint(P));
}

// Intersection of two straight lines
Point operator & (const Line& a, const Line& b) {
    int16_t x = ((long long)b.B * a.C - (long long)a.B * b.C) / ((long long)b.A * a.B - (long long)a.A * b.B);
    int16_t y = ((long long)b.A * a.C - (long long)a.A * b.C) / ((long long)a.A * b.B - (long long)b.A * a.B);
    return Point(x, y);
}

//SEGMENT

Segment::Segment() {}

Segment::Segment(const Point& A, const Point& B) : A(A), B(B), line(A, B) {}

// Setting a segment to an empty segment
void Segment::clear() {
    A = Point(0, 0);
    B = Point(0, 0);
}

// Checking the segment for emptiness
bool Segment::empty() const {
    return (A == Point(0, 0)) && (B == Point(0, 0));
}

// Getting the segment vector
Vector Segment::getVector() const {
    return A ^ B;
}
