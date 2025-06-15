/**
 * @file    hall_controller.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief Hall sensor controller for reading and processing hall effect sensors
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

#ifndef __HALL_SENSOR_H__
#define __HALL_SENSOR_H__

#include <Arduino.h>

/**
 * @brief Initializes the ADC by configuring its control registers for 10-bit
 * resolution, one-shot mode, 64-sample accumulation, a clock prescaler of 64,
 * and an external reference voltage (AREF). It also disables standby mode,
 * free-running mode, and sample capacitance, and enables the ADC for operation.
 */
void ADC_init(void);

/**
 * @brief Initializes the I/O pins for controlling hall sensors by setting the
 * specified pins as outputs and driving them low to put the respective sensor
 * groups to sleep. It also configure the analog pins required for hall sensor
 * operation
 *
 * @param[in] group0_sleep  Pin number for group 0 hall sensors sleep control
 * @param[in] group1_sleep  Pin number for group 1 hall sensors sleep control
 */
void HALL_initIO(const uint8_t group0_sleep, const uint8_t group1_sleep);

/**
 * @brief Reads hall sensor data by waking up the specified sensor groups. It
 * takes the two control pin to change the sleep state of sensor and reads the
 * analog values from the hall sensors, storing them in the provided array.
 *
 * @param[in] group0_sleep  Pin number for group 0 hall sensors sleep control
 * @param[in] group1_sleep  Pin number for group 1 hall sensors sleep control
 * @param[out] hall         Pointer to a uint16_t array where the hall sensor
 *                          readings will be stored
 */
void HALL_read(const uint8_t group0_sleep, const uint8_t group1_sleep,
               uint16_t *hall);

#endif  // !__HALL_SENSOR_H__