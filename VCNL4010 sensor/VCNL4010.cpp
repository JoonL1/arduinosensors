#include <SD.h>
#include <SPI.h>
#include "VCNL4010.h"

#define CS_PIN 10 // CS pin for SD card
#define INTERRUPT_PIN 2 // Interrupt pin for pinwheel

VCNL4010Sensor vcnl4010Sensor; // Initialize the VCNL4010 sensor
File dataFile;

// Interrupt service routine for counting spins
void countSpin() {
    vcnl4010Sensor.incrementSpinCount(); // Increment the spin count on interrupt
}

void setup() {
    Serial.begin(115200);
    pinMode(INTERRUPT_PIN, INPUT_PULLUP); // Set interrupt pin as input with pull-up
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), countSpin, RISING); // Attach interrupt

    if (!SD.begin(CS_PIN)) {
        Serial.println("SD card initialization failed!");
        return;
    }
    dataFile = SD.open("WindData.txt", FILE_WRITE);
    if (!dataFile) {
        Serial.println("Error opening WindData.txt");
    }
    
    vcnl4010Sensor.begin(); // Initialize the VCNL4010 sensor
}

void loop() {
    vcnl4010Sensor.logDataToSD(dataFile); // Log wind speed data to SD
    vcnl4010Sensor.printDataToSerial(); // Print wind speed to Serial Monitor
    delay(1000); // Adjust the delay as necessary for your application
}
