#include <Arduino.h>

#include "pin_definitions.h"
#include "error_codes.h"
#include "serial_number.h"
#include "sd_manager.h"
#include "rtc_controller.h"
#include "cmd_interpreter.h"
#include "temp_controller.h"
#include "hall_controller.h"

// #define DEBUG

DateTime now;  // Variable to hold current time

// Interrupt flag from RTC alarm
volatile bool alarmFlag = false;

// Sensors measures
uint16_t hall_measures[6] = {0};
float temp_measure = 0;

// Human-readable time stamp for log file
char timestamp[] = "YYYY-MM-DD hh:mm:ss";

void onAlarm(void) {
    alarmFlag = true;
}

void GPIO_init(void) {
    pinMode(ERROR_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    digitalWrite(ERROR_LED, LED_OFF_STATE);
    digitalWrite(GREEN_LED, LED_OFF_STATE);

    pinMode(RTC_ALARM_PIN, INPUT);  // Disable pull-up

    pinMode(SDCARD_SPI_CS, OUTPUT);
}

void setup() {
    // Initialize basic I/O pins (without drivers or controllers)
    GPIO_init();

    // Start serial USB
    Serial.begin(115200);

    // Flash green LED to show that we just booted up
    for (uint8_t _i = 0; _i < 3; ++_i) {
        digitalWrite(GREEN_LED, LED_ON_STATE);
        delay(100);
        digitalWrite(GREEN_LED, LED_OFF_STATE);
        delay(100);
    }

    Serial.println("<Arduino ready>");
    delay(500);

    /** -------------------------------------------------------
     * Start setup and configuration section
     * ------------------------------------------------------- */
    digitalWrite(ERROR_LED, LED_ON_STATE);

    uint16_t sn = 0;
    if (getSerialNumber(sn)) {  // If serial number is valid
#ifdef DEBUG
        Serial.print("Serial number: ");  // Print via serial
        Serial.println(sn);
#endif
    } else {  // If not valid
        Serial.print(ERROR_SN_NOTVALID_short);
        Serial.print(',');
        Serial.println(ERROR_SN_NOTVALID_str);  // Error
    }

    if (SDCard_init()) {  // Try start SD card
#ifdef DEBUG
        Serial.println("SD card initialized.");
#endif
    } else {  // If error
        Serial.print(ERROR_SDCARD_INITFAIL_short);
        Serial.print(',');
        Serial.println(ERROR_SDCARD_INITFAIL_str);

        // SD init fail -> Flash ERROR_LED 2 times 1 Hz
        while (1) {
            digitalWrite(ERROR_LED, LED_ON_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_OFF_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_ON_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_OFF_STATE);
            delay(1000);
        }
    }

    // Start external RTC
    uint8_t r = RTC_initExternal();
    if (r == 10) {
        Serial.print(ERROR_RTCEXT_INITFAIL_short);
        Serial.print(',');
        Serial.println(ERROR_RTCEXT_INITFAIL_str);

        // RTC init fail -> Flash ERROR_LED 1 time 1 Hz
        while (1) {
            digitalWrite(ERROR_LED, LED_ON_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_OFF_STATE);
            delay(1000);
        }
    } else if (r == 11) {  // RTC lost power
        Serial.print(ERROR_RTCEXT_LOSTPWR_short);
        Serial.print(',');
        Serial.println(ERROR_RTCEXT_LOSTPWR_str);
    } else if (r == 13) {  // RTC wrong date
        Serial.print(ERROR_RTCEXT_WRONGDT_short);
        Serial.print(',');
        Serial.println(ERROR_RTCEXT_WRONGDT_str);
    } else if (r == 0) {  // Success and apparently correct date and time
#ifdef DEBUG
        Serial.println(F("RTC initialized."));
#endif

        printTimeToSerial();  // Print actual date and time
    }

    while (!RTC_dateTimeValid()) {
        digitalWrite(ERROR_LED, !digitalRead(ERROR_LED));
        for (uint8_t _i = 0; _i < 10; ++_i) {
            CMD_readCommand();
            delay(50);
        }
    }

    // Initialize one-wire temperature sensor
    if (TEMP_init()) {
        Serial.print(ERROR_TEMPEXT_INITFAIL_short);
        Serial.print(',');
        Serial.println(ERROR_TEMPEXT_INITFAIL_str);

        while (1) {  // Temperature sensor fail -> Flash ERROR_LED 3 time 1 Hz
            digitalWrite(ERROR_LED, LED_ON_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_OFF_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_ON_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_OFF_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_ON_STATE);
            delay(50);
            digitalWrite(ERROR_LED, LED_OFF_STATE);
            delay(3000);
        }
    }

    ADC_init();
    HALL_initIO(HALL_SLEEP_GROUP0, HALL_SLEEP_GROUP1);

    now = RTC_getNow();  // get the updated time

    // Initialize logfile name
    SDCard_initFileName(now.year(), now.month(), now.day(), now.hour(),
                        now.minute(), now.second(), sn);

    // oldDay = now.day();

    // Start-up finished and error cleared
    Serial.flush();
    digitalWrite(ERROR_LED, LED_OFF_STATE);  // turn off

    /** -------------------------------------------------------
     * End of setup and configuration section
     * ------------------------------------------------------- */

    // schedule an alarm 10 seconds in the future
    if (!RTC_10secondsAlarm()) {
#ifdef DEBUG
        Serial.println("Error, alarm wasn't set!");
#endif
    } else {
#ifdef DEBUG
        Serial.println("Alarm will happen in 10 seconds!");
#endif
    }

    // Making it so, that the alarm will trigger an interrupt
    pinMode(RTC_ALARM_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(2), onAlarm, FALLING);
}

void loop() {
    if (alarmFlag) {
        alarmFlag = false;

        // Show that sensor read and process is running
        digitalWrite(GREEN_LED, LED_ON_STATE);

        // Update now
        now = RTC_getNow();

#ifdef DEBUG
        printTimeToSerial(now);
#endif

        // Read all six hall sensors
        HALL_read(HALL_SLEEP_GROUP0, HALL_SLEEP_GROUP1, hall_measures);

        // Read temperature sensor
        temp_measure = TEMP_read();

        // Create timestamp for logfile
        printTimeToBuffer(now, timestamp);
        // Write values to SD
        SDCard_writeFile(now.unixtime(), timestamp, hall_measures,
                         temp_measure);

#ifdef DEBUG
        for (uint8_t i = 0; i < 6; ++i) {
            Serial.print(hall_measures[i]);
            Serial.print(',');
        }
        Serial.println();
        Serial.flush();
#endif

        // Next 10s alarm
        RTC_10secondsAlarm();

        // Turn-off all
        digitalWrite(GREEN_LED, LED_OFF_STATE);
    }

    CMD_readCommand();
}
