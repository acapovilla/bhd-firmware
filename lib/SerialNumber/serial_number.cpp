/**
 * @file    serial_number.cpp
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @note Using EEPROM.get() and put() due to a write error in early start of
 * cpu
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

#include "serial_number.h"

// #define DEBUG

/**
 * Placeholder for serial number
 * (S) - Start char
 * (x x x) - Three digit serial number
 * ($) - Checksum: mod(10) of sum of the three digit
 */
static char _serialNumber[5]{'S', 0, 0, 0, '$'};

// Flag when retriving serial number
bool _SNValid = false;

bool getSerialNumber(uint16_t &sn) {
    _SNValid = false;

    EEPROM.get(0, _serialNumber);
    if (_serialNumber[0] == 'S') {
#ifdef DEBUG
        Serial.println("Reading serial number");
#endif
#ifdef DEBUG
        Serial.print("Digitos: ");
        Serial.print(_serialNumber[1], DEC);
        Serial.print(" ");
        Serial.print(_serialNumber[2], DEC);
        Serial.print(" ");
        Serial.println(_serialNumber[3], DEC);
        Serial.print("Checksum: ");
        Serial.println(_serialNumber[4], DEC);
#endif
        _SNValid = ((_serialNumber[1] + _serialNumber[2] + _serialNumber[3]) %
                    10) == _serialNumber[4];
#ifdef DEBUG
        Serial.print("Valido: ");
        Serial.println(_SNValid);
#endif
        sn = uint16_t(_serialNumber[1]) * 100;
        sn += _serialNumber[2] * 10 + _serialNumber[3];
#ifdef DEBUG
        Serial.print("Numero serie: ");
        Serial.println(sn);
#endif
    }

    return _SNValid;
}

bool setSerialNumber(const uint16_t sn) {
    _SNValid = false;
#ifdef DEBUG
    Serial.print("Numero serie: ");
    Serial.println(sn);
#endif
    if (sn == 0 || sn > 999) return;

    _serialNumber[0] = 'S';

    _serialNumber[1] = (sn / 100) % 10;
    _serialNumber[2] = (sn / 10) % 10;
    _serialNumber[3] = sn % 10;
    _serialNumber[4] =
        ((_serialNumber[1] + _serialNumber[2] + _serialNumber[3]) % 10);
    EEPROM.put(0, _serialNumber);
#ifdef DEBUG
    Serial.print("Digitos: ");
    Serial.print(_serialNumber[1], DEC);
    Serial.print(" ");
    Serial.print(_serialNumber[2], DEC);
    Serial.print(" ");
    Serial.println(_serialNumber[3], DEC);

    Serial.print("Checksum: ");
    Serial.println(_serialNumber[4], DEC);
#endif
    uint16_t _check = 0;
    _SNValid = getSerialNumber(_check);
    _SNValid &= (sn == _check);
#ifdef DEBUG
    Serial.print("Valido: ");
    Serial.println(_SNValid);
#endif
    return _SNValid;
}
