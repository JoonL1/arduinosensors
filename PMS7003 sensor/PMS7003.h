#ifndef MQ811_H
#define MQ811_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SD.h>

// PMS7003 sensor class definition
class PMS7003Sensor {
public:
    PMS7003Sensor(int rxPin, int txPin);
    void begin();
    void readData();
    void logDataToSD(File &dataFile);
    void printDataToSerial();

private:
    SoftwareSerial pmsSerial; // Software serial for PMS7003 communication
    uint16_t pm10, pm25, pm100; // PM data storage
    void initializeSensor();
    void requestData();
    bool readResponse();
};

#endif // MQ811_H
