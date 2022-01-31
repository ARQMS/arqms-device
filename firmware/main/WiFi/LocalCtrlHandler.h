#ifndef LOCALCTRLHANDLER_H
#define LOCALCTRLHANDLER_H

#include "esp_local_ctrl.h"

#include "Platform.h"
#include "DeviceStorage/DeviceStorage.h"

/**
 * Used for local ctrl actions to get and set configurations.
 * 
 * @see https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-reference/protocols/esp_local_ctrl.html
 */
class LocalCtrlHandler {
public:
    /**
     * Constructor
     */
    explicit LocalCtrlHandler(DeviceStorage& storage);

    /**
     * Deconstructor
     */
    ~LocalCtrlHandler() { }

    /**
     * Starts the ctrl service
     */
    void startService(void);

    /**
     * Stops the ctrl service
     */
    void stopService(void);

    /**
     * Indicates when local ctrl service is running
     *
     * @return bool true when service is already running, otherwise false.
     */
    bool isRunning() const;

private:
    /**
     * Private copy constructor.
     */
    LocalCtrlHandler(const LocalCtrlHandler& other);

    /**
     * Private assignment operator.
     */
    LocalCtrlHandler& operator=(const LocalCtrlHandler& other);

    /**
     * A custom property type enum. Just ensure each property type has also a valid getSize corresponding pair!
     */
    class PropertyType {
    public:
        enum Value : uint32_t {
            PROP_TYPE_UINT8,
            PROP_TYPE_UINT16,
            PROP_TYPE_UINT32,
            PROP_TYPE_UINT64,
            PROP_TYPE_INT8,
            PROP_TYPE_INT16,
            PROP_TYPE_INT32,
            PROP_TYPE_INT64,
            PROP_TYPE_FLOAT32,
            PROP_TYPE_BOOL,
            PROP_TYPE_CHAR_STRING,
        };

        constexpr PropertyType(Value val) : value(val) {}
        PropertyType() = default;

        constexpr bool operator==(PropertyType a) const { return value == a.value; }
        constexpr bool operator!=(PropertyType a) const { return value != a.value; }
        constexpr operator Value() const { return value; }
        explicit operator bool() = delete; 

        // return 0 when size is dynamic, otherwise given expected type
        uint32_t getSize() const {
            switch (value) {
                case PROP_TYPE_UINT8: return sizeof(uint8_t);
                case PROP_TYPE_UINT16: return sizeof(uint16_t);
                case PROP_TYPE_UINT32: return sizeof(uint32_t);
                case PROP_TYPE_UINT64: return sizeof(uint64_t);
                case PROP_TYPE_INT8: return sizeof(int8_t);
                case PROP_TYPE_INT16: return sizeof(int16_t);
                case PROP_TYPE_INT32: return sizeof(int32_t);
                case PROP_TYPE_INT64: return sizeof(int64_t);
                case PROP_TYPE_FLOAT32: return sizeof(float32_t);
                case PROP_TYPE_BOOL: return sizeof(bool_t);
                case PROP_TYPE_CHAR_STRING: return 0;
            }
            return 0;
        }

    private:
        Value value;
    };

    /**
     * A custom property flag.
     * 
     * Ensure each value is a single bit!
     */
    enum PropertyFlags : uint32_t {
        NONE                = 0, 
        PROP_FLAG_READONLY  = (1 << 0),
       // _reserved  = (1 << 1),
       // _reserved  = (1 << 2),
       // _reserved  = (1 << 3),
    };
    
    // helper functions
    void registerProperty(const char* name, const PropertyType type, const bool isReadOnly = false);
    static esp_err_t getPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx);
    static esp_err_t setPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], const esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx);

    // Members
    DeviceStorage& storage;
    bool serviceEnabled;
};

#endif // LOCALCTRLHANDLER_H