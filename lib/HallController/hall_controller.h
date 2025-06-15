/**
 * @file    hall_controller.h
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#ifndef __HALL_SENSOR_H__
#define __HALL_SENSOR_H__

#include <Arduino.h>

/**
 * @brief
 *
 */
void ADC_init(void);

/**
 * @brief Clear the alarm 1 and set new 10 seconds in the future
 *
 * @param[in] group0_sleep
 * @param[in] group1_sleep
 */
void HALL_initIO(const uint8_t group0_sleep, const uint8_t group1_sleep);

/**
 * @brief Clear the alarm 1 and set new 10 seconds in the future
 *
 * @param[in] group0_sleep
 * @param[in] group1_sleep
 * @param[out] hall
 */
void HALL_read(const uint8_t group0_sleep, const uint8_t group1_sleep,
               uint16_t *hall);

#endif  // !__HALL_SENSOR_H__