/**
 * @file    sd_manager.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   SD card manager for the data logger based on Greiman's SdFat
 * library. Functions to initialize the SD card, create a log file with a unique
 * name, and write data to the log file.
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

#ifndef __SD_MANAGER_H__
#define __SD_MANAGER_H__

// Configuración recomendada para minimizar el uso de flash
#define USE_FAT_FILE_FLAG_CONTIGUOUS 0
// #define ENABLE_DEDICATED_SPI 0
// #define USE_LONG_FILE_NAMES 0
#define SDFAT_FILE_TYPE 1
// #define CHECK_FLASH_PROGRAMMING 0  // May cause SD to sleep at high current.

#include "SdFat.h"

/**
 * @brief Initialize and sets up an SD card and FAT filesystem. It verifies the
 * SD card's connection, configuration, and metadata, handles errors, and
 * returns true on success
 *
 * @return True if the SD card is initialized successfully, false otherwise
 */
bool SDCard_init(void);

/**
 * @brief Initialize a filename for an SD card log file based on the provided
 * date, time, and serial number (append a counter if necessary for uniqueness).
 * It creates the file, writes a header line, sets timestamps, and closes the
 * file to finalize its creation.
 *
 * @note  Based on similar function in
 * https://github.com/millerlp/BivalveBit_lib
 *
 * @param[in] year              Full year (2000 to 2099)
 * @param[in] month             Month number (1 to 12)
 * @param[in] day               Day of the month (1 to 31)
 * @param[in] hour              Hour (0 to 23)
 * @param[in] minute            Minute (0 to 59)
 * @param[in] second            Second (0 to 59)
 * @param[in] serial_number     Serial number in format (0-999)
 */
void SDCard_initFileName(const uint16_t year, const uint8_t month,
                         const uint8_t day, const uint8_t hour,
                         const uint8_t minute, const uint8_t second,
                         const uint16_t serial_number);

/**
 * @brief Function that writes data to the log file with: POSIX timestamp, a
 * human-readable timestamp, all of six hall sensor values, and the temperature
 * value in Celsius
 *
 * @param[in] unix_time         The POSIX timestamp
 * @param[in] timestamp         A human-readable timestamp in the format
 * @param[in] hall              An array of six hall sensor values
 * @param[in] tempC             The temperature value in Celsius
 *
 * @return True if the operation fails or false if successful
 */
bool SDCard_writeFile(const uint32_t unix_time, const char *timestamp,
                      const uint16_t hall[6], const float tempC);

#endif  // !__SD_MANAGER_H__