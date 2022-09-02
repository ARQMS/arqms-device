#include "BME680Driver.h"

#include "ApplicationHardwareConfig.h"
#include "BME680Registers.h"
#include "esp_heap_caps.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "bme68x.h"

BME680Driver::BME680Driver() :
    m_pSpi(NULL) {
}

BME680Driver::~BME680Driver() {
}


struct bme68x_dev bme;
int8_t rslt;
struct bme68x_conf conf;
struct bme68x_heatr_conf heatr_conf;
struct bme68x_data data;
uint32_t del_period;
uint8_t n_fields;
uint16_t sample_count = 1;


BME68X_INTF_RET_TYPE bme68x_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t rxBuffer[len+1];
    uint8_t txBuffer[len+1];

    txBuffer[0] = reg_addr;

    spi_transaction_t transaction = {0};
    transaction.tx_buffer = txBuffer;
    transaction.rx_buffer = rxBuffer;
    transaction.length = (len + 1) * 8;

    spi_device_polling_transmit(s_pSpi, &transaction);

    memcpy(reg_data, &rxBuffer[1], len);

    return BME68X_OK;
}

BME68X_INTF_RET_TYPE bme68x_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t txBuffer[len+1];

    txBuffer[0] = reg_addr;
    memcpy(&txBuffer[1], reg_data, len);

    spi_transaction_t transaction = {0};
    transaction.flags = SPI_TRANS_USE_RXDATA;
    transaction.tx_buffer = txBuffer;
    transaction.length = (len + 1) * 8;
    
    spi_device_polling_transmit(s_pSpi, &transaction);

    return BME68X_OK;
}

void bme68x_delay_us(uint32_t period, void *intf_ptr) {
        vTaskDelay(period / 1000 / portTICK_PERIOD_MS );
}

