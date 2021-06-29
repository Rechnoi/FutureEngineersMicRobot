const byte port_led_red = A0;
const byte port_led_green = A1;
const byte port_led_blue = A2;

void ledSetup() {
    pinMode(port_led_red, OUTPUT);
    pinMode(port_led_green, OUTPUT);
    pinMode(port_led_blue, OUTPUT);

#if DEBUG_LED_CAMERA || DEBUG_STATES
    ledLight(LED_OFF);
#else
    ledLight(LED_GREEN);
#endif
}

int color_led;

void ledLight(int color) {
    color_led = color;
    switch (color) {
        case LED_RED:
            digitalWrite(port_led_red, 1);
            digitalWrite(port_led_green, 0);
            digitalWrite(port_led_blue, 0);
            break;
        case LED_GREEN:
            digitalWrite(port_led_red, 0);
            digitalWrite(port_led_green, 1);
            digitalWrite(port_led_blue, 0);
            break;
        case LED_BLUE:
            digitalWrite(port_led_red, 0);
            digitalWrite(port_led_green, 0);
            digitalWrite(port_led_blue, 1);
            break;
        case LED_CYAN:
            digitalWrite(port_led_red, 0);
            digitalWrite(port_led_green, 1);
            digitalWrite(port_led_blue, 1);
            break;
        case LED_YELLOW:
            digitalWrite(port_led_red, 1);
            digitalWrite(port_led_green, 1);
            digitalWrite(port_led_blue, 0);
            break;
        case LED_MAGENTA:
            digitalWrite(port_led_red, 1);
            digitalWrite(port_led_green, 0);
            digitalWrite(port_led_blue, 1);
            break;
        case LED_WHITE:
            digitalWrite(port_led_red, 1);
            digitalWrite(port_led_green, 1);
            digitalWrite(port_led_blue, 1);
            break;
        case LED_OFF:
            digitalWrite(port_led_red, 0);
            digitalWrite(port_led_green, 0);
            digitalWrite(port_led_blue, 0);
            break;
    }
}

#if DEBUG_LED_CHANGE
void ledChangeColor() {
    if (color_led == LED_GREEN) {
        ledLight(LED_RED);
    } else {
        ledLight(LED_GREEN);
    }
}
#endif

#if DEBUG_LED_STATES
void ledStatesLight(int color) {
    ledLight(color);
}
#else
void ledStatesLight(int color) {}
#endif
