#ifndef BME280_H
#define BME280_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// BME280 sensor class definition
class BME280Sensor {
public:
    BME280Sensor();
    bool begin();
    void readData(float &temperature, float &humidity);
    void logDataToSD(File &dataFile);

private:
    Adafruit_BME280 bme; // BME280 instance
};

#endif // BME280_H
