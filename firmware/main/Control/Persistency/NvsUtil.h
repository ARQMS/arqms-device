#ifndef NVS_UTIL_H_
#define NVS_UTIL_H_

// Platform include
#include <HumiDevice.Platform/Platform.h>

/**
 * Util class for read/write to NVS storage driver
 */
class NvsUtil {
public:
    // The partition namespace, must be same as in paritions.csv configured!
    const static char8_t* PARTITION_NAMESPACE;

    // get and set a string. String must be ended with \0 !
    static void setString(const char8_t* key, const char8_t* value);
    static void getString(const char8_t* key, char8_t* value, const size_t valueLength, const char8_t* defaultVal, const size_t defaultLength);

    // get and set a unsigned integer 8 bit
    static void setU8(const char8_t* key, const uint8_t value);
    static void getU8(const char8_t* key, uint8_t* value, const uint8_t defaultVal);

    // get and set a unsigned integer 32 bit
    static void setU32(const char8_t* key, const uint32_t value);
    static void getU32(const char8_t* key, uint32_t* value, const uint32_t defaultVal);

private:
    /**
     * Constructor
     */
    NvsUtil();

    /**
     * Destructor
     */
    ~NvsUtil();
};

#endif // NVS_UTIL_H_
