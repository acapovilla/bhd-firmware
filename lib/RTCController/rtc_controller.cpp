/**
 * @file    rtc_controller.cpp
 * @author  Agustín Capovilla
 * @date    2024-01
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

#include "rtc_controller.h"

RTC_DS3231 DS3231;

bool _clockErrorFlag = true;
bool _dateTimeValid = false;
bool _alarmSetFlag = false;

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

DateTime RTC_getNow(void) {
    return DS3231.now();
}

bool RTC_1secondAlarm(void) {
    // Disable SQW output
    DS3231.writeSqwPinMode(DS3231_OFF);

    // Clear older alarm
    DS3231.clearAlarm(1);

    // Set alarm from now + 1 seconds in the future
    /// @todo TheCavePearl project has a better way without TimeSpan class
    _alarmSetFlag =
        DS3231.setAlarm1(DS3231.now() + TimeSpan(1), DS3231_A1_Second);
    return _alarmSetFlag;
}

bool setDateAndTime(const uint16_t &year, const uint16_t &month,
                    const uint16_t &day, const uint16_t &hour,
                    const uint16_t &minute, const uint16_t &second) {
    // Disable alarm 1 before setting the time
    DS3231.disableAlarm(1);

    // Set the RTC date and time
    DS3231.adjust(DateTime(year, month, day, hour, minute, second));

    if (_alarmSetFlag) {  // If alarm was set before, reset it
        _alarmSetFlag = RTC_1secondAlarm();
    }

    if (!DS3231.lostPower()) {  // If the RTC didn't lose power, we can set the
                                // date time valid
        _dateTimeValid = true;
    }

    // Return true if the RTC didn't lose power
    return !DS3231.lostPower();
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
}

void printTimeToSerial(void) {
    printTimeToSerial(DS3231.now());
}

void printTimeToBuffer(const DateTime &dt, char *buffer) {
    strcpy(buffer, "YYYY-MM-DD hh:mm:ss");
    dt.toString(buffer);
}

void printTimeToBuffer(const uint32_t &unix_time, char *buffer) {
    DateTime _dt(unix_time);
    printTimeToBuffer(_dt, buffer);
}