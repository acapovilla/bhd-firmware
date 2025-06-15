/**
 * @file    cmd_interpreter.h
 * @author  Agustín Capovilla
 * @date    2024-01
 *
 * @brief Command interpreter for serial commands
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

#ifndef __CMD_INTERPRETER_H__
#define __CMD_INTERPRETER_H__

/**
 * @brief Enumeration of command types for the command interpreter.
 *
 * @details List of supported serial commands:
 *
 * Supported commands:
 *
 * - `SETSN XXX`
 *   Sets the device serial number <XXX> (a number between 1 and 999)
 *   Example: `SETSN 123`
 *
 * - `SETDT YYYY-MM-DD HH:MM:SS`
 *   Sets the current date and time
 *   Example: `SETDT 2024-01-31 01:23:45`
 *
 * Notes:
 * - All commands must be sent in plain ASCII via the serial interface.
 * - Responses or acknowledgments may be printed back over serial.
 */
enum COMMANDS {
    Unknown = 0,
    SetSerialNumber,
    GetSerialNumber,
    SetDateAndTime,
    GetDateAndTime
};

/**
 * @brief Reads data from the serial port until a newline character is
 * encountered, processes the input by converting it to uppercase, and parses
 * the first word to identify and execute specific commands such as setting the
 * serial number or date and time. It handles unknown commands gracefully and
 * resets the buffer position after processing.
 *
 * @note This function is designed to be called repeatedly in the main loop to
 * continuously check for new commands from the serial port.
 *
 * @note Based on similar function from https://github.com/millerlp/BivalveBit/
 *
 * @todo Implement the GetSerialNumber and GetDateAndTime commands.
 * @todo Add error handling for invalid commands.
 */
void CMD_readCommand(void);

#endif  // !__CMD_INTERPRETER_H__