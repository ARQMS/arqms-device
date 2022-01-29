#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include "Platform.h"
#include "Events/EventLoopIfc.h"

/**
 * The sensor manager which communicates with all sensors
 */
class SensorManager {
public:
    /**
     * Constructor
     */
    explicit SensorManager(EventLoopIfc& eventLoop);

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

    // Members
    EventLoopIfc& eventLoop;
};

#endif // SENSORMANAGER_H