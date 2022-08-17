#ifndef CONFIGURATION_SERVICE_H_
#define CONFIGURATION_SERVICE_H_

// IDF includes
#include <esp_local_ctrl.h>

// Platform includes
#include <HumiDevice.Platform/Platform.h>

// Project includes
#include "ConfigurationServiceIfc.h"
#include "ConfigurationProperty.h"
#include "KeyValueStorageIfc.h"

/**
 * @see ConfigurationServiceIfc
 */
class ConfigurationService : public ConfigurationServiceIfc {
public:
    /**
     * Constructor
     */
    explicit ConfigurationService();

    /**
     * Deconstructor
     */
    virtual ~ConfigurationService() { }

    /**
     * @see ConfigurationServiceIfc
     */
    virtual esp_err_t startService() override;

    /**
     * @see ConfigurationServiceIfc
     */
    virtual esp_err_t stopService() override;

    /**
     * @brief Set the Storage Driver object
     * 
     * @param pDriver 
     */
    static void setStorageDriver(KeyValueStorageIfc* const pDriver) {
        s_pNvsStorageDriver = pDriver;
    }

private:
    /**
     * Private copy constructor.
     */
    ConfigurationService(const ConfigurationService& other);

    /**
     * Private assignment operator.
     */
    ConfigurationService& operator=(const ConfigurationService& other);

    
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

    // static members
    static KeyValueStorageIfc* s_pNvsStorageDriver;
};

#endif // CONFIGURATION_SERVICE_H_