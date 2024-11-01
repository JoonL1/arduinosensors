#ifndef MQ811_H
#define MQ811_H

#include <Arduino.h>
#include <SD.h>

// MQ811 sensor class definition
class MQ811Sensor {
public:
    MQ811Sensor(int pin);
    void begin();
    float readCO2();
    void logDataToSD(File &dataFile);

private:
    int sensorPin; // Pin where the sensor is connected
    float calibrationFactor; // Calibration factor for CO₂ measurement
    void initializeSensor();
};

#endif // MQ811_H