void bme68x_check_rslt(const char api_name[], int8_t rslt) {
    switch (rslt)
    {
        case BME68X_OK:
            break;
        case BME68X_E_NULL_PTR:
            printf("API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            printf("API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            printf("API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            printf("API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}


int8_t bme68x_interface_init(struct bme68x_dev *bme, uint8_t intf) {
    int8_t rslt = BME68X_OK;
    
    printf("SPI Interface\n");
    bme->read = bme68x_spi_read;
    bme->write = bme68x_spi_write;
    bme->intf = BME68X_SPI_INTF;
    bme->delay_us = bme68x_delay_us;
    bme->intf_ptr = NULL;
    bme->amb_temp = 22;

    return rslt;
}



void BME680Driver::initialzie(spi_device_handle_t pSpi) {
    /*m_pSpi = pSpi;

    reset();

    BME680Registers::Id chipId;
    readRegister(chipId);
    if (chipId() != BME680Registers::BME680_ID) {
        ESP_LOGE("BME680", "ChipID: 0x%X invalid. Only support 0x%X", chipId(), BME680Registers::BME680_ID);
        return;
    }

    // // DEMO: The following instructions are given by quick start chapter 3.2.1

    // // start calibration
    // // TODO extract to method. read calibration values

    vTaskDelay(1000/ portTICK_PERIOD_MS);
    setMemPage(0x81);
    for (int i = 0x80; i < 0xFF; i++) {
        uint8_t value = 0;
        bme68x_spi_read(i, &value, 1, NULL);
    }

    BME680Registers::Par_t1_lsb parT1_lsb;
    readRegister(parT1_lsb);
    BME680Registers::Par_t1_msb parT1_msb;
    readRegister(parT1_msb);
    BME680Registers::Par_t2_lsb parT2_lsb;
    readRegister(parT2_lsb);
    BME680Registers::Par_t2_msb parT2_msb;
    readRegister(parT2_msb);
    BME680Registers::Par_t3 parT3;
    readRegister(parT3);
    par_t1 = ((parT1_msb() << 8) | parT1_lsb());
    par_t2 = ((parT2_msb() << 8) | parT2_lsb());
    par_t3 = parT3();
    // end calibration

    ESP_LOGI("BME680", "PG1: par_t1: %i, par_t2: %i, par_t3: %i", par_t1, par_t2, par_t3);

    setMemPage(0x80);

    BME680Registers::Ctrl_hum ctrlHum;
    ctrlHum().osrs_h = BME680Registers::OVERSAMPLING_4;
    writeRegister(ctrlHum);

    BME680Registers::Ctrl_meas ctrlMeas;
    readRegister(ctrlMeas);
    ctrlMeas().osrs_t = BME680Registers::OVERSAMPLING_4;
    ctrlMeas().osrs_p = BME680Registers::OVERSAMPLING_4;
    ctrlMeas().mode = BME680Registers::SLEEP_MODE;
    writeRegister(ctrlMeas);

    BME680Registers::Gas_wait_0 gasWait0;
    gasWait0() = 0x59; // = 100ms heat up
    writeRegister(gasWait0);

    BME680Registers::Res_heat_0 resHeat0;
    resHeat0() = BME680Registers::HEATER_PROFILE_SP_0;
    writeRegister(resHeat0);

    BME680Registers::Ctrl_gas_1 ctrlGas1;
    ctrlGas1().np_conv = BME680Registers::HEATER_PROFILE_SP_0;
    ctrlGas1().run_gas = true;
    writeRegister(ctrlGas1);
    //readTemp();*/

    rslt = bme68x_interface_init(&bme, BME68X_SPI_INTF);
    bme68x_check_rslt("bme68x_interface_init", rslt);

    rslt = bme68x_init(&bme);
    bme68x_check_rslt("bme68x_init", rslt);

    ESP_LOGI("BME680", "par_t1: %i, par_t2: %i, par_t3: %i", bme.calib.par_t1,  bme.calib.par_t2,  bme.calib.par_t3);

    // Check if rslt == BME68X_OK, report or handle if otherwise
    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = BME68X_OS_NONE;
    conf.os_pres = BME68X_OS_NONE;
    conf.os_temp = BME68X_OS_2X;
    rslt = bme68x_set_conf(&conf, &bme);
    bme68x_check_rslt("bme68x_set_conf", rslt);

    // Check if rslt == BME68X_OK, report or handle if otherwise
    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp = 300;
    heatr_conf.heatr_dur = 100;
    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);
    bme68x_check_rslt("bme68x_set_heatr_conf", rslt);

    printf("Sample, Temperature(deg C), Pressure(Pa), Humidity(%%), Gas resistance(ohm), Status\n");
}

void BME680Driver::readTemp() {/*
    BME680Registers::Ctrl_meas ctrlMeas;
    readRegister(ctrlMeas);
    ctrlMeas().mode = BME680Registers::FORCED_MODE;
    writeRegister(ctrlMeas);


    vTaskDelay(1000/ portTICK_PERIOD_MS);
    setMemPage(0x00);
    for (int i = 0x00; i < 0x80; i++) {
        uint8_t value = 0;
        bme68x_spi_read(i, &value, 1, NULL);
    }

    setMemPage(0x80);
    for (int i = 0x00; i < 0x80; i++) {
        uint8_t value = 0;
        bme68x_spi_read(i, &value, 1, NULL);
    }

    vTaskDelay(5000/ portTICK_PERIOD_MS);

    // // read data
    BME680Registers::Temp_lsb tempLsb;
    BME680Registers::Temp_msb tempMsb;
    readRegister(tempLsb);
    readRegister(tempMsb);

    // temperatur 16bit resoltion (for 20bit resolution use xlsb register and IIR filter)
    const float32_t temp_adc = (float32_t) ((tempMsb() << 12) | tempLsb() << 4 );

    float32_t var1 = ((temp_adc / 16384.f) - (par_t1 / 1024.f)) * par_t2;
    float32_t var2 = (((temp_adc / 131072.f) - (par_t1 / 8192.f)) * ((temp_adc / 131072.f) - (par_t1 / 8192.f))) * (par_t3 * 16.f);
    float32_t t_fine = var1 + var2;
    float32_t temp_comp = t_fine / 5120.f;

    ESP_LOGI("BME680", "Temperatur %f (comp: %f)", t_fine, temp_comp);
*/
    rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
    bme68x_check_rslt("bme68x_set_op_mode", rslt);

    // Calculate delay period in microseconds
    del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
    bme.delay_us(del_period, bme.intf_ptr);

    // Check if rslt == BME68X_OK, report or handle if otherwise
    rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
    bme68x_check_rslt("bme68x_get_data", rslt);

    // Check if rslt == BME68X_OK, report or handle if otherwise
    ESP_LOGI("BME680", "%u, %.2f, %.2f, %.2f, %.2f, 0x%x, %d, %d\n",
            sample_count,
            data.temperature,
            data.pressure,
            data.humidity,
            data.gas_resistance,
            data.status,
            data.gas_index,
            data.meas_index);

    vTaskDelay(2000/ portTICK_PERIOD_MS);
}

void BME680Driver::reset() {
    // just ensure correct mem page is cached
    getMemPage();

    BME680Registers::Reset reset = 0xB6;
    writeRegister(reset);
    // wait 10 ms until chip has been rebooted
    vTaskDelay(10 / portTICK_PERIOD_MS);

    // After reset get the memory page
    getMemPage();
}

void BME680Driver::setMemPage(const uint8_t adr) {
    // Datasheet [0] page 27
    //      0x80..0xFF Pg1, spi_mem_page: 1 --> true
    //      0x00..0x7F Pg0, spi_mem_page: 0 --> false
    const bool page = adr > 0x7f;

    if (m_status().spi_mem_page == page) { return; }

    // sync out-dated status register
    // getMemPage();

    m_status().spi_mem_page = page;
    uint8_t status = static_cast<uint8_t>(m_status);
    bme68x_spi_write(m_status.getAddress(), &status, 1, NULL);

    vTaskDelay(5 / portTICK_PERIOD_MS);

    ESP_LOGW("BME680", "Set Active Pg%i", m_status().spi_mem_page ? 1 : 0);
}

bool BME680Driver::getMemPage() {
    uint8_t status = 0;
    bme68x_spi_read(m_status.getAddress(), &status, 1, NULL);
    m_status = status;

    return m_status().spi_mem_page;
}

template<typename R>
void BME680Driver::readRegister(R& reg) {
    setMemPage(reg.getAddress());

    uint8_t result = 0;
    bme68x_spi_read(reg.getAddress(), &result, 1, NULL);
    reg = result;
}

template<typename R>
void BME680Driver::writeRegister(R& reg) {
    setMemPage(reg.getAddress());

    uint8_t result = static_cast<uint8_t>(reg);
    bme68x_spi_write(reg.getAddress(), &result, 1, NULL);
}


// support bulk read. readBulk(const uint8_t startAdr, uint8_t* arr, size_t length = 1)
void BME680Driver::read(const uint8_t adr, uint8_t& value) {
    // spi_device_acquire_bus(m_pSpi, portMAX_DELAY);

    // spi_transaction_t transaction;
    // memset(&transaction, 0, sizeof(transaction));
    // transaction.tx_data[0] = adr;
    // transaction.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    // transaction.length = 16;

    // spi_device_polling_transmit(m_pSpi, &transaction);
    // spi_device_release_bus(m_pSpi);

    // value = *(uint8_t*)&transaction.rx_data[1];

    // ESP_LOGI("Driver", "get adr 0x%X = 0x%X", adr, value);
}

void BME680Driver::write(const uint8_t adr, const uint8_t value) {
    // spi_device_acquire_bus(m_pSpi, portMAX_DELAY);

    // spi_transaction_t transaction;
    // memset(&transaction, 0, sizeof(transaction));
    // transaction.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    // transaction.tx_data[0] = adr;
    // transaction.tx_data[1] = value;
    // transaction.length = 16;

    // ESP_LOGI("Driver", "set adr 0x%X = 0x%X", transaction.tx_data[0], transaction.tx_data[1]);

    // spi_device_polling_transmit(m_pSpi, &transaction);
    // spi_device_release_bus(m_pSpi);
}
