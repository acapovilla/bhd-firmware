/**
 * @file    hall_controller.cpp
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

#include "hall_controller.h"

// Group 0 hall sensors sleep pin and port
#define HALL_SLEEP_GROUP0_PORT   PORTA
#define HALL_SLEEP_GROUP0_PIN    PIN1CTRL
#define HALL_SLEEP_GROUP0_PIN_bm PIN1_bm

// Group 1 hall sensors sleep pin and port
#define HALL_SLEEP_GROUP1_PORT   PORTE
#define HALL_SLEEP_GROUP1_PIN    PIN3CTRL
#define HALL_SLEEP_GROUP1_PIN_bm PIN3_bm

#define PORTD_BASE_ADDR 0x460

void ADC_init(void) {
    // Not RUN in standby
    // Resolution: 10bits
    // One-shot mode
    ADC0.CTRLA &= ~(ADC_RUNSTBY_bm | ADC_RESSEL_bm | ADC_FREERUN_bm);

    // Sample Accumulation: 64
    ADC0.CTRLB = ADC_SAMPNUM_ACC64_gc;

    // CLK_PER divided by 64 = 125kHz @ 16MHz CPU
    // External reference: AREF (connected to 3.3V output)
    // Sample capacitance = 0 (@todo more testing)
    ADC0.CTRLC = ADC_REFSEL_VREFA_gc | ADC_PRESC_DIV64_gc;

    // Enable ADC
    ADC0.CTRLA |= ADC_ENABLE_bm;
}

/**
 * @brief Configures pins PD0 to PD5 on the microcontroller as analog input pins
 * by clearing their direction bits, disabling their digital input buffers, and
 * disabling internal pull-up resistors.
 *
 * @todo function HALL_setupAnalogPins has PORTD and PINx hardcoded
 */
void HALL_setupAnalogPins(void) {
    /* From PD0 to PD5 pins as input */
    PORTD_DIRCLR = (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm);

    /* Disable digital input buffer and internal pull-up */
    for (uint8_t _ii = 0; _ii < 6; ++_ii) {
        _SFR_MEM8(PORTD_BASE_ADDR + 0x10 + _ii) &=
            ~(PORT_ISC_gm | PORT_PULLUPEN_bm);
        _SFR_MEM8(PORTD_BASE_ADDR + 0x10 + _ii) |= PORT_ISC_INPUT_DISABLE_gc;
    }
}

void HALL_initIO(const uint8_t group0_sleep, const uint8_t group1_sleep) {
    pinMode(group0_sleep, OUTPUT);
    digitalWrite(group0_sleep, LOW);  // put group 0 hall sensor to sleep
    pinMode(group1_sleep, OUTPUT);
    digitalWrite(group1_sleep, LOW);  // put group 1 hall sensor to sleep

    HALL_setupAnalogPins();
}

/**
 * @brief Reads a group of three analog inputs (AIN[x] to A[x+2]) based on the
 * specified group index, performs ADC conversions for each input, and stores
 * the 12-bit resolution results in the provided hall array. It handles ADC
 * channel configuration, conversion initiation, completion waiting, interrupt
 * flag clearing, and result retrieval for each input.
 *
 * @param[in] group  The group number (0 or 1) to read from
 * @param[out] hall  Pointer to a uint16_t array where the hall sensor readings
 *                   will be stored
 */
void _group_read(uint8_t group, uint16_t* hall) {
    // Read a group of three analog inputs (AIN[x] to A[x+2])
    uint8_t _ii = 3 * group, _fin = 3 * (group + 1);
    for (; _ii < _fin; ++_ii) {
        // Configure channel
        ADC0.MUXPOS = (_ii & ADC_MUXPOS_gm);

        // Start ADC conversion
        ADC0.COMMAND = ADC_STCONV_bm;

        // Wait until ADC conversion done
        while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {
            ;
        }

        // Clear the interrupt flag by writing 1
        ADC0.INTFLAGS = ADC_RESRDY_bm;

        hall[_ii - (3 * group)] = (ADC0.RES >> 4);  // 12-bit "resolution"
    }
}

/**
 * @brief Reads analog-to-digital conversion (ADC) results from six channels,
 * processes them in a specific order defined by `{3, 2, 1, 0, 4, 5}`, and
 * stores the 12-bit resolution values into the hall array. It configures the
 * ADC channel, starts the conversion, waits for completion, clears the
 * interrupt flag, and retrieves the result for each channel.
 *
 * @param[out] hall Pointer to a uint16_t array where the hall sensor readings
 *                  will be stored
 */
void _read(uint16_t* hall) {
    uint8_t _order[6] = {3, 2, 1, 0, 4, 5};

    for (uint8_t _ii = 0; _ii < 6; ++_ii) {
        // Configure channel
        ADC0.MUXPOS = (_ii & ADC_MUXPOS_gm);

        // Start ADC conversion
        ADC0.COMMAND = ADC_STCONV_bm;

        // Wait until ADC conversion done
        while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)) {
            ;
        }

        // Clear the interrupt flag by writing 1
        ADC0.INTFLAGS = ADC_RESRDY_bm;

        hall[_order[_ii]] = (ADC0.RES >> 4);  // 12-bit "resolution"
    }
}

/**
 * @brief Activates two groups of hall sensors by setting their corresponding
 * pins to high, waits for stabilization, reads their values into the provided
 * hall array using the _read function, and then puts the sensors back to sleep
 * by setting their pins to low.
 *
 * @param[in] group0_sleep  Pin number for group 0 hall sensors sleep control
 * @param[in] group1_sleep  Pin number for group 1 hall sensors sleep control
 * @param[out] hall         Pointer to a uint16_t array where the hall sensor
 *                          readings will be stored
 */
void HALL_wakeAndRead(const uint8_t group0_sleep, const uint8_t group1_sleep,
                      uint16_t* hall) {
    // Activar la ejecución en modo standby
    // ADC0.CTRLA |= ADC_RUNSTBY_bm;

    // Activar las interrupciones del ADC
    // ADC0.INTCTRL = ADC_RESRDY_bm;

    // Turn on group 0 hall sensors
    pinMode(group0_sleep, OUTPUT);
    digitalWrite(group0_sleep, HIGH);
    // Turn on group 1 hall sensors
    pinMode(group1_sleep, OUTPUT);
    digitalWrite(group1_sleep, HIGH);

    // Wait for the sensors to stabilize
    delayMicroseconds(1000);

    // Read all hall sensors
    _read(hall);

    digitalWrite(group0_sleep, LOW);  // put group 0 hall sensor to sleep
    digitalWrite(group1_sleep, LOW);  // put group 1 hall sensor to sleep
}

void HALL_read(const uint8_t group0_sleep, const uint8_t group1_sleep,
               uint16_t* hall) {
    HALL_wakeAndRead(group0_sleep, group1_sleep, hall);
}