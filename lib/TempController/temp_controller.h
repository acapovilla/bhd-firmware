/**
 * @file    temp_controller.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief   Temperature controller using Miles Burton's Maxim temperature
 * sensor library
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

#ifndef __TEMP_CONTROLLER_H__
#define __TEMP_CONTROLLER_H__

/**
 * @brief Initialize and setup temperature sensor (DS18B20)
 *
 * @return 0 if success, error code otherwise
 * @todo Describir código de errores
 */
bool TEMP_init(void);

/**
 * @brief Retrieves the temperature in Celsius from a DS18B20 sensor accessing
 * the temperature value at index 0
 *
 * @returns Temperature in Celsius as a float
 */
float TEMP_read(void);

#endif  // !__TEMP_CONTROLLER_H__