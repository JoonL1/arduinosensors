#include "MQ7.h"

// Constructor to initialize the analog pin and input voltage to MQ7
MQ7::MQ7(uint8_t pin, float v_input) {
  analogPin = pin;
  v_in = v_input;
}

// Function to return the ppm value of CO gas concentration
float MQ7::getPPM() {
  return (float)(coefficient_A * pow(getRatio(), coefficient_B));
}

// Function to convert analog input value to voltage
float MQ7::voltageConversion(int value) {
  return (float)value * (v_in / 1023.0);
}

// Function to derive the Rs/R0 to find ppm
float MQ7::getRatio() {
  int value = analogRead(analogPin);
  float v_out = voltageConversion(value);
  return (v_in - v_out) / v_out;
}

// Function to find the sensor resistance Rs
float MQ7::getSensorResistance() {
  return R_Load * getRatio();
}
