#include "MQ811.h"

// Constructor
MQ811Sensor::MQ811Sensor(int pin) : sensorPin(pin), calibrationFactor(2000.0) {}

// Initialize the MQ811 sensor
void MQ811Sensor::begin() {
    pinMode(sensorPin, INPUT); // Set the pin as an input
    initializeSensor(); // Additional initialization if needed
}

// Initialize the sensor (if specific setup is needed)
void MQ811Sensor::initializeSensor() {
    // Add any sensor-specific initialization here if required
}

// Read CO₂ concentration in ppm from the MQ811 sensor
float MQ811Sensor::readCO2() {
    int sensorValue = analogRead(sensorPin); // Read the analog value from the sensor
    float voltage = sensorValue * (5.0 / 1023.0); // Convert the analog value to voltage

    // Convert voltage to CO₂ concentration in ppm
    // The conversion factor may need to be adjusted based on calibration data
    float co2PPM = (voltage - 0.5) * calibrationFactor; // Example conversion formula
    return co2PPM > 0 ? co2PPM : 0; // Ensure no negative readings
}

// Log CO₂ concentration data to an SD card
void MQ811Sensor::logDataToSD(File &dataFile) {
    // Ensure the data file is open
    if (!dataFile) {
        Serial.println("Error: Data file is not open.");
        return;
    }
    
    float co2PPM = readCO2();
    
    // Log the data with timestamp
    dataFile.print("CO2 Concentration: ");
    dataFile.print(co2PPM);
    dataFile.println(" ppm");
}
