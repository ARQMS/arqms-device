#include "BME680Driver.h"

#include "ApplicationHardwareConfig.h"
#include "BME680Registers.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

BME680Driver::BME680Driver() :
    m_pSpi(NULL) {
}

BME680Driver::~BME680Driver() {
}

void BME680Driver::initialzie(spi_device_handle_t pSpi) {
    m_pSpi = pSpi;

    reset();

    BME680Registers::Id chipId;
    readRegister(chipId);
    if (chipId() != BME680Registers::BME680_ID) {
        ESP_LOGE("BME680", "ChipID: 0x%X invalid. Only support 0x%X", chipId(), BME680Registers::BME680_ID);
        return;
    }

    // DEMO: The following instructions are given by quick start chapter 3.2.1

    // start calibration
    // TODO extract to method. read calibration values
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
    const float32_t par_t1 = (float32_t)((parT1_msb() << 8) | parT1_lsb());
    const float32_t par_t2 = (float32_t)((parT2_msb() << 8) | parT2_lsb());
    const float32_t par_t3 = (float32_t)parT3();
    // end calibration


    BME680Registers::Ctrl_hum ctrlHum;
    ctrlHum().osrs_h = BME680Registers::OVERSAMPLING_2;
    writeRegister(ctrlHum);

    BME680Registers::Ctrl_meas ctrlMeas;
    ctrlMeas().osrs_t = BME680Registers::OVERSAMPLING_2;
    ctrlMeas().osrs_p = BME680Registers::OVERSAMPLING_2;
    writeRegister(ctrlMeas);

    BME680Registers::Gas_wait_0 gasWait0;
    gasWait0() = 0x59; // 100ms heat up
    writeRegister(gasWait0);

    BME680Registers::Res_heat_0 resHeat0;
    resHeat0() = BME680Registers::HEATER_PROFILE_SP_0;
    writeRegister(resHeat0);

    BME680Registers::Ctrl_gas_1 ctrlGas1;
    ctrlGas1().np_conv = BME680Registers::HEATER_PROFILE_SP_0;
    ctrlGas1().run_gas = true;
    writeRegister(ctrlGas1);

    ctrlMeas().mode = BME680Registers::FORCED_MODE;
    writeRegister(ctrlMeas);

    vTaskDelay(500/ portTICK_PERIOD_MS);

    // read data
    BME680Registers::Temp_lsb tempLsb;
    BME680Registers::Temp_msb tempMsb;
    readRegister(tempLsb);
    readRegister(tempMsb);

    // temperatur 16bit resoltion (for 20bit resolution use xlsb register)
    const float32_t temp_adc = (float32_t)((tempMsb() << 8) | tempLsb());

    float32_t var1 = ((temp_adc / 16384.0) - (par_t1 / 1024.0)) * par_t2;
    float32_t var2 = (((temp_adc / 131072.0) - (par_t1 / 8192.0)) * ((temp_adc / 131072.0) - (par_t1 / 8192.0))) * (par_t3 * 16.0);
    float32_t t_fine = var1 + var2;
    float32_t temp_comp = t_fine / 5120.0;

    ESP_LOGI("BME680", "Temperatur %f (comp: %f)", t_fine, temp_comp);
}

void BME680Driver::reset() {
    getMemPage();

    BME680Registers::Reset reset;
    reset() = 0xB6;
    writeRegister(reset);
    // wait 5 ms until chip has been rebooted
    vTaskDelay(5 / portTICK_PERIOD_MS);

    getMemPage();
}

template<typename R>
void BME680Driver::readRegister(R& reg) {
    setMemPage(reg.getAddress());
    uint8_t result = 0;
    read(reg.getAddress(), result);
    reg = result;
}

template<typename R>
void BME680Driver::writeRegister(R& reg) {
    setMemPage(reg.getAddress());
    write(reg.getAddress(), static_cast<uint8_t>(reg));
}

void BME680Driver::setMemPage(const uint8_t adr) {
    // 0x80..0xFF spi_mem_page: 0
    // 0x00..0x7F spi_mem_page: 1
    const bool page = adr < 0x7f;

    ESP_LOGI("BME680", "MemPage 0x%X for adr 0x%X", m_status, adr);

    if (m_status().spi_mem_page == page) { return; }

    m_status().spi_mem_page = page;
    write(m_status.getAddress(), static_cast<uint8_t>(m_status));

    ESP_LOGI("BME680", "Current Page (new): 0x%X", m_status);
}

bool BME680Driver::getMemPage() {
    uint8_t status = 0;
    read(m_status.getAddress(), status);
    m_status = status;

    return m_status().spi_mem_page;
}

void BME680Driver::read(const uint8_t adr, uint8_t& value) {
    spi_device_acquire_bus(m_pSpi, portMAX_DELAY);

    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));
    transaction.tx_data[0] = 0x80 | adr;
    transaction.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    transaction.length = 16;

    spi_device_polling_transmit(m_pSpi, &transaction);
    spi_device_release_bus(m_pSpi);

    ESP_LOGI("Driver", "get adr 0x%X = 0x%X", adr, *(uint8_t*)&transaction.rx_data[1]);

    value = *(uint8_t*)&transaction.rx_data[1];
}

void BME680Driver::write(const uint8_t adr, const uint8_t value) {
    spi_device_acquire_bus(m_pSpi, portMAX_DELAY);

    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));
    transaction.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    transaction.tx_data[0] = adr;
    transaction.tx_data[1] = value;
    transaction.length = 16;

    ESP_LOGI("Driver", "set adr 0x%X = 0x%X", transaction.tx_data[0], transaction.tx_data[1]);

    spi_device_polling_transmit(m_pSpi, &transaction);
    spi_device_release_bus(m_pSpi);
}
