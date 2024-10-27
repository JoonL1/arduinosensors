/*********************************************************************
This sketch demonstrates the use of the Plantower PMS7003 laser particle
counter sensor with a rover. It integrates the sensor to monitor air
quality and make decisions based on PM1.0, PM2.5, and PM10 concentrations.

Written for the PMS7003 sensor.
BSD license. See license.txt for details.
*********************************************************************/

#include <Arduino.h>
#include "Plantower_PMS7003.h"

// Define pins for motors, sensors, etc.
#define MOTOR_LEFT_FORWARD_PIN 9
#define MOTOR_LEFT_BACKWARD_PIN 10
#define MOTOR_RIGHT_FORWARD_PIN 11
#define MOTOR_RIGHT_BACKWARD_PIN 12

#define SERIAL_SPEED 9600

// Initialize PMS7003 sensor
Plantower_PMS7003 pms7003;
char output[256];

// Function prototypes
void moveForward();
void stopMovement();
void updateAirQuality();

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial1.begin(SERIAL_SPEED);
  
  // Initialize PMS7003
  pms7003.init(&Serial1);
  Serial.println("Initializing PMS7003 sensor...");
  delay(1000);  // Give some time for the sensor to warm up
}

void loop() {
  pms7003.updateFrame();

  if (pms7003.hasNewData()) {
    updateAirQuality();
  }

  // Example rover behavior based on air quality
  if (pms7003.getPM_2_5() > 35) {  // Threshold for PM2.5 (example value)
    Serial.println("Air quality poor! Stopping rover.");
    stopMovement();
  } else {
    Serial.println("Air quality good! Moving forward.");
    moveForward();
  }

  delay(1000);  // Read data every second
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
