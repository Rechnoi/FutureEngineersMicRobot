#include "PinChangeInterrupt.h"

const char table[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};

byte now_enc;
byte old_enc = ((PINB >> 3) & 1) ^ (PINB & 0b10);

void encoderSetup() {
    attachPCINT(digitalPinToPCINT(14), encoderInterrupt, CHANGE);
    attachPCINT(digitalPinToPCINT(15), encoderInterrupt, CHANGE);
}

void encoderInterrupt() {
    now_enc = ((PINB >> 3) & 1) ^ (PINB & 0b10);
    encoder_value += table[(old_enc << 2) ^ now_enc];
    old_enc = now_enc;
}
