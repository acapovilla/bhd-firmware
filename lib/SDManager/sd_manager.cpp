/**
 * @file    sd_manager.cpp
 * @author
 * @date    01-2024
 *
 * @brief
 *
 * @todo COPYRIGHT NOTICE
 */

#include "sd_manager.h"

// #define DEBUG

/*******************************************************
 * SD card objects
 *******************************************************/
// SPI pins for SD card
const byte SDCard_SS = 10;  // define the Chip Select pin for SD card
#define SD_CONFIG SdSpiConfig(SDCard_SS, DEDICATED_SPI, SPI_HALF_SPEED)
SdFat sd;  // sd card object
bool SDfailFlag = false;
SdFile logfile;  // for sd card, this is the file object to be written to
char filename[] = "YYYYMMDD_HHMM_00_SN000.csv";

void errorPrint() {
    if (sd.sdErrorCode()) {
        Serial.print(F("SD errorCode: "));

        printSdErrorSymbol(&Serial, sd.sdErrorCode());

        Serial.print(F(" = "));
        Serial.println(int(sd.sdErrorCode()), DEC);
        Serial.print(F("SD errorData = "));
        Serial.println(int(sd.sdErrorData()), DEC);
    }
}

bool SDCard_init(void) {
    if (!sd.begin(SD_CONFIG)) {
#ifdef DEBUG
        Serial.print(
            F("\nSD initialization failed.\n"
              "Do not reformat the card!\n"
              "Is the card correctly inserted?\n"
              "Is there a wiring/soldering problem?\n"));
        if (isSpi(SD_CONFIG)) {
            Serial.print(
                F("Is SD_CS_PIN set to the correct value?\n"
                  "Does another SPI device need to be disabled?\n"));
        }
#endif
        SDfailFlag = true;
#ifdef DEBUG
        errorPrint();
#endif
        // while (1) delay(10);
        return false;
    }

    cid_t cid;
    csd_t csd;
    uint32_t ocr;

    if (!sd.card()->readCID(&cid) || !sd.card()->readCSD(&csd) ||
        !sd.card()->readOCR(&ocr) /* || !sd.card()->readSCR(&scr) */) {
#ifdef DEBUG
        Serial.print(F("readInfo failed\n"));
        errorPrint();
#endif
        // while (1) delay(10);
        return false;
    }

    // printCardType();

    return true;
}

/**
 *  Copy of https://github.com/millerlp/BivalveBit_lib
 */
void SDCard_initFileName(const uint16_t year, const uint8_t month,
                         const uint8_t day, const uint8_t hour,
                         const uint8_t minute, const uint8_t second,
                         const uint16_t serial_number) {
    char buf[5];
    // integer to ascii function itoa(), supplied with numeric year value,
    // a buffer to hold output, and the base for the conversion (base 10 here)
    itoa(year, buf, 10);
    // copy the ascii year into the filename array
    for (byte i = 0; i < 4; i++) {
        filename[i] = buf[i];
    }
    // Insert the month value
    if (month < 10) {
        filename[4] = '0';
        filename[5] = month + '0';
    } else if (month >= 10) {
        filename[4] = (month / 10) + '0';
        filename[5] = (month % 10) + '0';
    }
    // Insert the day value
    if (day < 10) {
        filename[6] = '0';
        filename[7] = day + '0';
    } else if (day >= 10) {
        filename[6] = (day / 10) + '0';
        filename[7] = (day % 10) + '0';
    }
    // Insert an underscore between date and time
    filename[8] = '_';
    // Insert the hour
    if (hour < 10) {
        filename[9] = '0';
        filename[10] = hour + '0';
    } else if (hour >= 10) {
        filename[9] = (hour / 10) + '0';
        filename[10] = (hour % 10) + '0';
    }
    // Insert minutes
    if (minute < 10) {
        filename[11] = '0';
        filename[12] = minute + '0';
    } else if (minute >= 10) {
        filename[11] = (minute / 10) + '0';
        filename[12] = (minute % 10) + '0';
    }
    // Insert another underscore after time
    filename[13] = '_';
    // If there is a valid serialnumber SNxxx, insert it into
    // the file name in positions 17-21.
    filename[19] = '0' + (serial_number / 100) % 10;
    filename[20] = '0' + (serial_number / 10) % 10;
    filename[21] = '0' + serial_number % 10;
    // Next change the counter on the end of the filename
    // (digits 14+15) to increment count for files generated on
    // the same day. This shouldn't come into play
    // during a normal data run, but can be useful when
    // troubleshooting.
    for (uint8_t i = 0; i < 100; i++) {
        filename[14] = i / 10 + '0';
        filename[15] = i % 10 + '0';

        if (!sd.exists(filename)) {
            // when sd.exists() returns false, this block
            // of code will be executed to open the file

            if (!logfile.open(filename, O_APPEND | O_CREAT | O_WRONLY)) {
                // If there is an error opening the file, notify the
                // user. Otherwise, the file is open and ready for writing
#ifdef DEBUG
                Serial.println("<<< CANT CREATE OR OPEN LOG FILE >>>");
                Serial.flush();
#endif
            } else {
#ifdef DEBUG
                Serial.println("File created");
#endif
            }
#ifdef DEBUG
            Serial.print("Name: ");
            Serial.println(filename);
#endif
            break;  // Break out of the for loop when the statement
                    // if(!logfile.exists()) is finally false (i.e. you found a
                    // new file name to use).
        }           // end of if(!sd.exists())
    }               // end of file-naming for loop

    //------------------------------------------------------------
    // Write 1st header line
    // Header will be: POSIX Time, Date & Time, Hall[0-5] value, Temperature
    logfile.print(
        F("POSIXt,DateTime,hall1,hall2,hall3,hall4,hall5,hall6,Temp.C"));
    logfile.println();
    // Update the file's creation date, modify date, and access date.
    logfile.timestamp(T_CREATE, year, month, day, hour, minute, second);
    logfile.timestamp(T_WRITE, year, month, day, hour, minute, second);
    logfile.timestamp(T_ACCESS, year, month, day, hour, minute, second);

    // force the data to be written to the file by closing it
    if (logfile.close()) {
#ifdef DEBUG
        Serial.print("File writed: ");
        Serial.println(filename);
#endif
    } else {
#ifdef DEBUG
        Serial.println("<<< FAILED TO WRITE IN LOG FILE >>>");
#endif
    }
}

bool SDCard_writeFile(const uint32_t unix_time, const char *timestamp,
                      const uint16_t hall[6], const float tempC) {
    // Reopen logfile. If opening fails, notify the user
    if (!logfile.isOpen()) {
        if (!logfile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
            return true;
        }
    }

#ifdef DEBUG
    Serial.print("Writing to file: ");
#endif
    // Write the unixtime
    logfile.print(unix_time, DEC);
    logfile.print(F(","));  // POSIX time value
#ifdef DEBUG
    Serial.print(unix_time, DEC);
    Serial.print(",");
#endif
    logfile.print(timestamp);
    logfile.print(F(","));  // human-readable time stamp
#ifdef DEBUG
    Serial.print(timestamp);
    Serial.print(",");
#endif
    for (uint8_t _i = 0; _i < 6; ++_i) {
        logfile.print(hall[_i], DEC);
        logfile.print(F(","));  // Hall sensor value
#ifdef DEBUG
        Serial.print(hall[_i], DEC);
        Serial.print(",");
#endif
    }
    logfile.print(tempC, 2);  // Temperature sensor value
    logfile.println();
#ifdef DEBUG
    Serial.print(tempC, 2);
    Serial.println();
#endif

    return !logfile.close();
}