#ifndef BME280_H
#define BME280_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>  // Include the Adafruit BME280 library

// BME280 Sensor Class Definition
class BME280Sensor {
public:
    // Constructor
    BME280Sensor();

    // Function to initialize the BME280 sensor
    bool begin();

    // Function to read data from the sensor
    void readSensorData();

    // Function to log data to an SD card
    void logData(File &dataFile);

    // Function to display data for debugging purposes
    void displayData();

    // Getters for temperature, humidity, and pressure
    float getTemperature();
    float getHumidity();
    float getPressure();

private:
    Adafruit_BME280 bme; // Instance of the BME280 sensor
    float temperature;    // Variable to store temperature
    float humidity;       // Variable to store humidity
    float pressure;       // Variable to store pressure
};

#endif // BME280_H
