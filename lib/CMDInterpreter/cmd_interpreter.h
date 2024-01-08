/**
 * @file    cmd_interpreter.h
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#ifndef __CMD_INTERPRETER_H__
#define __CMD_INTERPRETER_H__

/**
 * The current list of commands understood are as follows:
 * **
 * . SETSN xxx                  - Set serial number device
 * . SETDT YYYY-MM-DD HH:MM:SS  - Set date and time
 * **
 * @todo Get serial number
 * @todo Get date and time
 */
enum COMMANDS {
    Unknown = 0,
    SetSerialNumber,
    GetSerialNumber,
    SetDateAndTime,
    GetDateAndTime
};

/**
 * Check the serial port, read until `\n`, parse and check the command
 * Based on https://github.com/millerlp/BivalveBit/
 */
void CMD_readCommand(void);

#endif  // !CMD_INTERPRETER