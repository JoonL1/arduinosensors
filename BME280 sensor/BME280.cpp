#include "BME280.h"

// Constructor
BME280Sensor::BME280Sensor() {}

// Initialize the BME280 sensor
bool BME280Sensor::begin() {
    if (!bme.begin(0x76)) { // Check the I2C address
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return false;
    }
    return true;
}

// Read sensor data
void BME280Sensor::readData() {
    temperature = bme.readTemperature(); // Read temperature
    humidity = bme.readHumidity();       // Read humidity
    pressure = bme.readPressure() / 100.0F; // Read pressure and convert to hPa
}

// Log data to SD card
void BME280Sensor::logDataToSDCard(File &dataFile) {
    dataFile.print("Temperature: ");
    dataFile.print(temperature);
    dataFile.print(" °C, ");
    dataFile.print("Humidity: ");
    dataFile.print(humidity);
    dataFile.print(" %, ");
    dataFile.print("Pressure: ");
    dataFile.print(pressure);
    dataFile.println(" hPa");
}

// Display data on serial monitor for debugging
void BME280Sensor::displayData() {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, ");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
}

// Get temperature
float BME280Sensor::getTemperature() {
    return temperature;
}

// Get humidity
float BME280Sensor::getHumidity() {
    return humidity;
}

// Get pressure
float BME280Sensor::getPressure() {
    return pressure;
}
