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

#endif  // !__SD_MANAGER_H__