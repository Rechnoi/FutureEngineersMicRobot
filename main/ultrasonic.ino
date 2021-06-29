#include <NewPing.h>

const byte port_left = 8;
const byte port_right = 7;
const byte port_forward = 4;

#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 //20 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[ULTRASONIC_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[ULTRASONIC_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;            // Keeps track of which sensor is active.

NewPing sonar[ULTRASONIC_NUM] = {     // Sensor object array.
    NewPing(port_left, port_left, 100),    // Each sensor's trigger pin, echo pin, and max distance to ping.
    NewPing(port_right, port_right, 100),
    NewPing(port_forward, port_forward, 333),
};

void ultrasonicSetup() {
    pingTimer[0] = millis() + 75;             // First ping starts at 75ms, gives time for the Arduino to chill before starting.
    for (uint8_t i = 1; i < ULTRASONIC_NUM; i++) { // Set the starting time for each sensor.
        pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
    } 
}

void ultrasonicCheckTimer() {
    for (uint8_t i = 0; i < ULTRASONIC_NUM; i++) { // Loop through all the sensors.
        if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
            pingTimer[i] += PING_INTERVAL * ULTRASONIC_NUM;    // Set next time this sensor will be pinged.
            if (i == 0 && currentSensor == ULTRASONIC_NUM - 1) {
                oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
            }
            sonar[currentSensor].timer_stop();            // Make sure previous timer is canceled before starting a new ping (insurance).
            currentSensor = i;                            // Sensor being accessed.
            cm[currentSensor] = 0;                        // Make distance zero in case there's no ping echo for this sensor.
            sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
        }
    }
    // Other code that *DOESN'T* analyze ping results can go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
    if (sonar[currentSensor].check_timer()) {
        cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
    }
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
    // The following code would be replaced with your code that does something with the ping results.
    for (int i = 0; i < ULTRASONIC_NUM; ++i) {
        distance[i] = cm[i];
    }
    is_ultrasonic_ready = true;
#if DEBUG_WRITE
    ultrasonicDebug();
    //Serial.println(distance[FORWARD_ULTRASONIC]);
#endif
}

void ultrasonicForwardUpdate() {
    ultrasonic_move_forward = side_to_dir(now_side);
    targetDistanceUpdate();
}

void ultrasonicDebug() {
    Serial.print("left ultrasonic: ");
    Serial.print(distance[LEFT_ULTRASONIC]);
    Serial.println(" cm");
    
    Serial.print("right ultrasonic: ");
    Serial.print(distance[RIGHT_ULTRASONIC]);
    Serial.println(" cm");
    
    Serial.print("forward ultrasonic: ");
    Serial.print(distance[FORWARD_ULTRASONIC]);
    Serial.println(" cm");
    Serial.println();
}
