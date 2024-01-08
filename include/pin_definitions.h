/**
 * @file    pin_definitions.h
 * @author
 * @date    01-2024
 *
 * @brief Arduino Nano Every pin definitions
 *
 * @todo COPYRIGHT NOTICE
 */

#ifndef __PIN_DEFINITIONS_H__
#define __PIN_DEFINITIONS_H__

/** --------------------------------------------------------------------------
 * SD card slot
 * -------------------------------------------------------------------------- */
#define SDCARD_SPI_MOSI 11
#define SDCARD_SPI_MISO 12
#define SDCARD_SPI_SCK 13
#define SDCARD_SPI_CS 10
// Disable LED_BUILTIN use
#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif

/** --------------------------------------------------------------------------
 * DS3231 I2C RTC
 * -------------------------------------------------------------------------- */
#define RTC_I2C_SDA 18
#define RTC_I2C_SCL 19
#define RTC_ALARM_PIN 2

/** --------------------------------------------------------------------------
 * LED outputs and ON/OFF state
 * -------------------------------------------------------------------------- */
#define GREEN_LED 4
#define ERROR_LED 3
#define LED_OFF_STATE HIGH
#define LED_ON_STATE LOW

#endif  // !__PIN_DEFINITIONS_H__