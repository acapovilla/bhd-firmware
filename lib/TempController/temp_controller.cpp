/**
 * @file    temp_controller.cpp
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#include "temp_controller.h"

#include <DallasTemperature.h>
#include <OneWire.h>

/**
 * @todo One-Wire data pin is hardcoded
 */
// Setup a oneWire instance to communicate with any OneWire devices
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