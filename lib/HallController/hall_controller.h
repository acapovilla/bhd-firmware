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

void ADC_init(void);

void HALL_initIO(const uint8_t group0_sleep, const uint8_t group1_sleep);

void HALL_read(uint16_t *hall);

#endif  // !__HALL_SENSOR_H__