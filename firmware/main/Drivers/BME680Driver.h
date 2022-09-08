#ifndef BME680DRIVER_H_
#define BME680DRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

#include "Events/SensorDataEvent.h"

/**
 * An BME680 wrapper class for official bosch driver API @see https://github.com/BoschSensortec/BME68x-Sensor-API
 * This class encapulate all API calls and uses die SPI interface of bosch bme680 ic.
 * 
 * In future, maybe a custom driver will be written so we encapsulate all API calls inside this wrapper class.
 */
class BME680Driver {
public:
    /**
     * Constructor
     */
    BME680Driver();

    /**
     * Destructor
     */
    ~BME680Driver();

    /**
     * Initialize sensor
     */
    void initialize();

    /**
     * Resets the hardware
     */
    void reset();

    /**
     * Starts a self check
     * 
     * @return true when selfcheck passed, otherwise false
     */
    bool selfCheck();

    /**
     * 
     * Starts a snapshot of current sensor. @see BME680Driver::getData()
     * to read data after time getWaitForNewDataMs is ellapsed.
     */
    void startSnapshot();

    /**
     * Writes the consumed sensor values into data argument
     * 
     * @param data out of sensor values
     */
    void getData(SensorDataEvent& data);

    /**
     * Gets the calculated delay time before sensor data are available.
     * @return uint32_t delay in ms to wait at least before data ready.
     */
    uint32_t getWaitForNewDataMs() const;

private:
    // Helper methods
    static int8_t read(uint8_t adr, uint8_t* pData, uint32_t len, void* pIntf);
    static int8_t write(uint8_t adr, const uint8_t *pData, uint32_t len, void* pIntf);
    static void delay(uint32_t period, void* pIntf);
    static void checkApiRslt(const char api_name[], int8_t rslt);

    // Members
    uint32_t m_delayDataMs;
};

#endif // BME680DRIVER_H_