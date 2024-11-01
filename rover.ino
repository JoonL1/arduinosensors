#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "VCNL4010.h"

// Include libraries for the sensors
#include <PMS7003.h>
#include <MQ7.h>
#include <MQ811.h>

// Motor Driver Pins
#define ENA 5
#define IN1 6
#define IN2 7
#define ENB 8
#define IN3 9
#define IN4 10

// SD Card Pin
#define CS_PIN 4 // Chip select pin for SD card

// Create sensor instances
PMS7003 pms7003;
MQ7 mq7(A0);
MQ811 mq811(A1);
Adafruit_BME280 bme;
VCNL4010Sensor vcnl4010;

// Data logging file
File dataFile;

// Function to initialize all sensors
void initializeSensors() {
    // Initialize PMS7003
    pms7003.begin();
    // Initialize MQ-7 and MQ-811
    mq7.begin();
    mq811.begin();
    // Initialize BME280
    if (!bme.begin(0x76)) { // Check BME280 I2C address
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    // Initialize VCNL4010
    vcnl4010.begin();
}

// Function to initialize SD card
void initializeSD() {
    Serial.print("Initializing SD card...");
    if (!SD.begin(CS_PIN)) {
        Serial.println("SD card initialization failed!");
        while (1);
    }
    Serial.println("SD card is ready to use.");
}

// Function to write data to SD
void logData() {
    dataFile = SD.open("RoverData.txt", FILE_WRITE);
    if (dataFile) {
        // Log sensor data
        dataFile.print("Time: ");
        dataFile.print(millis() / 1000);
        dataFile.print("s, PM: ");
        dataFile.print(pms7003.getPM2_5());
        dataFile.print(" particles/m³, CO: ");
        dataFile.print(mq7.readCO());
        dataFile.print(" ppm, CO₂: ");
        dataFile.print(mq811.readCO2());
        dataFile.print(" ppm, Temp: ");
        dataFile.print(bme.readTemperature());
        dataFile.print(" °C, Humidity: ");
        dataFile.print(bme.readHumidity());
        dataFile.print(" %, Wind Speed: ");
        dataFile.print(vcnl4010.getWindSpeedKPH());
        dataFile.println(" kph");
        dataFile.close(); // Close the file
    } else {
        Serial.println("Error opening data file");
    }
}

// Function to control motors
void controlMotors(int leftSpeed, int rightSpeed) {
    analogWrite(ENA, leftSpeed);
    analogWrite(ENB, rightSpeed);
    digitalWrite(IN1, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, leftSpeed < 0 ? HIGH : LOW);
    digitalWrite(IN3, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, rightSpeed < 0 ? HIGH : LOW);
}

void setup() {
    Serial.begin(115200);
    initializeSD();
    initializeSensors();

    // Set motor pins as output
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
    // Control motors (example: moving forward)
    controlMotors(255, 255); // Set speed for both motors

    // Log data every 10 seconds
    static unsigned long lastLogTime = 0;
    if (millis() - lastLogTime >= 10000) {
        lastLogTime = millis();
        logData(); // Log sensor data to SD card
    }

    // Delay for stability
    delay(100);
}
