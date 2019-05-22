#include "ModbusMaster.h"

#include <Arduino.h>

#include "SPI.h"
#include "MPU9250.h"
#include "Adafruit_BME680.h"


#define LED_PIN 32

#define SPI_MOSI_PIN 8
#define SPI_MISO_PIN 10
#define SPI_SCKL_PIN 9
#define SPI_NCS_PIN 6

#define RS485_TX_EN 5
#define RS485_RX_EN 4

SPIClass spi(&sercom1, SPI_MISO_PIN, SPI_SCKL_PIN, SPI_MOSI_PIN, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

ModbusMaster node;

Adafruit_BME680 bme;

MPU9250 IMU(spi, SPI_NCS_PIN);

void enableRS485TX() {
	digitalWrite(RS485_TX_EN, HIGH);
	digitalWrite(RS485_RX_EN, HIGH);
}

void disableRS485TX() {
	digitalWrite(RS485_TX_EN, LOW);
	digitalWrite(RS485_RX_EN, LOW);
}


void setupRS485() {
	Serial1.begin(9600);
	
	pinMode(RS485_TX_EN, OUTPUT);
	pinMode(RS485_RX_EN, OUTPUT);
	
	disableRS485TX();
	
	node.preTransmission(enableRS485TX);
	node.postTransmission(disableRS485TX);
	
	node.begin(1, Serial1);
}

void setupAccelerometer() {
	int status = 0;
	
	IMU.disableWhoAmICheck();
	IMU.disableMagnetometer();
	
	do {
		status = IMU.begin();
		if (status < 0) {
			Serial.println("IMU initialization unsuccessful");
			Serial.println("Check IMU wiring or try cycling power");
			Serial.print("Status: ");
			Serial.println(status);
		}
		delay(1000);
	} while (status < 0);
}

void setupBME680() {
	bool status = false;
	
	do {
		status = bme.begin(0x77);
		if (!status) {
			Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
		}
		delay(1000);
	} while (!status);
	
	// Set up oversampling and filter initialization
	bme.setTemperatureOversampling(BME680_OS_8X);
	bme.setHumidityOversampling(BME680_OS_2X);
	bme.setPressureOversampling(BME680_OS_4X);
	bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
	bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void setup() {
	Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);

	setupRS485();
	setupAccelerometer();
	setupBME680();
	
}

void readVoltageOverRS485() {
	uint8_t result = node.readInputRegisters(0x0001, 2);
	
	if(node.ku8MBSuccess == result) {
		uint16_t rawVoltage = node.getResponseBuffer(0) << 16 | node.getResponseBuffer(1);
		Serial.print("[RS485] Voltage: ");
		Serial.println((float)rawVoltage/100);
		} else {
		Serial.print("Failed with status: ");
		Serial.println(result);
		
	}
}

void readAccelerometerOverSPI() {
	IMU.readSensor();
	
	Serial.print(IMU.getAccelX_mss(),6);
	Serial.print("\t");
	Serial.print(IMU.getAccelY_mss(),6);
	Serial.print("\t");
	Serial.print(IMU.getAccelZ_mss(),6);
	Serial.print("\t");
	Serial.print(IMU.getGyroX_rads(),6);
	Serial.print("\t");
	Serial.print(IMU.getGyroY_rads(),6);
	Serial.print("\t");
	Serial.print(IMU.getGyroZ_rads(),6);
	Serial.print("\t");
	Serial.println(IMU.getTemperature_C(),2);
}

void readBME680OverI2C() {
	unsigned long endTime = bme.beginReading();
	if (endTime == 0) {
		Serial.println(F("Failed to begin reading :("));
		return;
	}
	Serial.print(F("Reading started at "));
	Serial.print(millis());
	Serial.print(F(" and will finish at "));
	Serial.println(endTime);
	
	delay(50);
	
	if (!bme.endReading()) {
		Serial.println(F("Failed to complete reading :("));
		return;
	}
	Serial.print(F("Reading completed at "));
	Serial.println(millis());

	Serial.print(F("Temperature = "));
	Serial.print(bme.temperature);
	Serial.println(F(" *C"));

	Serial.print(F("Pressure = "));
	Serial.print(bme.pressure / 100.0);
	Serial.println(F(" hPa"));

	Serial.print(F("Humidity = "));
	Serial.print(bme.humidity);
	Serial.println(F(" %"));

	Serial.print(F("Gas = "));
	Serial.print(bme.gas_resistance / 1000.0);
	Serial.println(F(" KOhms"));

	Serial.println();
}

void loop() {
	Serial.println("--- Start the loop ---");
	digitalWrite(LED_PIN, HIGH);

	readVoltageOverRS485();
	readAccelerometerOverSPI();
	readBME680OverI2C();
	
	delay(500);
	digitalWrite(LED_PIN, LOW);
	delay(500);
}
