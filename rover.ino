#include <Arduino.h>
#include "MQ7.h"
#include "Plantower_PMS7003.h"

// Define pins for motors and sensors
#define MOTOR_LEFT_FORWARD_PIN 9
#define MOTOR_LEFT_BACKWARD_PIN 10
#define MOTOR_RIGHT_FORWARD_PIN 11
#define MOTOR_RIGHT_BACKWARD_PIN 12
#define MQ7_SENSOR_PIN A0

#define SERIAL_SPEED 9600

// Initialize PMS7003 and MQ7 sensor
Plantower_PMS7003 pms7003;
MQ7 mq7(MQ7_SENSOR_PIN, 5.0);
char output[256];

// Function prototypes
void moveForward();
void stopMovement();
void updateAirQuality();
void updateCOLevels();

void setup() {
  Serial.begin(SERIAL_SPEED);
  Serial1.begin(SERIAL_SPEED);
  
  // Initialize PMS7003
  pms7003.init(&Serial1);
  Serial.println("Initializing PMS7003 sensor...");
  delay(1000);  // Allow time for sensor warm-up
}

void loop() {
  pms7003.updateFrame();

  if (pms7003.hasNewData()) {
    updateAirQuality();
  }

  updateCOLevels();

  // Example rover behavior based on air quality and CO levels
  if (pms7003.getPM_2_5() > 35 || mq7.getPPM() > 35) {  // Threshold values for PM2.5 and CO
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

void updateCOLevels() {
  float coPPM = mq7.getPPM();
  Serial.print("CO Concentration (PPM): ");
  Serial.println(coPPM);
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
