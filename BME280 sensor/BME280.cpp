#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>  // Adafruit BME280 Library

// Create an instance of the BME280 sensor
Adafruit_BME280 bme; // I2C

// Sensor data variables
float temperature, humidity, pressure;

// Function to initialize the BME280 sensor
bool initBME280() {
    if (!bme.begin(0x76)) { // Check the sensor I2C address (0x76 is common for BME280)
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        return false;
    }
    return true;
}

// Function to read sensor data
void readBME280Data() {
    temperature = bme.readTemperature(); // Read temperature in Celsius
    humidity = bme.readHumidity();       // Read humidity percentage
    pressure = bme.readPressure() / 100.0F; // Read pressure in hPa
}

// Function to log data to SD card
void logDataToSDCard(File &dataFile) {
    // Format and log data
    dataFile.print("Temperature: ");
    dataFile.print(temperature);
    dataFile.print(" °C, ");
    dataFile.print("Humidity: ");
    dataFile.print(humidity);
    dataFile.print(" %, ");
    dataFile.print("Pressure: ");
    dataFile.print(pressure);
    dataFile.println(" hPa");
}

// Function to display data on serial monitor for debugging
void displayData() {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, ");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, ");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");
}

// Setup function
void setup() {
    Serial.begin(9600); // Start the serial communication at 9600 baud
    if (!initBME280()) {
        while (1); // Halt if the sensor cannot be initialized
    }
}

// Loop function
void loop() {
    // Read data from the BME280 sensor
    readBME280Data();

    // Log data to SD card (assume dataFile is already opened elsewhere in your main code)
    File dataFile; // Assume dataFile is correctly initialized before calling logDataToSDCard()
    logDataToSDCard(dataFile);

    // Optional: Display the data on the serial monitor for debugging
    displayData();

    delay(10000); // Wait for 10 seconds before the next reading
}
