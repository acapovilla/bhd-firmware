/**
 * @file    rtc_controller.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   Controller for interfacing with a DS3231 real-time clock (RTC)
 * module.
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

#ifndef __RTC_CONTROLLER_H__
#define __RTC_CONTROLLER_H__

#include <SPI.h>

#include "RTClib.h"

/**
 * @brief Initializes communication with a DS3231 real-time clock (RTC) module,
 * verifies its power state, checks the validity of its current time, and
 * configures various settings such as disabling alarms and the 32K pin
 *
 * @retval 0: Success
 * @retval 10: Unable to find DS3231
 * @retval 11: Clock isn't set (lost power)
 * @retval 13: Invalid time value (year < 2023 or year > 2027)
 */
uint8_t RTC_initExternal(void);

/**
 * @brief Returns a boolean value indicating the validity of the current date
 * and time
 *
 * @return True if the date and time are valid, false otherwise
 */
bool RTC_dateTimeValid(void);

/**
 * @brief Returns the current date and time as a DateTime object
 *
 * @return Current date and time as a DateTime object
 */
DateTime RTC_getNow(void);

/**
 * @brief Configures a 1-second alarm on the RTC module disabling the square
 * wave output, clearing any existing alarms, and setting a new alarm to trigger
 * 1 seconds from the current time
 *
 * @return True if the alarm was successfully set
 */
bool RTC_1secondAlarm(void);

/**
 * @brief Set the date and time on the RTC module using the provided year,
 * month, day, hour, minute, and second values. It disables the first
 * alarm, adjusts the RTC's time, updates an alarm flag if necessary, checks for
 * power loss, and returns whether the RTC has not lost power.
 *
 * @param[in] year      Either the full year (2000 to 2099) or the offset from
 * year 2000
 * @param[in] month     Month number (1 to 12)
 * @param[in] day       Day of the month (1 to 31)
 * @param[in] hour      Hour (0 to 23)
 * @param[in] minute    Minute (0 to 59)
 * @param[in] second    Second (0 to 59)
 *
 * @return False if the RTC has lost power, true otherwise
 */
bool setDateAndTime(const uint16_t &year, const uint16_t &month,
                    const uint16_t &day, const uint16_t &hour,
                    const uint16_t &minute, const uint16_t &second);

/**
 * @brief Print DateTime object to Serial monitor
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @param[in] dt Date & Time to print
 * @deprecated Use printTimeToBuffer and print buffer to Serial port
 */
void printTimeToSerial(const DateTime &dt);

/**
 * @brief Print the current time retrieved from the RTC module to the serial
 * interface
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @deprecated Use printTimeToBuffer and print buffer to Serial port
 */
void printTimeToSerial(void);

/**
 * @brief Print formatted Unix timestamp to char*
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @param[in]  unix_time    Date and Time to print in unix format
 * @param[out] buffer       Formatted string
 */
void printTimeToBuffer(const uint32_t &unix_time, char *buffer);

/**
 * @brief Print DateTime object to char*
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @param[in]  unix_time    Date and Time to print in unix format
 * @param[out] buffer       Formatted string
 */
void printTimeToBuffer(const DateTime &dt, char *buffer);

#endif  // !__RTC_CONTROLLER_H__