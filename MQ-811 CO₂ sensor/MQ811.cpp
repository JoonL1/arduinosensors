#include "MG811.h"

/**
 * Constructor
 */
MG811::MG811(uint8_t input) {
    _input = input;
    _V400 = 4.535;   // Initial value - must be calibrated
    _V40000 = 3.206; // Initial value - must be calibrated
}

/**
 * Initialize the usage of the sensor with calibrated values
 */
void MG811::begin(float v400, float v40000) {
    _V400 = v400;
    _V40000 = v40000;
}

/**
 * Measure multiple raw data from the sensor and compute the mean
 */
float MG811::raw() {
    uint8_t i = 0;
    float buffer = 0;
    for (i = 0; i < 10; i++) {
        buffer += analogRead(_input);
        delay(20); // 20ms
    }
    buffer /= i; // Compute the mean
    return map(buffer, 0, 1023, 0, 5);
}

/**
 * Measure voltage from the sensor and compute the CO2 ppm value
 */
float MG811::read() {
    float buffer = 0;
    buffer = (_V400 - _V40000) / (log10(400) - log10(40000)); // Delta V
    buffer = (raw() - _V400) / buffer;
    buffer += log10(400);
    return pow(10, buffer);
}

/**
 * Calibrate the sensor to get reference value for measurement
 */
void MG811::calibrate() {
    uint8_t i = 0;
    Serial.println("Time (min) \t\t Measurement (volt)");
    for (i = 0; i < 120; i++) {
        delay(60000); // Wait 1 minute
        Serial.print(i);
        Serial.print("\t\t ");
        Serial.println(raw(), 3);
    }
}
