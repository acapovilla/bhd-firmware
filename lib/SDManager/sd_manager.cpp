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

#define DEBUG

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
