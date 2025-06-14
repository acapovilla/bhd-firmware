/**
 * @file    cmd_interpreter.cpp
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#include "cmd_interpreter.h"
#include <Arduino.h>

extern bool setSerialNumber(uint16_t);
extern bool setDateAndTime(const uint16_t&, const uint16_t&, const uint16_t&,
                           const uint16_t&, const uint16_t&, const uint16_t&);

// #define DEBUG

static const uint8_t BUFFER_SIZE{32};  // Buffer size
char _serialBuffer[BUFFER_SIZE];       // Buffer for serial command

COMMANDS _command;

/**
 * @brief
 *
 * @param[in] serialnumber
 *
 * @return True if success
 *
 * @todo Describir
 */
bool _cmd_setSerialNumber(const char* serialnumber) {
    uint16_t sn = strtoul(serialnumber, NULL, 10);
    if (sn > 0 && sn < 1000) {
#ifdef DEBUG
        Serial.print(F("Setting serial number to "));
        Serial.println(sn);
#endif

        bool f = setSerialNumber(sn);
        if (f) {
#ifdef DEBUG
            Serial.println(F("Success"));
#endif
            return true;
        }
    }
    return false;
}

/**
 * @brief
 *
 * @param[in] datetime YYYY-MM-DD HH:MM:SS
 *
 * @todo Describir
 */
void _cmd_setDateAndtime(const char* datetime) {
    uint16_t _y, _m, _d, _h, _mm, _s;
    uint16_t _tokens = sscanf(datetime, "%hu-%hu-%hu %hu:%hu:%hu;", &_y, &_m,
                              &_d, &_h, &_mm, &_s);
    if (_tokens != 6) {  // Check to see if it was parsed
#ifdef DEBUG
        Serial.print(F("Unable to parse date/time\n"));
#endif
    } else {
        setDateAndTime(_y, _m, _d, _h, _mm, _s);  // Adjust the RTC date/time
#ifdef DEBUG
        Serial.println(F("Date has been set."));
#endif
    }  // of if-then-else the date could be parsed
}

void CMD_readCommand(void) {
    static uint8_t _bytesR = 0;   // Buffer position
    while (Serial.available()) {  // Loop while incoming serial data
        _serialBuffer[_bytesR] = Serial.read();  // Get the next byte of data

        // keep on reading newline shows up
        if (_serialBuffer[_bytesR] != '\n' &&
            _bytesR < BUFFER_SIZE)  // or the buffer is full
            ++_bytesR;
        else {
            _serialBuffer[_bytesR] = '\0';  // Add the termination character

            // Convert the buffer to uppercase
            strupr(_serialBuffer);
#ifdef DEBUG
            Serial.write("\nCommand ");
            Serial.write(_serialBuffer);
            Serial.write("\n");
#endif
            // Parse the string for first word
            char* _cmd = strtok(_serialBuffer, " ");
            if (strstr(_cmd, "SETSN"))
                _command = COMMANDS::SetSerialNumber;
            else if (strstr(_cmd, "SETDT"))
                _command = COMMANDS::SetDateAndTime;
            else
                _command = COMMANDS::Unknown;  // Otherwise set to not found

            switch (_command)  // Action depending upon command
            {
                /** -------------------------------------------------------
                 * Set the device serial number
                 * ------------------------------------------------------- */
                case COMMANDS::SetSerialNumber:
                    // Call set serial number function with the next token
                    // delimited by end of buffer
                    _cmd_setSerialNumber(strtok(NULL, '\0'));
                    break;

                /** -------------------------------------------------------
                 * Get the device serial number
                 * ------------------------------------------------------- */
                case COMMANDS::GetSerialNumber:
                    break;

                /** -------------------------------------------------------
                 * Set the device date and time
                 * ------------------------------------------------------- */
                case COMMANDS::SetDateAndTime:
                    _cmd_setDateAndtime(strtok(NULL, '\0'));
                    break;

                /** -------------------------------------------------------
                 * Get the device date and time
                 * ------------------------------------------------------- */
                case COMMANDS::GetDateAndTime:
                    break;

                /** -------------------------------------------------------
                 * Unknown command
                 * ------------------------------------------------------- */
                case COMMANDS::Unknown:
                default:
#ifdef DEBUG
                    Serial.println(F("Unknown cmd"));
#endif
                    break;
            }

            _bytesR = 0;  // reset the counter
        }
    }
}