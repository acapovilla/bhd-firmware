/**
 * @file    main.cpp
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   Main program logic for the data logger system. It initializes
 * hardware components such as LEDs, SD card, RTC, temperature, and
 * hall sensors. The program uses interrupts for periodically data acquisition
 * and logs it to an SD card with a timestamp. It handles errors using LEDs and
 * serial messages. It also supports command input via serial for setting the
 * date and time.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>

#include "pin_definitions.h"
#include "error_codes.h"
#include "msg_codes.h"
#include "serial_number.h"
#include "sd_manager.h"
#include "rtc_controller.h"
#include "cmd_interpreter.h"
#include "temp_controller.h"
#include "hall_controller.h"

// Uncomment the following line to enable debug messages
// #define DEBUG

DateTime now;  // Variable to hold current time

// Interrupt flag from RTC alarm
volatile bool alarmFlag = false;

// Sensors measures
uint16_t hall_measures[6] = {0};
float temp_measure = 0;

// Human-readable time stamp for log file
char timestamp[] = "YYYY-MM-DD hh:mm:ss";

// Interrupt handler for RTC alarm
void onAlarm(void) {
    alarmFlag = true;
}

// Initialize GPIO pins
void GPIO_init(void) {
    pinMode(ERROR_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    // Shutdown LEDs
    digitalWrite(ERROR_LED, LED_OFF_STATE);
    digitalWrite(GREEN_LED, LED_OFF_STATE);

    pinMode(RTC_ALARM_PIN, INPUT);  // Disable pull-up

    pinMode(SDCARD_SPI_CS, OUTPUT);  // SD card chip select
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

    // Print startup message
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

    // If RTC date and time is not valid, wait until it is set correctly
    while (!RTC_dateTimeValid()) {
        // Flash ERROR_LED at 1 Hz
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

    // Initialize ADC and sleep GPIO for hall sensors
    ADC_init();
    HALL_initIO(HALL_SLEEP_GROUP0, HALL_SLEEP_GROUP1);

    now = RTC_getNow();  // get the updated time

    // Initialize logfile name
    SDCard_initFileName(now.year(), now.month(), now.day(), now.hour(),
                        now.minute(), now.second(), sn);

    // oldDay = now.day();

    // Start-up finished and error cleared
    Serial.flush();
    digitalWrite(ERROR_LED, LED_OFF_STATE);  // turn off ERROR_LED

    /** -------------------------------------------------------
     * End of setup and configuration section
     * ------------------------------------------------------- */

    // schedule an alarm 10 seconds in the future
    if (!RTC_1secondAlarm()) {
#ifdef DEBUG
        Serial.println("Error, alarm wasn't set!");
#endif
        /// @todo Handle error and not continue
    } else {
#ifdef DEBUG
        Serial.println("Alarm will happen in 1 seconds!");
#endif
    }

    // The alarm will trigger an interrupt
    pinMode(RTC_ALARM_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(2), onAlarm, FALLING);

    Serial.print(MSG_SYS_READY_short);
    Serial.print(',');
    Serial.println(MSG_SYS_READY_str);
}

void loop() {
    if (alarmFlag) {        // If alarm was triggered
        alarmFlag = false;  // Clear the flag

        // Show that sensor read and process is running
        digitalWrite(GREEN_LED, LED_ON_STATE);

        // Update now
        now = RTC_getNow();

        // Print current time to Serial
        Serial.print(now.unixtime(), DEC);
        Serial.print(F(","));  // POSIX time value

        // Read all six hall sensors
        HALL_read(HALL_SLEEP_GROUP0, HALL_SLEEP_GROUP1, hall_measures);

        // Read temperature sensor
        temp_measure = TEMP_read();

        // Create timestamp for logfile
        printTimeToBuffer(now, timestamp);
        // Write values to SD
        SDCard_writeFile(now.unixtime(), timestamp, hall_measures,
                         temp_measure);

        // Print hall sensor values and temperature to Serial
        for (uint8_t i = 0; i < 6; ++i) {
            Serial.print(hall_measures[i]);
            Serial.print(',');
        }
        Serial.print(temp_measure, 2);

        // End of logging line
        Serial.println();
        Serial.flush();

        // Next 1s alarm
        RTC_1secondAlarm();

        // Turn-off green LED to show that the process is done
        digitalWrite(GREEN_LED, LED_OFF_STATE);
    }

    CMD_readCommand();
}
