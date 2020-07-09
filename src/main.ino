#include <Arduino.h>
#include <VarSpeedServo.h>
#include <HCSR04.h>

// lights on LOW
constexpr int LED_R_PIN = 3; 
constexpr int LED_G_PIN = 5;
constexpr int LED_B_PIN = 6;
constexpr int SERVO_PIN = 10;
constexpr int ECHO_PIN = 4;
constexpr int TRIG_PIN = 2;
constexpr int BUTTON_PIN = 8;

constexpr int DISTANCE_THRESH = 35; // cm
constexpr int SERVO_LIFT_UP = 59; // deg
constexpr int SERVO_NORMAL = 180; // deg
constexpr int SERVO_SPEED_UP = 40; // %
constexpr int SERVO_SPEED_DOWN = 15; // %
constexpr int OPEN_TIME = 3000; // ms
constexpr int REACTIVE_TIME = 5000; // ms (don't lift up for REACTIVE_TIME after closing trash by button)

//#define DEBUG
#ifdef DEBUG
auto &Debug = Serial;
#else
struct DummySerial {
    template<typename T>
    void print(T &) {}

    template<typename T>
    void println(T &) {}

    void begin(int) {}
} Debug;
#endif

void ledOn(int ledPin);
void ledOff(int ledPin);
void holdUp();
bool isObstacleClose(const double &distance);
void blink(int duration);
void closeBin();
void openBin();
bool isButtonPressed();

VarSpeedServo servo;
UltraSonicDistanceSensor distanceSensor(TRIG_PIN, ECHO_PIN);

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED_R_PIN, OUTPUT);
    pinMode(LED_G_PIN, OUTPUT);
    pinMode(LED_B_PIN, OUTPUT);
    
    digitalWrite(LED_R_PIN, HIGH);
    digitalWrite(LED_G_PIN, HIGH);
    digitalWrite(LED_B_PIN, HIGH);
    
    ledOn(LED_R_PIN);

    pinMode(SERVO_PIN, OUTPUT);
    digitalWrite(SERVO_PIN, LOW);

    servo.attach(SERVO_PIN);
    servo.write(SERVO_NORMAL);

    Debug.begin(115200);
}

void loop()
{
    double distance = distanceSensor.measureDistanceCm();
    bool isClose = isObstacleClose(distance);

    if(isButtonPressed()) {
        holdUp();
        return;
    }

    if(isClose) {
        ledOn(LED_G_PIN);
        openBin();

        constexpr int tickInterval = 10;
        constexpr int openTicks = OPEN_TIME / tickInterval;

        for(int i = 0; i < openTicks; i++) {
            if(isButtonPressed()) {
                ledOff(LED_G_PIN);
                holdUp();
                return;
            }
            distance = distanceSensor.measureDistanceCm();
            if(isObstacleClose(distance)) {
                i = 0;
            }
            bool isTooFar = distance == -1.0 || distance >= DISTANCE_THRESH * 3;
            if(isTooFar) {
                i += 100 / tickInterval;
            }
            delay(tickInterval);
        }
        closeBin();
        ledOff(LED_G_PIN);
    }
}

bool isObstacleClose(const double &distance) {
    Debug.print("Distance: ");
    Debug.println(distance);
    return distance >= 4.0 && distance <= DISTANCE_THRESH;;
}

bool isButtonPressed() {
    return digitalRead(BUTTON_PIN) == LOW;
}

void holdUp() {
    ledOn(LED_B_PIN);
    openBin();
    delay(3000);
    while(!isButtonPressed()) {
        delay(1);
    }
    closeBin();
    ledOff(LED_B_PIN);
    blink(REACTIVE_TIME);
}

void ledOn(int ledPin) {
    analogWrite(ledPin, 1022);
}

void openBin() {
    servo.write(SERVO_LIFT_UP, SERVO_SPEED_UP, false);
}

void closeBin() {
    servo.write(SERVO_NORMAL, SERVO_SPEED_DOWN, false);
}

void ledOff(int ledPin) {
    digitalWrite(ledPin, HIGH);
}

void blink(int duration) {
    int ticks = duration / 1000;
    while(ticks > 0) {
        ledOff(LED_R_PIN);
        delay(500);
        ledOn(LED_R_PIN);
        delay(500);
        ticks--;
    }
}
