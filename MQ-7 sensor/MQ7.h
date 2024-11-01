#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>
#include <SD.h>

// MQ7 sensor class definition
class MQ7Sensor {
public:
    MQ7Sensor(int pin);
    void begin();
    float readCO();
    void logDataToSD(File &dataFile);

private:
    int sensorPin; // Pin where the sensor is connected
    float calibrationFactor; // Calibration factor for CO measurement
    void initializeSensor();
};

#endif // MQ7_H
