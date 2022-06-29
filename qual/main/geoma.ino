Point queue[MAX_CNT_POINTS];

int queue_ptr_begin;
int queue_ptr_end;

// Initializing the queue
void queue_init(const Point& A) {
    queue_ptr_begin = 0;
    queue[0] = A;
    queue_ptr_end = 1;
}

bool used[MAX_CNT_POINTS];

const int MAX_DIST_COMP = 150;
const int SQR_MAX_DIST_COMP = MAX_DIST_COMP * MAX_DIST_COMP;

Segment local_border[4];

// Dividing points into components
void splitComponent() {
    for (int i = 0; i < 4; ++i) {
        local_border[i].clear();
    }
    
    memset(used, 0, cnt_points);
    cnt_segments = 0;
    cnt_signs = 0;
    for (int i = 0; i < cnt_points; ++i) {
        if (!used[i]) {
            queue_init(points[i]);
            used[i] = true;
            while (queue_ptr_begin < queue_ptr_end) {
                Point A = queue[queue_ptr_begin++];
                for (int j = 0; j < cnt_points; ++j) {
                    if ((!used[j]) && ((A ^ points[j]).getSqrLen() <= SQR_MAX_DIST_COMP)) {
                        queue[queue_ptr_end++] = points[j];
                        used[j] = true;
                    }
                }
            }
            processingComponent(queue, queue_ptr_end);
        }
    }
    for (int i = 0; i < 4; ++i) {
        border[i] = local_border[i];
    }
}

// Processing the component
void processingComponent(Point* component, int size_component) {
    if (size_component >= 2) {
        int sqr_max_dist = 0;
        Point ans1;
        Point ans2;
        for (int i = 0; i < size_component; ++i) {
            for (int j = 0; j < i; ++j) {
                int sqr_dist = (component[i] ^ component[j]).getSqrLen();
                if (sqr_dist > sqr_max_dist) {
                    sqr_max_dist = sqr_dist;
                    ans1 = component[i];
                    ans2 = component[j];
                }
            }
        }
        if (size_component >= 5) {
            Line line(ans1, ans2);
            int cnt = 0;
            for (int i = 0; i < size_component; ++i) {
                if (line.distToPoint(component[i]) > 100) {
                    ++cnt;
                }
            }
            if (cnt <= 3) {
                Segment ans_segment(ans1, ans2);
                processingSegmentBorder(ans_segment);
            } else {
                Point ans3;
                int max_val = 0;
                for (int i = 0; i < size_component; ++i) {
                    int curr_val = (ans1 ^ component[i]).getLen() + (ans2 ^ component[i]).getLen();
                    if (curr_val > max_val) {
                        max_val = curr_val;
                        ans3 = component[i];
                    }
                }
                processingSegmentBorder(Segment(ans1, ans3));
                processingSegmentBorder(Segment(ans2, ans3));
            }
        }
    }
}

// We translate the point to another coordinate system the point
Point conv_rect_coord(const Point& A) {
    int y = !border[BACK].empty() ? border[BACK].line.distToPoint(A) : 3000 - border[FORWARD].line.distToPoint(A);
    int x = side_move_forward == LEFT ? border[LEFT].line.distToPoint(A) : 1000 - border[RIGHT].line.distToPoint(A);
    return Point(x, y);
}

// Processing of the wall segment
void processingSegmentBorder(const Segment& segment) {
    Vector v = segment.getVector();
    double angle = abs(atan2(v.y, v.x));

    if (abs(angle - PI / 2) < radians(45)) {
        if ((segment.line.getX(0) < 0)
            && ((local_border[LEFT].empty()) || (segment.line.distToCenter() > local_border[LEFT].line.distToCenter()))) {
            local_border[LEFT] = segment;
        }
        if ((segment.line.getX(0) > 0)
            && ((local_border[RIGHT].empty()) || (segment.line.distToCenter() > local_border[RIGHT].line.distToCenter()))) {
            local_border[RIGHT] = segment;
        }
    } else if (min(angle, PI - angle) < radians(45)) {
        if (((segment.line.getY(0) < 0)
            && ((local_border[BACK].empty()) || (segment.line.distToCenter() > local_border[BACK].line.distToCenter())))) {
            local_border[BACK] = segment;
        }
        if (((segment.line.getY(0) > 0)
            && ((local_border[FORWARD].empty()) || (segment.line.distToCenter() > local_border[FORWARD].line.distToCenter())))) {
            local_border[FORWARD] = segment;
        }
    }
}
