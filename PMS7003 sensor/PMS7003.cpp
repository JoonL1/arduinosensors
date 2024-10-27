#include "Plantower_PMS7003.h"

// Constructor
Plantower_PMS7003::Plantower_PMS7003() {
    dataReady = false;
    initialized = false;
    debug = false;
}

// Initialize the sensor with a specified serial stream
void Plantower_PMS7003::init(Stream *s) {
    dataReady = false;
    serial = s;
    nextByte = 0;
    lastByte = 0;
    bufferIndex = 0;
    initialized = true;
}

// Update the sensor data frame
void Plantower_PMS7003::updateFrame() {
    if (!initialized) {
        Serial.println("Error: must call Plantower_PMS7003::init()");
        return;
    }
    dataReady = false;
    if (serial->available()) {
        nextByte = serial->read();

        if (nextByte == 0x4D && lastByte == 0x42) {
            sensorData.bytes[0] = 0x42;
            bufferIndex = 1;
        }
        sensorData.bytes[bufferIndex] = nextByte;    
        bufferIndex++;
        lastByte = nextByte;
    }

    if (bufferIndex == PMS7003_DATA_SIZE) {
        if (sensorData.bytes[0] == 0x42 && sensorData.bytes[1] == 0x4D) {
            convertSensorData();

            if (isValidChecksum()) {
                dataReady = true;
            } else {
                if (debug) {
                    Serial.println("Invalid data checksum");
                }
            }
        }
        bufferIndex = 0; // Reset buffer index for next frame
    }
}

// Check if new data is available
bool Plantower_PMS7003::hasNewData() {
    return dataReady;
}

// Get PM values
uint16_t Plantower_PMS7003::getPM_1_0() { return sensorData.values.pm_1_0; }
uint16_t Plantower_PMS7003::getPM_2_5() { return sensorData.values.pm_2_5; }
uint16_t Plantower_PMS7003::getPM_10_0() { return sensorData.values.pm_10_0; }
uint16_t Plantower_PMS7003::getPM_1_0_atmos() { return sensorData.values.pm_1_0_atmos; }
uint16_t Plantower_PMS7003::getPM_2_5_atmos() { return sensorData.values.pm_2_5_atmos; }
uint16_t Plantower_PMS7003::getPM_10_0_atmos() { return sensorData.values.pm_10_0_atmos; }

uint16_t Plantower_PMS7003::getRawGreaterThan_0_3() { return sensorData.values.raw_gt_0_3; }
uint16_t Plantower_PMS7003::getRawGreaterThan_0_5() { return sensorData.values.raw_gt_0_5; }
uint16_t Plantower_PMS7003::getRawGreaterThan_1_0() { return sensorData.values.raw_gt_1_0; }
uint16_t Plantower_PMS7003::getRawGreaterThan_2_5() { return sensorData.values.raw_gt_2_5; }
uint16_t Plantower_PMS7003::getRawGreaterThan_5_0() { return sensorData.values.raw_gt_5_0; }
uint16_t Plantower_PMS7003::getRawGreaterThan_10_0() { return sensorData.values.raw_gt_10_0; }

uint8_t Plantower_PMS7003::getHWVersion() { return sensorData.values.version_number; }
uint8_t Plantower_PMS7003::getErrorCode() { return sensorData.values.error_code; }

// Convert sensor data from raw bytes to usable values
void Plantower_PMS7003::convertSensorData() {
    int loc = 0;

    loc += 1; // First byte (fixed)

    // Next 13 words are 16-bit ints
    for (int i = 0; i < 13; i++) {
        sensorData.words[loc] = uint16FromBufferData(sensorData.bytes, loc * 2);
        loc++;
    }

    loc += 1; // Skip one byte for frame length

    // Final checksum word is 16-bit
    sensorData.words[loc] = uint16FromBufferData(sensorData.bytes, loc * 2);
}

// Check data integrity with checksum
bool Plantower_PMS7003::isValidChecksum() {
    uint16_t sum = 0;
    
    for (int i = 0; i < (PMS7003_DATA_SIZE - 2); i++) {
        sum += sensorData.bytes[i];
    }
    return (sum == sensorData.values.checksum);
}

// Convert from buffer data
uint16_t Plantower_PMS7003::uint16FromBufferData(unsigned char *buff, int loc) {
    if (loc > PMS7003_DATA_SIZE - 2 || loc < 2) {
        return -1;
    }
    return ((buff[loc] << 8) + buff[loc + 1]);
}
