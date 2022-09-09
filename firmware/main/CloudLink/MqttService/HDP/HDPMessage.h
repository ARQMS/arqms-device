#ifndef HDP_MESSAGE_H_
#define HDP_MESSAGE_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

#include <cJSON.h>

/**
 * Interface for any HDP message
 */
class HDPMessage {
public:
    /**
     * Destructor
     */
    virtual ~HDPMessage() {
    };

    /**
     * Gets the JSON object
     * 
     * @param obj the root json object where attributes will be appended to
     */
    virtual void getJson(cJSON* obj) const = 0;
};

#endif // HDP_MESSAGE_H_