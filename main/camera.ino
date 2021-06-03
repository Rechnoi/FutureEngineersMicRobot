/*#include "TrackingCamI2C.h"

TrackingCamI2C trackingCam;

void cameraSetup() {
    trackingCam.init(50, 100000);
}

void cameraDebugBlobs() {
    uint8_t n = trackingCam.readBlobs(5);
    Serial.println("All blobs");
    Serial.println(n);
    for(int i = 0; i < n; i++) {
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
*/
