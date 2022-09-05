#include "BME680Driver.h"

#include "ApplicationHardwareConfig.h"
#include "Bosch68x.Api/bme68x.h"

#include "freertos/task.h"

// c struct for bosch sensor
struct bme68x_dev driver;

BME680Driver::BME680Driver() :
    m_delayDataMs(0U) {
    driver.read = &BME680Driver::read;
    driver.write = &BME680Driver::write;
    driver.intf = BME68X_SPI_INTF;
    driver.delay_us = &BME680Driver::delay;
    driver.intf_ptr = this;
    driver.amb_temp = 25; // default value
}

BME680Driver::~BME680Driver() {
}

void BME680Driver::initialize() {
    checkApiRslt("bme68x_init", bme68x_init(&driver));

    bme68x_conf conf = bme68x_conf();
    conf.filter = BME68X_FILTER_SIZE_7;
    conf.odr = BME68X_ODR_250_MS;
    conf.os_hum = BME68X_OS_4X;
    conf.os_pres = BME68X_OS_4X;
    conf.os_temp = BME68X_OS_16X;
    checkApiRslt("bme68x_set_conf", bme68x_set_conf(&conf, &driver));

    // ATTENTION: At the moment m_delayDataMs is calculated based on heater configuration,
    // so do not change those configuration dynamically. Because MeasSensorTask timer is not updated 
    // too!
    bme68x_heatr_conf heatrConf = bme68x_heatr_conf();
    heatrConf.enable = BME68X_ENABLE;
    heatrConf.heatr_temp = 300;
    heatrConf.heatr_dur = 100;
    checkApiRslt("bme68x_set_heatr_conf", bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatrConf, &driver));

    // Calculate delay period in microseconds
    const uint32_t period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &driver) + (heatrConf.heatr_dur * 1000);
    m_delayDataMs = (period + 1000) / 1000; // ceiling to ms
}

bool BME680Driver::selfCheck() {
    int8_t rslt = bme68x_selftest_check(&driver);
    checkApiRslt("bme68x_selftest_check", rslt);

    return rslt == BME68X_OK;
}

void BME680Driver::startSnapshot() {
    checkApiRslt("bme68x_set_op_mode", bme68x_set_op_mode(BME68X_FORCED_MODE, &driver));
}

void BME680Driver::getData(SensorDataEvent& data) {
    // Check if rslt == BME68X_OK, report or handle if otherwise
    bme68x_data values = bme68x_data();
    uint8_t fields = 0;
    checkApiRslt("bme68x_get_data", bme68x_get_data(BME68X_FORCED_MODE, &values, &fields, &driver));

    data.setPressure(values.pressure);
    data.setRelativeHumidity(values.humidity);
    data.setTemperature(values.temperature);
    // data.setCo2(0f); // TODO
    // data.setVoc(0f); // TODO
}

uint32_t BME680Driver::getWaitForNewDataMs() const {
    return m_delayDataMs;
}

void BME680Driver::reset() {
    bme68x_soft_reset(&driver);
}

int8_t BME680Driver::read(uint8_t adr, uint8_t* pData, uint32_t len, void* pIntf) {
    uint8_t rxBuffer[len+1];
    uint8_t txBuffer[len+1];

    txBuffer[0] = adr;

    spi_transaction_t transaction = spi_transaction_t();
    transaction.tx_buffer = txBuffer;
    transaction.rx_buffer = rxBuffer;
    transaction.length = (len + 1) * 8;

    const esp_err_t err = spi_device_polling_transmit(s_pSpi, &transaction);
    memcpy(pData, &rxBuffer[1], len);

    return err == ESP_OK ? BME68X_OK : BME68X_E_COM_FAIL;
}

int8_t BME680Driver::write(uint8_t adr, const uint8_t *pData, uint32_t len, void* pIntf) {
    uint8_t txBuffer[len+1];

    txBuffer[0] = adr;
    memcpy(&txBuffer[1], pData, len);

    spi_transaction_t transaction = spi_transaction_t();
    transaction.flags = SPI_TRANS_USE_RXDATA;
    transaction.tx_buffer = txBuffer;
    transaction.length = (len + 1) * 8;
    
    const esp_err_t err = spi_device_polling_transmit(s_pSpi, &transaction);
    return err == ESP_OK ? BME68X_OK : BME68X_E_COM_FAIL;
}

void BME680Driver::delay(uint32_t period, void* pIntf) {
    period = (period + 1000) / 1000; // ceiling to ms

    vTaskDelay(period / portTICK_PERIOD_MS);
}

void BME680Driver::checkApiRslt(const char api_name[], int8_t rslt) {
    switch (rslt) {
        case BME68X_OK:
            break;
        case BME68X_E_NULL_PTR:
            ESP_LOGE("BME680Driver", "API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            ESP_LOGE("BME680Driver", "API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            ESP_LOGE("BME680Driver", "API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            ESP_LOGE("BME680Driver", "API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            ESP_LOGE("BME680Driver", "API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            ESP_LOGW("BME680Driver", "API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            ESP_LOGE("BME680Driver", "API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}