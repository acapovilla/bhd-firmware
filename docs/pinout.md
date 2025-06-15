# GPIO Pin Mapping

This table lists the GPIO assignments used in the firmware, including references for Arduino Nano Every pin numbers, ATmega4809 internal names (when applicable), and connected peripherals.

| Symbolic Name       | Arduino Pin | ATmega4809 Pin | Description              | Connected Device    |
| ------------------- | ----------- | -------------- | ------------------------ | ------------------- |
| `SDCARD_SPI_MOSI`   | 11          | PE0            | SPI MOSI for SD card     | SD Card             |
| `SDCARD_SPI_MISO`   | 12          | PE1            | SPI MISO for SD card     | SD Card             |
| `SDCARD_SPI_SCK`    | 13          | PE2            | SPI Clock                | SD Card             |
| `SDCARD_SPI_CS`     | 10          | PB1            | SPI Chip Select          | SD Card             |
| `SDCARD_DETECT`     | 5           | PB2            | SPI Clock                | SD Card Socket      |
| `RTC_I2C_SDA`       | A4 (18)     | PA2            | I2C Data line            | DS3231 RTC          |
| `RTC_I2C_SCL`       | A5 (19)     | PA3            | I2C Clock line           | DS3231 RTC          |
| `RTC_ALARM_PIN`     | 2           | PA0            | Alarm interrupt          | DS3231 RTC          |
| `TEMP_ONEWIRE_DATA` | 9           | PB0            | OneWire bus              | DS18B20 Temp Sensor |
| `HALL_GROUP0_A0`    | A0 (14)     | PD3            | Analog input 0 (group 0) | Hall Sensor 1       |
| `HALL_GROUP0_A1`    | A1 (15)     | PD2            | Analog input 1 (group 0) | Hall Sensor 2       |
| `HALL_GROUP0_A2`    | A2 (16)     | PD1            | Analog input 2 (group 0) | Hall Sensor 3       |
| `HALL_GROUP1_A3`    | A3 (17)     | PD0            | Analog input 3 (group 1) | Hall Sensor 4       |
| `HALL_GROUP1_A4`    | A4 (20)     | PD4            | Analog input 4 (group 1) | Hall Sensor 5       |
| `HALL_GROUP1_A5`    | A5 (21)     | PD5            | Analog input 5 (group 1) | Hall Sensor 6       |
| `HALL_SLEEP_GROUP0` | 8           | PE3            | Sleep control (group 0)  | Hall Sensor (1,2,3) |
| `HALL_SLEEP_GROUP1` | 7           | PA1            | Sleep control (group 1)  | Hall Sensor (4,5,6) |
| `ERROR_LED`         | 3           | PF5            | Error indicator          | Onboard LED         |
| `GREEN_LED`         | 4           | PC6            | Logging activity         | Onboard LED         |
