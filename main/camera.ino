#include "TrackingCamI2C.h"

TrackingCamI2C trackingCam;

void cameraSetup() {
    trackingCam.init(50, 100000);
}

int num_blobs = 0;

void cameraReadBlobs() {
    num_blobs = trackingCam.readBlobs(5);
#if DEBUG_WRITE
    //cameraDebugBlobs();
#endif
}

unsigned long timer_read_camera = 0;

void cameraCheckTimer() {
    if (millis() - timer_read_camera >= 33) {
        timer_read_camera = millis();
        cameraReadBlobs();
        cameraCheckSign();
#if DEBUG_WRITE
        //cameraDebugBlobs();
#endif
    }
}

void cameraDebugBlobs() {
    if (num_blobs) {
        Serial.println("All blobs");
        Serial.println(num_blobs);
        for (int i = 0; i < num_blobs; i++) {
            Serial.print(trackingCam.blob[i].type, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].dummy, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].cx, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].cy, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].area, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].left, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].right, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].top, DEC);
            Serial.print(" ");
            Serial.print(trackingCam.blob[i].bottom, DEC);
            Serial.println();
        }
    }
}

void cameraDebugTmp() {
    for (int i = 0; i < num_blobs; i++) {
        Serial.print(trackingCam.blob[i].area);
        Serial.println();
    }
}

void cameraDebugAreaBlobs() {
    for (int i = 0; i < num_blobs; i++) {
        Serial.print(trackingCam.blob[i].area, DEC);
        Serial.println();
    }
}

#if DEBUG_LED_CAMERA
    bool on_debug_led_camera = true;
#endif

void cameraCheckSign() {
    //is_sign[GREEN_SIGN] = is_sign[RED_SIGN] = false;
    for (int i = 0; i < num_blobs; ++i) {
        if (trackingCam.blob[i].area > 100) {
            if (trackingCam.blob[i].type == 0) { //Если видим зелёный кубик
                if (!is_sign[GREEN_SIGN]) {
                    ledStatesLight(LED_RED);
                }
                is_sign[GREEN_SIGN] = true;
            } else if (trackingCam.blob[i].type == 1) { //Если видим красный кубик
                if (!is_sign[RED_SIGN]) {
                    ledStatesLight(LED_RED);
                }
                is_sign[RED_SIGN] = true;
            }
        }
    }
#if DEBUG_LED_CAMERA
    if ((on_debug_led_camera) && (!is_begin)) {
        if ((is_sign[GREEN_SIGN]) && (is_sign[RED_SIGN])) {
            ledLight(LED_MAGENTA);
        } else {
            if (is_sign[GREEN_SIGN]) {
                ledLight(LED_GREEN);
            } else if (is_sign[RED_SIGN]) {
                ledLight(LED_RED);
            } else {
                ledLight(LED_OFF);
            }
        }
    }
#endif
}

int getSign() {
    for (int i = 0; i < 2; ++i) {
        if (is_sign[i]) {
            return i;
        }
    }
    return NOT_SIGN;
}

void isSignClear() {
    is_sign[GREEN_SIGN] = false;
    is_sign[RED_SIGN] = false;
    //is_near_sign[GREEN_SIGN] = false;
    //is_near_sign[RED_SIGN] = false;
}
