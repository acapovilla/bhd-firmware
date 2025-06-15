/**
 * @file    rtc_controller.h
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#ifndef __RTC_CONTROLLER_H__
#define __RTC_CONTROLLER_H__

#include <SPI.h>

#include "RTClib.h"

/**
 * @brief Initialize and setup external RTC (DS3231)
 *
 * @return 0 if success, error code otherwise
 *
 * @todo Describir parámetros que se setean
 * @todo Describir código de errores
 */
uint8_t RTC_initExternal(void);

/**
 * @brief Date and time are valid?
 *
 * @return Valid
 */
bool RTC_dateTimeValid(void);

/**
 * @brief
 *
 * @return
 */
DateTime RTC_getNow(void);

/**
 * @brief Clear the alarm 1 and set new 10 seconds in the future
 *
 * @return Success
 */
bool RTC_10secondsAlarm(void);

/**
 * @brief Set external RTC date and Time
 *
 * @param[in] year Either the full year (2000 to 2099) or the offset from year
 * 2000
 * @param[in] month Month number (1 to 12)
 * @param[in] day Day of the month (1 to 31)
 * @param[in] hour Hour (0 to 23)
 * @param[in] minute Minute (0 to 59)
 * @param[in] second Second (0 to 59)
 *
 * @return Success to set date and time
 */
bool setDateAndTime(const uint16_t &year, const uint16_t &month,
                    const uint16_t &day, const uint16_t &hour,
                    const uint16_t &minute, const uint16_t &second);

/**
 * @brief Print formatted date and time to Serial monitor
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @param[in] dt Date & Time to print
 * @deprecated Use printTimeToBuffer and print buffer to Serial port
 */
void printTimeToSerial(const DateTime &dt);

/**
 * @brief Print now date and time to Serial monitor
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @deprecated Use printTimeToBuffer and print buffer to Serial port
 */
void printTimeToSerial(void);

/**
 * @brief Print formatted date and time to char*
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @param[in]  unix_time Date and Time to print in unix format
 * @param[out] buffer Formatted string
 */
void printTimeToBuffer(const uint32_t &unix_time, char *buffer);

/**
 * @brief Print formatted date and time to char*
 * @note Format: "YYYY-MM-DD hh:mm:ss"
 *
 * @param[in]  unix_time Date and Time to print in unix format
 * @param[out] buffer Formatted string
 */
void printTimeToBuffer(const DateTime &dt, char *buffer);

#endif  // !__RTC_CONTROLLER_H__