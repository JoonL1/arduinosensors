#ifndef MQ7_H
#define MQ7_H

#include <Arduino.h>

#define coefficient_A 19.32
#define coefficient_B -0.64
#define R_Load 10.0 // Load resistance 10 kOhms on the sensor potentiometer

class MQ7 {
  private:
    uint8_t analogPin;
    float v_in;
    float voltageConversion(int value);
  
  public:
    MQ7(uint8_t pin, float v_input);
    float getPPM();
    float getSensorResistance();
    float getRatio();
};

#endif
