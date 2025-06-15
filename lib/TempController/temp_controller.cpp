/**
 * @file    temp_controller.cpp
 * @author  Agustín Capovilla
 * @date    2024-01
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

#include "temp_controller.h"

#include <DallasTemperature.h>
#include <OneWire.h>

// Setup a oneWire instance to communicate with any OneWire devices
/// @todo One-Wire data pin is hardcoded
OneWire oneWire(9);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature ds18b20(&oneWire);

bool TEMP_init(void) {
    // Start up the library
    ds18b20.begin();

    if (ds18b20.getDS18Count() == 1) {
        // Set resolution to 12 bits
        ds18b20.setResolution(12);

        return 0;
    }

    return 1;
}

float TEMP_read(void) {
    ds18b20.requestTemperatures();

    return ds18b20.getTempCByIndex(0);
}