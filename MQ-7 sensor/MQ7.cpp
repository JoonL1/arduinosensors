#include "MQ7.h"

// Constructor
MQ7Sensor::MQ7Sensor(int pin) : sensorPin(pin), calibrationFactor(2000.0) {}

// Initialize the MQ7 sensor
void MQ7Sensor::begin() {
    pinMode(sensorPin, INPUT); // Set the pin as an input
    initializeSensor(); // Additional initialization if needed
}

// Initialize the sensor (if specific setup is needed)
void MQ7Sensor::initializeSensor() {
    // Add any sensor-specific initialization here if required
}

// Read CO concentration in ppm from the MQ7 sensor
float MQ7Sensor::readCO() {
    int sensorValue = analogRead(sensorPin); // Read the analog value from the sensor
    float voltage = sensorValue * (5.0 / 1023.0); // Convert the analog value to voltage

    // Convert voltage to CO concentration in ppm
    // The conversion factor may need to be adjusted based on calibration data
    float coPPM = (voltage - 0.5) * calibrationFactor; // Example conversion formula
    return coPPM > 0 ? coPPM : 0; // Ensure no negative readings
}

// Log CO concentration data to an SD card
void MQ7Sensor::logDataToSD(File &dataFile) {
    // Ensure the data file is open
    if (!dataFile) {
        Serial.println("Error: Data file is not open.");
        return;
    }
    
    float coPPM = readCO();
    
    // Log the data with timestamp
    dataFile.print("CO Concentration: ");
    dataFile.print(coPPM);
    dataFile.println(" ppm");
}
