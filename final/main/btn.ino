#if BUTTON
const byte port_btn = 4;

// Initializing the button
void btnSetup() {
    pinMode(port_btn, INPUT);
}

// Reading the button status
bool btnReadState() {
    return digitalRead(port_btn);
}
#endif
