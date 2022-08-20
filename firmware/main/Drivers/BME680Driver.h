#ifndef BME680DRIVER_H_
#define BME680DRIVER_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

// Project inluces
#include "driver/spi_master.h"
#include "BME680Registers.h"

/**
 * Represents BME680 Driver
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
    void initialzie(spi_device_handle_t pSpi);

    /**
     * Resets the hardware
     */
    void reset();
private:
    // Helper
    void setMemPage(const uint8_t adr);
    bool getMemPage();

	template<typename R> void readRegister(R& reg);
	template<typename R> void writeRegister(R& reg);

    // TODO move to Spi Class
    void read(const uint8_t adr, uint8_t& value);
    void write(const uint8_t adr, const uint8_t value);

    // Members
    spi_device_handle_t m_pSpi;
    uint8_t m_chipId;
    BME680Registers::Status m_status;
};

#endif // BME680DRIVER_H_