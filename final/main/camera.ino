#include "TrackingCamI2C.h"

TrackingCamI2C trackingCam;

// Инициализация камеры
void cameraSetup() {
    trackingCam.init(50, 100000);
}

int num_blobs = 0;

// Считывание объектов с камеры
void cameraReadBlobs() {
    num_blobs = trackingCam.readBlobs(5);
}

unsigned long timer_read_camera = 0;

// Считывание с камеры
void cameraRead() {
    if (millis() - timer_read_camera >= 33) {
        timer_read_camera = millis();
        cameraReadBlobs();
    }
}

// Дебаговый вывод с камеры
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
