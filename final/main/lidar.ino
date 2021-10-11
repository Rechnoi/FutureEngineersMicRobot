#include "analogWrite.h"

const byte port_lidar_motor_dir = 32;
const byte port_lidar_motor_pwm = 33;

#define lidarSendBuff(buff) Serial2.write(buff, sizeof(buff))

const byte buff_reset[] = {0xA5, 0x40};

// Restarting lidar
void lidarReset() {
    lidarSendBuff(buff_reset);
}

const byte buff_start_scan[] = {0xA5, 0x20};

// Starting the scan
void lidarStartScan() {
    lidarSendBuff(buff_start_scan);
}

const byte buff_start_express_scan[] = {0xA5, 0x82, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22};

// Starting express scanning
void lidarStartExpressScan() {
    lidarSendBuff(buff_start_express_scan);
}

// Reading the lidar response
void lidarReadDescription() {
    for (int i = 0; i < 7; ++i) {
        while (!Serial2.available()) {}
        Serial2.read();
    }
}

// Setting the lidar motor speed
void lidarMotorWrite(int speed_motor) {
    digitalWrite(port_lidar_motor_dir, HIGH);
    analogWrite(port_lidar_motor_pwm, 255 - speed_motor, 10000); 
}

// Initializing lidar
void lidarSetup() {
    pinMode(port_lidar_motor_dir, OUTPUT);
    lidarMotorWrite(255);
    delay(100);
    Serial2.begin(115200);
    lidarReset();
    delay(1000);
    while (Serial2.available()) {
        Serial2.read();
    }
    lidarStartExpressScan();
    lidarReadDescription();
}

// Stopping lidar
void lidarStop() {
    lidarMotorWrite(0);
}

byte lidar_buff[84];
byte lidar_old_buff[84];

int lidar_buff_ptr = 0;

int cnt = 0;

// Reading information from lidar
void lidarRead() {
    if (!Serial2.available()) {
        return;
    }
    byte current_byte = Serial2.read();
    if (lidar_buff_ptr == 0) {
        if ((current_byte >> 4) != 0xA) {
            return;
        }
    } else if (lidar_buff_ptr == 1) {
        if ((current_byte >> 4) == 0xA) {
            lidar_buff_ptr = 0;
        } else if ((current_byte >> 4) != 0x5) {
            lidar_buff_ptr = 0;
            return;
        }
    }

    lidar_buff[lidar_buff_ptr++] = current_byte;

    if (lidar_buff_ptr == 4) {
        lidarProcessingData();
    }

    if (lidar_buff_ptr == 84) {
        memcpy(lidar_old_buff, lidar_buff, 84);
        lidar_buff_ptr = 0;
    }
}

// Getting angle compensation
double getDiffAngle(byte data_angle) {
    bool sign = data_angle >> 7;
    double diff_angle = (double)(data_angle & 0b1111111) / (1 << 3);
    if (sign) {
        diff_angle = -diff_angle;
    }
    return diff_angle;
}

double d0[2];
uint16_t distance[2];

double old_start_angle = 0;

// Getting the difference between the angles
double angleDiff(double old_angle, double now_angle) {
    return old_angle <= now_angle ? now_angle - old_angle : 360 + now_angle - old_angle;
}

// Processing data from the lidar
void lidarProcessingData() {
    double start_angle = (((uint16_t)(lidar_buff[3] & 0b01111111) << 8) ^ lidar_buff[2]) / 64.;
    double old_start_angle = (((uint16_t)(lidar_old_buff[3] & 0b01111111) << 8) ^ lidar_old_buff[2]) / 64.;

    for (int i = 0; i < 16; ++i) {
        byte* cabin = lidar_old_buff + 5 * i + 4;
        
        d0[0] = getDiffAngle(((cabin[0] & 0b11) << 4) ^ (cabin[4] & 0b1111));
        d0[1] = getDiffAngle(((cabin[2] & 0b11) << 4) ^ (cabin[4] >> 4));

        for (int i = 0; i < 2; ++i) {
            distance[i] = ((uint16_t)cabin[2 * i + 1] << 6) ^ (cabin[2 * i] >> 2);
        }

        for (int j = 0; j < 2; ++j) {
            int k = 2 * i + 1 + j;
            double angle = old_start_angle + angleDiff(old_start_angle, start_angle) / 32 * k - d0[j];
            if (angle >= 360) {
                angle -= 360;
            }
            
            processingPoint(distance[j], angle);
        }
    }
}

double old_angle = 0;

// Point Processing
void processingPoint(uint16_t distance, double angle) {
    if ((old_angle - angle >= 90) && (cnt_points >= 10)) {
        processingTurnover();
#if DEBUG
        debugWrite();
#endif
        cnt_points = 0;
    }
    old_angle = angle;

    if ((distance != 0) && (distance < 4000)) {
        angle = radians(angle);
        points[cnt_points++] = Point(distance * sin(angle), distance * cos(angle));
    }
}
