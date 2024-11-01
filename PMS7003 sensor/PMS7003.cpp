#include "MQ811.h"

// Constructor
PMS7003Sensor::PMS7003Sensor(int rxPin, int txPin) 
    : pmsSerial(rxPin, txPin), pm10(0), pm25(0), pm100(0) {}

// Initialize the PMS7003 sensor
void PMS7003Sensor::begin() {
    pmsSerial.begin(9600); // Start serial communication at 9600 baud
    initializeSensor(); // Additional initialization if needed
}

// Initialize the sensor (if specific setup is needed)
void PMS7003Sensor::initializeSensor() {
    // Any additional initialization can go here if needed
}

// Request data from the PMS7003 sensor
void PMS7003Sensor::requestData() {
    pmsSerial.write(0x42); // Send command to read data
    pmsSerial.write(0x4D); // Followed by additional command byte
}

// Read the response from the sensor
bool PMS7003Sensor::readResponse() {
    if (pmsSerial.available() >= 32) { // Expecting a 32-byte response
        uint8_t response[32];
        pmsSerial.readBytes(response, 32); // Read the response into the buffer

        // Check for valid response (first byte should be 0x42)
        if (response[0] == 0x42) {
            pm10 = (response[10] << 8) | response[9]; // PM10 value
            pm25 = (response[12] << 8) | response[11]; // PM2.5 value
            pm100 = (response[14] << 8) | response[13]; // PM100 value
            return true; // Successfully read the data
        }
    }
    return false; // Failed to read data
}

// Read and process data from the PMS7003 sensor
void PMS7003Sensor::readData() {
    requestData(); // Send request for data
    delay(300); // Wait for the sensor to respond
    if (!readResponse()) {
        Serial.println("Failed to read data from PMS7003");
    }
}

// Log PM data to an SD card
void PMS7003Sensor::logDataToSD(File &dataFile) {
    // Ensure the data file is open
    if (!dataFile) {
        Serial.println("Error: Data file is not open.");
        return;
    }

    readData(); // Read data from the sensor
    dataFile.print("PM10: ");
    dataFile.print(pm10);
    dataFile.print(" particles/m³, PM2.5: ");
    dataFile.print(pm25);
    dataFile.print(" particles/m³, PM100: ");
    dataFile.print(pm100);
    dataFile.println(" particles/m³");
}

// Print PM data to Serial Monitor for debugging
void PMS7003Sensor::printDataToSerial() {
    readData(); // Read data from the sensor
    Serial.print("PM10: ");
    Serial.print(pm10);
    Serial.print(" particles/m³, PM2.5: ");
    Serial.print(pm25);
    Serial.print(" particles/m³, PM100: ");
    Serial.print(pm100);
    Serial.println(" particles/m³");
}
