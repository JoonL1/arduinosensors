#include "VCNL4010.h"

// Constructor
VCNL4010Sensor::VCNL4010Sensor(uint8_t address) 
    : _address(address), _spinCount(0), _windSpeedKPH(0.0), _lastCountTime(0) {}

// Initialize the VCNL4010 sensor
void VCNL4010Sensor::begin() {
    Wire.begin(); // Initialize I2C communication
    // Additional initialization can be done here
    resetSpinCount(); // Reset spin count to start fresh
}

// Update the spin count and calculate wind speed
void VCNL4010Sensor::update() {
    unsigned long currentTime = millis();
    if (currentTime - _lastCountTime >= 1000) { // Update every second
        // Calculate wind speed based on spins counted
        _windSpeedKPH = (_spinCount * 60.0) / 1000.0; // Convert spins to KPH (assuming distance per spin is accounted for)
        resetSpinCount(); // Reset count for next interval
        _lastCountTime = currentTime; // Update the last count time
    }
}

// Get the calculated wind speed in kilometers per hour
float VCNL4010Sensor::getWindSpeedKPH() const {
    return _windSpeedKPH;
}

// Log wind speed data to an SD card
void VCNL4010Sensor::logDataToSD(File &dataFile) {
    // Ensure the data file is open
    if (!dataFile) {
        Serial.println("Error: Data file is not open.");
        return;
    }
    
    update(); // Update wind speed
    dataFile.print("Wind Speed: ");
    dataFile.print(getWindSpeedKPH());
    dataFile.println(" km/h");
}

// Print wind speed data to Serial Monitor for debugging
void VCNL4010Sensor::printDataToSerial() const {
    Serial.print("Wind Speed: ");
    Serial.print(getWindSpeedKPH());
    Serial.println(" km/h");
}

// Reset spin count
void VCNL4010Sensor::resetSpinCount() {
    _spinCount = 0; // Reset the spin count
}

// Increment spin count (this should be called in an interrupt routine)
void VCNL4010Sensor::incrementSpinCount() {
    _spinCount++; // Increment the spin count
}
