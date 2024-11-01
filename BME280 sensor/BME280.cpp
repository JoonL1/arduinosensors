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

// Read temperature and humidity from the BME280 sensor
void BME280Sensor::readData(float &temperature, float &humidity) {
    temperature = bme.readTemperature(); // Get temperature in Celsius
    humidity = bme.readHumidity();       // Get humidity percentage
}

// Log sensor data to an SD card
void BME280Sensor::logDataToSD(File &dataFile) {
    float temperature, humidity;
    readData(temperature, humidity);
    
    // Format and log the data
    dataFile.print("Temperature: ");
    dataFile.print(temperature);
    dataFile.print(" °C, Humidity: ");
    dataFile.print(humidity);
    dataFile.println(" %");
}
