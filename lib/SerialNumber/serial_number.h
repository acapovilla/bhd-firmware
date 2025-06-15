/**
 * @file    serial_number.h
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#ifndef __SERIAL_NUMBER_H__
#define __SERIAL_NUMBER_H__

#include <Arduino.h>
#include <EEPROM.h>

/**
 * @brief Read serial number from EEPROM
 *
 * @param[out] sn  If it's valid, the serial number value in range 001 to 999
 *
 * @return Serial number valid
 */
bool getSerialNumber(uint16_t &sn);

/**
 * @brief Write serial number to EEPROM
 *
 * @param[in] sn The serial number value in range 001 to 999
 *
 * @return True if success
 */
bool setSerialNumber(const uint16_t sn);

#endif  // !__SERIAL_NUMBER_H__