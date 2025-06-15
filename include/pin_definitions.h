/**
 * @file    pin_definitions.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   Pin definitions for the data logger system.
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

#ifndef __PIN_DEFINITIONS_H__
#define __PIN_DEFINITIONS_H__

/** --------------------------------------------------------------------------
 * SD card slot
 * -------------------------------------------------------------------------- */

#define SDCARD_SPI_MOSI 11
#define SDCARD_SPI_MISO 12
#define SDCARD_SPI_SCK  13
#define SDCARD_SPI_CS   10
// Disable LED_BUILTIN use
#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif

/** --------------------------------------------------------------------------
 * DS3231 I2C RTC
 * -------------------------------------------------------------------------- */

#define RTC_I2C_SDA   18
#define RTC_I2C_SCL   19
#define RTC_ALARM_PIN 2

/** --------------------------------------------------------------------------
 * DS18B20 OneWire Temperature sensor
 * -------------------------------------------------------------------------- */

#define TEMP_ONEWIRE_DATA 9

/** --------------------------------------------------------------------------
 * Hall sensors analog inputs and sleep control outputs
 * -------------------------------------------------------------------------- */

#define HALL_GROUP0_A0    A0  // 14 - AIN[3]
#define HALL_GROUP0_A1    A1  // 15 - AIN[2]
#define HALL_GROUP0_A2    A2  // 16 - AIN[1]
#define HALL_GROUP1_A3    A3  // 17 - AIN[0]
#define HALL_GROUP1_A4    A6  // 20 - AIN[4]
#define HALL_GROUP1_A5    A7  // 21 - AIN[5]
#define HALL_SLEEP_GROUP0 8   // PE3
#define HALL_SLEEP_GROUP1 7   // PA1
// #define ANALOG_EXT_AREF

/** --------------------------------------------------------------------------
 * LED outputs and ON/OFF state
 * -------------------------------------------------------------------------- */

#define GREEN_LED     4
#define ERROR_LED     3
#define LED_OFF_STATE HIGH
#define LED_ON_STATE  LOW

#endif  // !__PIN_DEFINITIONS_H__
