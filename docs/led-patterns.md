# LED Status Patterns

This document describes the visual feedback provided by the datalogger’s LEDs during startup and operation. These LED signals are designed to help identify hardware issues or verify correct operation without needing a serial monitor.

---

## 🟥 Error LED Patterns

During the `setup()` phase, the red error LED provides diagnostic feedback when a module fails to initialize properly.

| Pattern                       | Meaning                                                  |
| ----------------------------- | -------------------------------------------------------- |
| 1 short flash per second      | RTC module initialization failed                         |
| 2 short flashes per second    | SD card not detected or initialization failed            |
| 3 short flashes per second    | Temperature sensor not detected or initialization failed |
| 1 Hz blinking (regular flash) | RTC time not set or invalid date/time                    |
| **OFF**                       | **No errors detected – setup completed successfully**    |

> ⚠️ Multiple errors may result in only one of the patterns being shown, depending on code execution order.

---

## 🟢 Green LED Behavior

The green LED provides feedback during the normal logging process.

| Condition        | LED State                                      |
| ---------------- | ---------------------------------------------- |
| Normal operation | Blinks briefly during each recording cycle     |
| LED stays ON     | ⚠️ Indicates possible hang or SD write failure |

---

## Troubleshooting Notes

- If the error LED remains ON or blinks continuously, connect the device to a serial monitor for debugging output.
- Ensure that all peripherals (SD, RTC, sensors) are connected correctly and powered.
