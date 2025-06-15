## Ideas

- [ ] Implement a serial protocol based on [Serial Line Internet Protocol (SLIP)](https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol)
- [ ] Evaluate the use of internal RTC for power saving and higher measurement frequency
- [ ] Improve DS3231 power consumption and implement some tips and tricks from [TheCavePearlProject](https://thecavepearlproject.org/tag/ds3231/)
- [ ] Evaluate method to measure the Vin voltage (used by AREF) ([link](https://forum.arduino.cc/t/can-arduino-measure-its-own-vin/15694))
- [ ] Improve safety and realiability with a watchdog timer ([AVR132](files/doc2551.pdf))
- [ ] Analyse ADC techniques and suggestions from [AN2573](files/AN2573-ADC-Basics-with-tinyAVR-and-megaAVR-00002573C.pdf), [AN2551](files/AN2551-Noise-Countermeasures-for-ADC-Applications-00002551C.pdf) and [TB3213](files/TB3213-Getting-Started-with-RTC-DS90003213.pdf)

## Other useful links

- Microchip AVR examples for Atmega4809: [microchip-pic-avr-examples](https://github.com/orgs/microchip-pic-avr-examples/repositories?language=&q=atmega4809&sort=&type=all)
- [SdFat Issue 428](https://github.com/greiman/SdFat/issues/428) - "Program compactibility SDfat 1.1.4 vs 2.2.2."
- [EEVblog #140 - Battery Capacity Tutorial](https://www.youtube.com/watch?v=R8hTQXqURB4)
- TMP102 Temperature sensor report from [TheCavePearlProject](https://thecavepearlproject.org/2015/08/26/field-report-2015-08-10-diagnosing-a-tmp102-temp-sensor-failure/)
