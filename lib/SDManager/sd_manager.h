/**
 * @file    sd_manager.h
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
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
 * @brief Initialize and setup SD card and FAT filesystem
 *
 * @return Success
 */
bool SDCard_init(void);

/**
 * @brief Initialize and setup SD card and FAT filesystem
 *
 * @param[in] year
 * @param[in] month
 * @param[in] day
 * @param[in] hour
 * @param[in] minute
 * @param[in] second
 * @param[in] serial_number
 */
void SDCard_initFileName(const uint16_t year, const uint8_t month,
                         const uint8_t day, const uint8_t hour,
                         const uint8_t minute, const uint8_t second,
                         const uint16_t serial_number);

/**
 * @brief Initialize and setup SD card and FAT filesystem
 *
 * @param[in] unix_time
 * @param[in] timestamp
 * @param[in] hall
 * @param[in] tempC
 *
 * @return Success
 */
bool SDCard_writeFile(const uint32_t unix_time, const char *timestamp,
                      const uint16_t hall[6], const float tempC);

#endif  // !__SD_MANAGER_H__