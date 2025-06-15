/**
 * @file    error_codes.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   Error codes for the project
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

#ifndef __ERROR_CODES_H__
#define __ERROR_CODES_H__

/** --------------------------------------------------------------------------
 * Serial number
 * -------------------------------------------------------------------------- */
#define ERROR_SN_NOTVALID_code  0x001
#define ERROR_SN_NOTVALID_str   "(E001) Failed to get serial number"
#define ERROR_SN_NOTVALID_short "E001"

/** --------------------------------------------------------------------------
 * SD card & file system
 * -------------------------------------------------------------------------- */
#define ERROR_SDCARD_INITFAIL_code  0x014
#define ERROR_SDCARD_INITFAIL_str   "(E020) SD initialization failed"
#define ERROR_SDCARD_INITFAIL_short "E020"

/** --------------------------------------------------------------------------
 * RTC and date/time
 * -------------------------------------------------------------------------- */
#define ERROR_RTCEXT_INITFAIL_code  0x00A
#define ERROR_RTCEXT_INITFAIL_str   "(E010) Couldn't find RTC"
#define ERROR_RTCEXT_INITFAIL_short "E010"

#define ERROR_RTCEXT_LOSTPWR_code  0x00B
#define ERROR_RTCEXT_LOSTPWR_str   "(E011) RTC lost power. Set the time"
#define ERROR_RTCEXT_LOSTPWR_short "E011"

#define ERROR_RTCEXT_WRONGDT_code  0x00D
#define ERROR_RTCEXT_WRONGDT_str   "(E013) Wrong time setting"
#define ERROR_RTCEXT_WRONGDT_short "E013"

/** --------------------------------------------------------------------------
 * Temperature sensor
 * -------------------------------------------------------------------------- */
#define ERROR_TEMPEXT_INITFAIL_code  0x01E
#define ERROR_TEMPEXT_INITFAIL_str   "(E030) Temp sensor init failed"
#define ERROR_TEMPEXT_INITFAIL_short "E030"

/** --------------------------------------------------------------------------
 * Hall sensor
 * -------------------------------------------------------------------------- */

#endif  // !__ERROR_CODES_H__