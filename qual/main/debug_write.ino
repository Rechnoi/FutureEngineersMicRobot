// Инициализация отладочного вывода
void debugWriteSetup() {
    Serial.begin(500000);
}

int ptr_buff = 0;

// Добавление в буффер точки
void addBuffPoint(const Point& a) {
    buff[ptr_buff++] = a.x;
    buff[ptr_buff++] = a.y;
}

// Добавление в буффер отрезка
void addBuffSegment(const Segment& seg) {
    addBuffPoint(seg.A);
    addBuffPoint(seg.B);
}

// Добавление в буффер последовательности бит, обозначающих окончание отправки
void addBuffEnd() {
    buff[ptr_buff++] = 1 << 14;
}

// Добавление в буффер разделительной последовательности бит
void addBuffSep() {
    buff[ptr_buff++] = (1 << 14) - 1;
}

// Добавление в буффер значения типа int16_t
void addBuffInt16_t(uint16_t x) {
    buff[ptr_buff++] = x;
}

// Очистка буфера
void buffClear() {
    ptr_buff = 0;
}

// Вывод буффера
void sendBuff() {
    Serial.write((byte*)buff, ptr_buff << 1);
}

// Отладочный вывод
void debugWrite() {
    buffClear();
    for (int i = 0; i < cnt_points; ++i) {
        addBuffPoint(points[i]);
    }
    
    addBuffSep();
    for (int i = 0; i < 4; ++i) {
        addBuffSegment(border[i]);
    }

    addBuffSep();
    if ((!border[side_move_forward].empty()) && ((!border[BACK].empty()) || (!border[FORWARD].empty()))) {
        for (int i = 0; i < cnt_signs; ++i) {
            addBuffPoint(conv_rect_coord(signs[i]));
        }
    }
    
    addBuffSep();
    for (int side = 0; side < 4; ++side) {
        addBuffInt16_t(border[side].line.distToCenter());
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            addBuffInt16_t(is_sign[i][j]);
        }
    }
    
    addBuffInt16_t(cnt_signs);
    addBuffInt16_t(cnt_good_signs);
    addBuffInt16_t(target_distance);
    
    addBuffEnd();
    sendBuff();
}
