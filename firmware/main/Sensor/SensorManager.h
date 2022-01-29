#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "Platform.h"

/**
 * The sensor manager which communicates with all sensors
 */
class SensorManager {
public:
    /**
     * Constructor
     */
    explicit SensorManager(void);

    /**
     * Deconstructor
     */
    ~SensorManager() { }

    /**
     * Start sensor data acquisition
     */
    void startDataAcquisition();

private:
    /**
     * Private copy constructor.
     */
    SensorManager(const SensorManager& other);

    /**
     * Private assignment operator.
     */
    SensorManager& operator=(const SensorManager& other);

};

#endif // SENSORMANAGER_H