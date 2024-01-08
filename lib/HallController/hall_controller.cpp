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

void HALL_read(uint16_t *hall) {}