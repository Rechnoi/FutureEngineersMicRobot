const byte port_btn = 16;

void btnSetup() {
    pinMode(port_btn, INPUT);
}

bool readStateBtn() {
    return digitalRead(port_btn);
}
