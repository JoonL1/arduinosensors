#ifndef H_MG811
#define H_MG811

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class MG811 {
public:
    MG811(uint8_t input);
    void begin(float v400, float v40000);
    float read();       // return the air quality in 'ppm'
    float raw();        // return raw data (in volt) from the sensor
    void calibrate();   // calibrate the sensor to get reference values

private:
    uint8_t _input;
    float _V400;
    float _V40000;
};

#endif
