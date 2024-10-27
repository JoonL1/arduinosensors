#include <Arduino.h>
#include "MQ7.h"
#include "MQ811.h"
#include "Plantower_PMS7003.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "VCNL4010.h"

// Define pins for motors and sensors
#define MOTOR_LEFT_FORWARD_PIN 9
#define MOTOR_LEFT_BACKWARD_PIN 10
#define MOTOR_RIGHT_FORWARD_PIN 11
#define MOTOR_RIGHT_BACKWARD_PIN 12
#define MQ7_SENSOR_PIN A0
#define MQ811_SENSOR_PIN A1
#define BME280_I2C_ADDRESS 0x76
#define VCNL4010_I2C_ADDRESS 0x13
#define SERIAL_SPEED 9600

// Initialize sensors
Plantower_PMS7003 pms7003;
MQ7 mq7(MQ7_SENSOR_PIN, 5.0);
MQ811 mq811(MQ811_SENSOR_PIN);
Adafruit_BME280 bme;  // BME280 object
VCNL4010 vcnl4010(VCNL4010_I2C_ADDRESS);
char output[256];

// Function prototypes
void moveForward();
void stopMovement();
void updateAirQuality();
void updateCOLevels();
void updateCO2Levels();
void updateWeatherData();
void updateWindSpeed();
bool initializeSensors();

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial1.begin(SERIAL_SPEED);

    if (!initializeSensors()) {
        Serial.println("Sensor initialization failed. Stopping execution.");
        while (1);  // Halt execution if sensors fail to initialize
    }
    Serial.println("All sensors initialized successfully.");
}

void loop() {
    pms7003.updateFrame();

    if (pms7003.hasNewData()) {
        updateAirQuality();
    }

    updateCOLevels();
    updateCO2Levels();
    updateWeatherData();
    updateWindSpeed();

    // Example rover behavior based on air quality and CO levels
    if (pms7003.getPM_2_5() > 35 || mq7.getPPM() > 35 || mq811.getPPM() > 1000) {  // Threshold values for PM2.5, CO, and CO₂
        Serial.println("Air quality poor! Stopping rover.");
        stopMovement();
    } else {
        Serial.println("Air quality good! Moving forward.");
        moveForward();
    }

    delay(1000);  // Read data every second
}

bool initializeSensors() {
    Serial.println("Initializing PMS7003 sensor...");
    pms7003.init(&Serial1);
    delay(1000);  // Allow time for sensor warm-up

    if (!bme.begin(BME280_I2C_ADDRESS)) {
        Serial.println("Could not find BME280 sensor!");
        return false;  // Return false if initialization fails
    }

    if (!vcnl4010.begin()) {
        Serial.println("Could not find VCNL4010 sensor!");
        return false;  // Return false if initialization fails
    }

    return true;  // Return true if all sensors initialized successfully
}

void updateAirQuality() {
    sprintf(output, "\nSensor Version: %d    Error Code: %d\n",
            pms7003.getHWVersion(),
            pms7003.getErrorCode());
    Serial.print(output);

    sprintf(output, "PM1.0 (ug/m3): %2d     [atmos: %d]\n",
            pms7003.getPM_1_0(),
            pms7003.getPM_1_0_atmos());
    Serial.print(output);

    sprintf(output, "PM2.5 (ug/m3): %2d     [atmos: %d]\n",
            pms7003.getPM_2_5(),
            pms7003.getPM_2_5_atmos());
    Serial.print(output);

    sprintf(output, "PM10 (ug/m3): %2d     [atmos: %d]\n",
            pms7003.getPM_10_0(),
            pms7003.getPM_10_0_atmos());
    Serial.print(output);
}

void updateCOLevels() {
    float coPPM = mq7.getPPM();
    Serial.print("CO Concentration (PPM): ");
    Serial.println(coPPM);
}

void updateCO2Levels() {
    float co2PPM = mq811.getPPM();
    Serial.print("CO2 Concentration (PPM): ");
    Serial.println(co2PPM);
}

void updateWeatherData() {
    float temperature = bme.readTemperature(); // °C
    float humidity = bme.readHumidity();       // %

    sprintf(output, "Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);
    Serial.print(output);
}

void updateWindSpeed() {
    int spins = vcnl4010.readSpins(); // Assume readSpins() function to get the number of spins
    float windSpeedKPH = calculateWindSpeed(spins); // Custom function to calculate wind speed
    Serial.print("Wind Speed: ");
    Serial.print(windSpeedKPH);
    Serial.println(" km/h");
}

float calculateWindSpeed(int spins) {
    // Example calculation: adjust based on your pinwheel specs
    float windSpeed = spins * 0.5; // Example conversion factor
    return windSpeed;
}

void moveForward() {
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    Serial.println("Moving forward.");
}

void stopMovement() {
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, LOW);
    Serial.println("Rover stopped.");
}
