#include "hall_controller.h"

/**
 * @todo function detectPins has PORTx and PINx hardcoded
 * @todo function HALL_setupAnalogPins has PORTD and PINx hardcoded
 */
#define HALL_SLEEP_GROUP0_PORT PORTA
#define HALL_SLEEP_GROUP0_PIN PIN1CTRL
#define HALL_SLEEP_GROUP0_PIN_bm PIN1_bm

#define HALL_SLEEP_GROUP1_PORT PORTE
#define HALL_SLEEP_GROUP1_PIN PIN3CTRL
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
    delayMicroseconds(1000);

    _read(hall);

    // _group_read(0, hall);

    // delayMicroseconds(500);

    // _group_read(1, hall + 3);

    digitalWrite(group0_sleep, LOW);  // put group 0 hall sensor to sleep
    digitalWrite(group1_sleep, LOW);  // put group 1 hall sensor to sleep
}

void HALL_read(const uint8_t group0_sleep, const uint8_t group1_sleep,
               uint16_t* hall) {
    HALL_wakeAndRead(group0_sleep, group1_sleep, hall);
}