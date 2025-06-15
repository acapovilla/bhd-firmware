# Bivalve Health Device (firmware)

The **Bivalve Health Device** is an open-source datalogger for bivalve gape and water temperature.

## About the Project

This project is a custom firmware for a datalogger used to monitor bivalve valve activity in aquatic environments. It is designed to record the opening and closing of bivalve shells using Hall effect sensors and magnets, along with ambient temperature and timestamp data. The collected data is stored on an SD card for later retrieval and analysis.

### Features

- **New megaAVR 0-series microcontroller**: Runs on an Arduino Nano Every (ATmega4809)
- **Six Hall Effect Sensors:** Monitors shell gape (opening/closing) with enhanced resolution.
- **DS18B20 Temperature Sensor:** Records temperature using a digital sensor.
- **DS3231 RTC:** Maintains accurate date and time for each measurement, with alarm-based wakeup for low-power operation.
- **SD Card Logging:** Stores all measurements in CSV format for easy analysis.
- **Status LEDs:** Indicates device status and errors for easy troubleshooting.
- **Command Interface:** Supports serial commands for setting device serial number and date/time.
- **Open Source:** Hardware and firmware are fully open for modification and improvement.

### Project Links

- **Firmware Repository:** [https://github.com/acapovilla/bhd-sw](https://github.com/acapovilla/bhd-firmware)

- **Interface Repository:** [https://github.com/gmsanchez/almejas_gui](https://github.com/gmsanchez/almejas_gui)

## Getting Started

This section will guide you through setting up the firmware project on your local machine using PlatformIO and Visual Studio Code. The firmware is ready to build and upload to the target device (Arduino Nano Every) via USB.

A graphical user interface (GUI) is available in a separate repository and can be used to configure the real-time clock (RTC) after flashing the firmware.

### Dependencies

This project uses <a href="https://platformio.org/" target="_blank">PlatformIO</a> for building and uploading the firmware. All dependencies are managed automatically via the `platformio.ini` configuration file.

#### Prerequisites:

- <a href="https://code.visualstudio.com/" target="_blank">Visual Studio Code</a>
- <a href="https://platformio.org/install/ide?install=vscode" target="_blank">PlatformIO IDE extension</a>

Once PlatformIO is installed, all required libraries and toolchains will be installed automatically when you open the project.

### Getting the Source

This project is [hosted on GitHub](https://github.com/acapovilla/bhd-firmware). You can clone the repository using:

```
git clone -b main https://github.com/acapovilla/bhd-firmware
```

### Building

To build the firmware, open the project in VSCode with the PlatformIO extension enabled and click the **Build** button (checkmark icon) on the PlatformIO toolbar,

### Uploading to the BHD

Connect your Arduino Nano Every via USB and click the **Upload** button (arrow icon)

#### Running Tests

There are currently no automated tests implemented for this project.

### Usage

After flashing the firmware, the datalogger will begin operating automatically when powered.

To set the correct time and date on the real-time clock (RTC), connect to the device using the provided graphical interface (available in a separate repository)

## Further Reading

Additional documentation is available in the [`docs`](docs/) directory.

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

This firmware is based in part on the following open-source projects:

- <a href="https://github.com/millerlp/BivalveBit" target="_blank">BivalveBit</a>
- <a href="https://github.com/millerlp/MusselGapeTracker" target="_blank">MusselGapeTracker</a>
- <a href="https://github.com/millerlp/BivalveBit_lib" target="_blank">BivalveBit_lib</a>  
  by <a href="https://github.com/millerlp" target="_blank">Luke Miller</a>

These contributions are acknowledged and respected under the terms of the GPL v3 license.

## Authors

- **[Agustín Capovilla](https://github.com/acapovilla)** - _Initial work_

Also see the list of [contributors](https://github.com/acapovilla/bhd-firmware/contributors) who participated in this project.

## Acknowledgments

This project builds upon and was inspired by the excellent work of:

- **Luke Miller** – Creator of the BivalveBit and MusselGapeTracker projects, which provided foundational ideas and code for this firmware. [https://github.com/millerlp](https://github.com/millerlp)

---

<br>

_README_ based on [embeddedartistry](https://github.com/embeddedartistry/templates) template.
