#include <Arduino.h>

#define ORANGELED LED_BUILTIN

void setup() {
    // put your setup code here, to run once:
    pinMode(ORANGELED, OUTPUT);
    digitalWrite(ORANGELED, LOW);   // set low to turn OFF

    Serial.begin(115200);
    while (!Serial)
        ;   // wait for serial port to connect. Needed for native USB

    delay(100);
    Serial.println("Hello");
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(ORANGELED, !(digitalRead(ORANGELED)));
    delay(50);
}
