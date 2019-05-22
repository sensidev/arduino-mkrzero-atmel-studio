Arduino MKR Zero with Atmel Studio 7
---

# Prerequisites

1. Arduino MKR Zero
2. MPU9250 - accelerometer dev kit
3. BME680 - gas sensor dev kit
4. RS485 - dev kit
5. Equipment - slave Modbus RTU over RS485, we used energy meter Lovato DMED330, but any Modbus RTU slave will do.

# Pinout

We used the standard variant proposed for Arduino MKR Zero: https://github.com/arduino/ArduinoCore-samd/blob/master/variants/mkrzero/variant.cpp


| SAMD Pin Number | MKR Board Pin | PIN  | Sercom | Pad | Connect To    |
|-----------------|---------------|------|--------|-----|---------------|
| 8               | MOSI          | PA16 | 1      | 0   | MPU9250 - SDA |
| 9               | SCK           | PA17 | 1      | 1   | MPU9250 - SKL |
| 10              | MISO          | PA19 | 1      | 3   | MPU9250 - ADO |
| 6               | 1             | PA20 |        |     | MPU9250 - NCS |
| 11              | SDA           | PA08 | 2      | 0   | BME680 - SDA  |
| 12              | SCL           | PA09 | 2      | 1   | BME680 - SCL  |
| 13              | RX            | PB23 | 5      | 3   | RS485 - RO    |
| 14              | TX            | PB22 | 5      | 2   | RS485 - DI    |
| 5               | 14            | PB11 |        |     | RS485 - DE    |
| 4               | 13            | PB10 |        |     | RS485 - RE    |
| 32              |               | PB08 |        |     | STATUS LED    |

# Dependencies

Added with Add Arduino Library to Solution.

* https://github.com/arduino/ArduinoCore-samd/tree/master/libraries/SPI
* https://github.com/arduino/ArduinoCore-samd/tree/master/libraries/Wire
* https://github.com/adafruit/Adafruit_Sensor
* https://github.com/adafruit/Adafruit_BME680
* https://github.com/sensidev/MPU9250
* https://github.com/sensidev/arduino-modbus-master

# Steps to create an Atmel Studio 7 project with Arduino Core.

1. Install Arduino IDE
1. Uninstall the default Arduino AVR boards
1. Install Arduino SAMD boards
1. Install all dependencies from Manage Libraries or by including zip library into Arduino project
1. Create a blank Arduino sketch
1. Create project from the blank Arduino sketch, choose Arduino MKRZERO with ATSAMD21G18A
1. Right click on the project solution and Add Arduino library, add all the dependencies
1. Build the solution
1. Select a programmer (We use Atmel ICE)
1. Run.
