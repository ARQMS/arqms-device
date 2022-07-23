#ifndef EVENTIFC_H_
#define EVENTIFC_H_

// Project includes
#include <HumiPlatform.h>

/**
 * A unique event id to deserialize event 
 */
typedef uint32_t EventId;

/**
 * An event is used to send data or signal to another task 
 */
class EventIfc {
public:

    ~EventIfc();
};


#endif // EVENTIFC_H_