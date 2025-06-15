# Status and error messages

## Error Codes

| Name                     | Code value | Short id | Message                             |
| ------------------------ | ---------- | -------- | ----------------------------------- |
| `ERROR_SN_NOTVALID`      | `0x001`    | E001     | (E001) Failed to get serial number  |
| `ERROR_SDCARD_INITFAIL`  | `0x014`    | E020     | (E020) SD initialization failed     |
| `ERROR_RTCEXT_INITFAIL`  | `0x00A`    | E010     | (E010) Couldn't find RTC            |
| `ERROR_RTCEXT_LOSTPWR`   | `0x00B`    | E011     | (E011) RTC lost power. Set the time |
| `ERROR_RTCEXT_WRONGDT`   | `0x00D`    | E013     | (E013) Wrong time setting           |
| `ERROR_TEMPEXT_INITFAIL` | `0x01E`    | E030     | (E030) Temp sensor init failed      |

## System and Info Messages

| Name                 | Code value | Short id | Message                   |
| -------------------- | ---------- | -------- | ------------------------- |
| `MSG_SYS_READY_code` | `0x064`    | M100     | (M100) Ready to send data |

# Serial Commands

These commands are sent via the serial interface for device configuration.

### `SETSN` – Set Serial Number

- **Usage:** `SETSN XXX`
- **Example:** `SETSN 123`
- **Description:** Assigns a unique serial number to the device. The value is a string of digits.

---

### `SETDT` – Set Date and Time

- **Usage:** `SETDT YYYY-MM-DD HH:MM:SS`
- **Example:** `SETDT 2024-01-30 01:23:45`
- **Description:** Sets the internal real-time clock (RTC) to the specified date and time. Format must be ISO 8601 compliant (24-hour clock).

---

### Notes

- Commands must be sent over a plain ASCII serial connection (e.g., via serial terminal).
- A response will confirm success or return an error if the command is malformed or rejected.
- These commands are typically used during device setup or calibration.
