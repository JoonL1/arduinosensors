#ifndef BME280_H
#define BME280_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Class to handle BME280 sensor functionalities
class BME280Sensor {
public:
    BME280Sensor();
    bool begin();                     // Initialize the BME280 sensor
    void readData();                 // Read temperature, humidity, and pressure
    void logDataToSDCard(File &dataFile); // Log data to SD card
    void displayData();              // Display data on the serial monitor

    float getTemperature();          // Getter for temperature
    float getHumidity();             // Getter for humidity
    float getPressure();             // Getter for pressure

private:
    Adafruit_BME280 bme;             // BME280 instance
    float temperature;                // Temperature in Celsius
    float humidity;                   // Humidity in percentage
    float pressure;                   // Pressure in hPa
};

#endif // BME280_H
