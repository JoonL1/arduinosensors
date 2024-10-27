#ifndef H_MQ7
#define H_MQ7

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class MQ7 {
public:
    MQ7(uint8_t pin);
    void begin();
    float read(); // Return CO concentration in ppm
    float raw();  // Return raw voltage from the sensor
    void calibrate(); // Calibration routine

private:
    uint8_t _pin;
    float _RLoad; // Load resistance
    float voltageConversion(int value);
    float getRatio();
};

#endif
