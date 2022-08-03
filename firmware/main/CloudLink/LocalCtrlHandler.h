#ifndef LOCALCTRLHANDLER_H
#define LOCALCTRLHANDLER_H

// IDF includes
#include <esp_local_ctrl.h>

// Platform includes
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "LocalCtrlHandlerIfc.h"
#include "ConfigProviderIfc.h"

/**
 * @see LocalCtrlHandlerIfc
 */
class LocalCtrlHandler : public LocalCtrlHandlerIfc {
public:
    /**
     * Constructor
     */
    explicit LocalCtrlHandler(ConfigProviderIfc& provider);

    /**
     * Deconstructor
     */
    virtual ~LocalCtrlHandler() { }

    /**
     * @see LocalCtrlHandlerIfc
     */
    virtual esp_err_t startService() override;

    /**
     * @see LocalCtrlHandlerIfc
     */
    virtual esp_err_t stopService() override;

    /**
     * @see LocalCtrlHandlerIfc
     */
    virtual bool isRunning() const override;

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

    /**
     * Handler function to be implemented for retrieving current values of properties
     *
     * @note  If any of the properties have fixed sizes, the size field of
     * corresponding element in `prop_values` need to be set
     *
     * @param[in]  props_count  Total elements in the props array
     * @param[in]  props        Array of properties, the current values for which
     *                          have been requested by the client
     * @param[out] prop_values  Array of empty property values, the elements of
     *                          which need to be populated with the current values
     *                          of those properties specified by props argument
     * @param[in]  usr_ctx      This provides value of the `usr_ctx` field of
     *                          `esp_local_ctrl_handlers_t` structure
     *
     * @return Returning different error codes will convey the corresponding
     *         protocol level errors to the client :
     *         - ESP_OK : Success
     *         - ESP_ERR_INVALID_ARG   : InvalidArgument
     *         - ESP_ERR_INVALID_STATE : InvalidProto
     *         - All other error codes : InternalError
     */
    static esp_err_t getPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx);

    /**
     * Handler function to be implemented for changing values of properties
     *
     * @note  If any of the properties have variable sizes, the size field
     * of the corresponding element in `prop_values` must be checked
     * explicitly before making any assumptions on the size.
     *
     * @param[in]  props_count  Total elements in the props array
     * @param[in]  props        Array of properties, the values for which the
     *                          client requests to change
     * @param[in]  prop_values  Array of property values, the elements of which
     *                          need to be used for updating those properties
     *                          specified by props argument
     * @param[in]  usr_ctx      This provides value of the `usr_ctx` field of
     *                          `esp_local_ctrl_handlers_t` structure
     *
     * @return Returning different error codes will convey the corresponding
     *         protocol level errors to the client :
     *         - ESP_OK : Success
     *         - ESP_ERR_INVALID_ARG   : InvalidArgument
     *         - ESP_ERR_INVALID_STATE : InvalidProto
     *         - All other error codes : InternalError
     */
    static esp_err_t setPropertyValues(size_t props_count, const esp_local_ctrl_prop_t props[], const esp_local_ctrl_prop_val_t prop_values[], void *usr_ctx);

    // Members
    ConfigProviderIfc& m_provider;
    bool m_isServiceEnabled;
};

#endif // LOCALCTRLHANDLER_H