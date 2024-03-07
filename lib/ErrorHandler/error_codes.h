/**
 * @file    error_codes.h
 * @author
 * @date    01-2024
 *
 * @brief List of errors
 *
 * @todo COPYRIGHT NOTICE
 */

#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

/** --------------------------------------------------------------------------
 * Serial number
 * -------------------------------------------------------------------------- */
#define ERROR_SN_NOTVALID_code 0x001
#define ERROR_SN_NOTVALID_str "(E001) Failed to get serial number"
#define ERROR_SN_NOTVALID_short "E001"

/** --------------------------------------------------------------------------
 * SD card & file system
 * -------------------------------------------------------------------------- */
#define ERROR_SDCARD_INITFAIL_code 0x014
#define ERROR_SDCARD_INITFAIL_str "(E020) SD initialization failed"
#define ERROR_SDCARD_INITFAIL_short "E020"

/** --------------------------------------------------------------------------
 * RTC and date/time
 * -------------------------------------------------------------------------- */
#define ERROR_RTCEXT_INITFAIL_code 0x00A
#define ERROR_RTCEXT_INITFAIL_str "(E010) Couldn't find RTC"
#define ERROR_RTCEXT_INITFAIL_short "E010"

#define ERROR_RTCEXT_LOSTPWR_code 0x00B
#define ERROR_RTCEXT_LOSTPWR_str "(E011) RTC lost power, set the time"
#define ERROR_RTCEXT_LOSTPWR_short "E011"

#define ERROR_RTCEXT_WRONGDT_code 0x00D
#define ERROR_RTCEXT_WRONGDT_str "(E013) Wrong time setting"
#define ERROR_RTCEXT_WRONGDT_short "E013"

/** --------------------------------------------------------------------------
 * Temperature sensor
 * -------------------------------------------------------------------------- */
#define ERROR_TEMPEXT_INITFAIL_code 0x01E
#define ERROR_TEMPEXT_INITFAIL_str "(E030) Temp sensor init failed"
#define ERROR_TEMPEXT_INITFAIL_short "E030"

/** --------------------------------------------------------------------------
 * Hall sensor
 * -------------------------------------------------------------------------- */

#endif  // !__ERROR_CODES_H__