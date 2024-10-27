#ifndef _Plantower_PMS7003_H_
#define _Plantower_PMS7003_H_

#include <Arduino.h>

#define PMS7003_DATA_SIZE 32 // 0x42 + 0x4d + 28 bytes data + 2 bytes checksum

// Structure to hold PMS7003 sensor data
typedef struct {
    uint8_t start_1;          // Start char 1 (0x42)
    uint8_t start_2;          // Start char 2 (0x4d)
    uint16_t frame_length;    // Frame length (data length + parity)
    
    // PM concentration values (in µg/m³)
    uint16_t pm_1_0;          
    uint16_t pm_2_5;         
    uint16_t pm_10_0;        

    // PM concentration values in the atmosphere (in µg/m³)
    uint16_t pm_1_0_atmos;    
    uint16_t pm_2_5_atmos;    
    uint16_t pm_10_0_atmos;   

    // Raw particle counts
    uint16_t raw_gt_0_3;      
    uint16_t raw_gt_0_5;      
    uint16_t raw_gt_1_0;      
    uint16_t raw_gt_2_5;      
    uint16_t raw_gt_5_0;      
    uint16_t raw_gt_10_0;     

    // Sensor version and error codes
    uint8_t version_number;   
    uint8_t error_code;       
    uint16_t checksum;        
} PMS7003_STRUCT;

// Union to access sensor data as bytes and words
typedef union {
    unsigned char bytes[PMS7003_DATA_SIZE];
    uint16_t words[PMS7003_DATA_SIZE / 2];
    PMS7003_STRUCT values;
} PMS7003_DATABUF;

class Plantower_PMS7003 {
public:
    bool debug; // Enable debug messages
    Plantower_PMS7003();
    
    void init(Stream *serial); // Initialize with a specified serial stream
    void updateFrame();         // Update sensor data frame
    bool hasNewData();          // Check if new data is available

    // Get PM values
    uint16_t getPM_1_0();
    uint16_t getPM_2_5();
    uint16_t getPM_10_0();
    uint16_t getPM_1_0_atmos();
    uint16_t getPM_2_5_atmos();
    uint16_t getPM_10_0_atmos();
    
    // Get raw particle counts
    uint16_t getRawGreaterThan_0_3();
    uint16_t getRawGreaterThan_0_5();
    uint16_t getRawGreaterThan_1_0();
    uint16_t getRawGreaterThan_2_5();
    uint16_t getRawGreaterThan_5_0();
    uint16_t getRawGreaterThan_10_0();

    // Get hardware version and error code
    uint8_t getHWVersion();
    uint8_t getErrorCode();

private:
    PMS7003_DATABUF sensorData; // Sensor data buffer
    bool dataReady;             // Flag to indicate new data readiness
    bool initialized;           // Initialization flag
    Stream *serial;            // Pointer to the serial stream
    unsigned char lastByte, nextByte; // Variables for reading data
    int bufferIndex;           // Index for the data buffer

    void dumpBytes();          // Dump raw byte data (for debugging)
    void convertSensorData();  // Convert raw data into usable values
    bool isValidChecksum();     // Check data integrity with checksum
    uint16_t uint16FromBufferData(unsigned char *buff, int loc); // Convert from buffer
};

#endif /*_Plantower_PMS7003_H_*/
