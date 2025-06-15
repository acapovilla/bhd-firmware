/**
 * @file    serial_number.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   Serial number management for the data logger system. It provides
 * functions to read and write a serial number to EEPROM. The serial number
 * is expected to be in the range of 001 to 999. The functions ensure that
 * the serial number is valid and handle EEPROM operations safely.
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

#ifndef __SERIAL_NUMBER_H__
#define __SERIAL_NUMBER_H__

#include <Arduino.h>
#include <EEPROM.h>

/**
 * @brief Retrieves a serial number from EEPROM, validates it using a checksum
 * and stores it in the provided reference parameter `sn`
 *
 * @param[out] sn  If it's valid, the serial number value in range 001 to 999
 *
 * @return True if the serial number was successfully read and is valid
 */
bool getSerialNumber(uint16_t &sn);

/**
 * @brief Writes a serial number (`sn`) for the device validating the input,
 * calculating a checksum, storing the serial number in EEPROM and verifying it
 *
 * @param[in] sn The serial number value in range 001 to 999
 *
 * @return True if the serial number was successfully written and is valid
 */
bool setSerialNumber(const uint16_t sn);

#endif  // !__SERIAL_NUMBER_H__