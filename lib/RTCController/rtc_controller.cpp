/**
 * @file    rtc_controller.cpp
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#include "rtc_controller.h"

RTC_DS3231 DS3231;

bool _clockErrorFlag = true;
bool _dateTimeValid = false;

uint8_t RTC_initExternal(void) {
    if (!DS3231.begin()) {  // Initialize RTC communications
        // Unable to find DS3231
        return 10;
    }

    _clockErrorFlag = false;

    if (DS3231.lostPower()) {  // Error, clock isn't set
        return 11;
    }

    // Check that real time clock has a reasonable time value
    uint16_t _year = DS3231.now().year();
    if (_year < 2023 | _year > 2027) {
        return 13;
    }

    /// @todo Almacenar la hora en una dirección de la EEPROM para luego leerla
    /// y comprobar que la actual sea futura por si pasa todos los chequeos
    _dateTimeValid = true;

    // we don't need the 32K Pin, so disable it
    DS3231.disable32K();

    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset
    // on reboot/recompile
    DS3231.clearAlarm(1);
    DS3231.clearAlarm(2);

    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    DS3231.writeSqwPinMode(DS3231_OFF);

    // turn off alarm 2 (in case it isn't off already)
    // again, this isn't done at reboot, so a previously set alarm could easily
    // go overlooked
    DS3231.disableAlarm(2);

    return 0;
}

bool RTC_dateTimeValid(void) {
    return _dateTimeValid;
}

bool setDateAndTime(const uint16_t &year, const uint16_t &month,
                    const uint16_t &day, const uint16_t &hour,
                    const uint16_t &minute, const uint16_t &second) {
    DS3231.adjust(DateTime(year, month, day, hour, minute, second));
    if (!DS3231.lostPower()) {
        _dateTimeValid = true;
    }
    return DS3231.lostPower();
}

void printTimeToSerial(const DateTime &dt) {
    Serial.print(dt.year(), DEC);
    Serial.print('-');
    Serial.print(dt.month(), DEC);
    Serial.print('-');
    Serial.print(dt.day(), DEC);
    Serial.print(' ');
    Serial.print(dt.hour(), DEC);
    Serial.print(':');
    if (dt.minute() < 10) {
        Serial.print("0");
    }
    Serial.print(dt.minute(), DEC);
    Serial.print(':');
    if (dt.second() < 10) {
        Serial.print("0");
    }
    Serial.print(dt.second(), DEC);
    Serial.println();

    // char buf[] = "YYYY-MM-DD hh:mm:ss";
    // Serial.println(now.toString(buf));
}

void printTimeToSerial(void) {
    printTimeToSerial(DS3231.now());
}