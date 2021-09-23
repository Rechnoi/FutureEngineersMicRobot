#if BUTTON
const byte port_btn = 4;

// Инициализация кнопки
void btnSetup() {
    pinMode(port_btn, INPUT);
}

// Считывание состояния кнопки
bool btnReadState() {
    return digitalRead(port_btn);
}
#endif
