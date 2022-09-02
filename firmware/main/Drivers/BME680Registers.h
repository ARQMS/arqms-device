#ifndef BME680_REGISTER_H_
#define BME680_REGISTER_H_

#include <HumiDevice.Platform/BitField.h>
#include <HumiDevice.Platform/Register.h>

/**
 * The following registers are defined by BME680 Datasheet page 28
 * @see https://github.com/ARQMS/arqms-device/files/9315130/bst-bme680-ds001.pdf
 */
struct BME680Registers {
private:
    union StatusRegister {
        BitField<4> spi_mem_page;
    };

    union ConfigRegister {
        BitField<0>    spi_3w_en;
        BitField<2, 3> filter;
    };

    union CtrlMeasRegister {
        BitField<0, 2> mode;
        BitField<2, 3> osrs_p;
        BitField<5, 3> osrs_t;
    };

    union CtrlHumRegister {
        BitField<0, 3> osrs_h;
        BitField<6>    spi_3w_int_en;
    };

    union CtrlGas1Register {
        BitField<0, 4> np_conv;
        BitField<4>    run_gas;
    };

    union CtrlGas0Register {
        BitField<3>    heat_off;
    };

    union GasRLsbRegister {
        BitField<0, 4> gas_range_r;
        BitField<4>    heat_stab_r;
        BitField<5>    gas_valid_r;
        BitField<6,2>  gas_r;
    };

    union TempXlsbRegister {
        BitField<4, 4> temp_xlsb;
    };

    union PreasXlsbRegister {
        BitField<4, 4> press_xlsb;
    };

    union EasStatus0Register {
        BitField<0, 4> gas_meas_index_0;
        BitField<5>    measuring;
        BitField<6>    gas_measuring;
        BitField<7>    new_data_0;
    };

public:
    typedef Register<0xF3, 8, StatusRegister> Status;
    typedef Register<0x60, 8, uint8_t> Reset;
    typedef Register<0x50, 8, uint8_t> Id;
    typedef Register<0xF5, 8, ConfigRegister> Config;
    typedef Register<0xF4, 8, CtrlMeasRegister> Ctrl_meas;
    typedef Register<0xF2, 8, CtrlHumRegister> Ctrl_hum;
    typedef Register<0xF1, 8, CtrlGas1Register> Ctrl_gas_1;
    typedef Register<0xF0, 8, CtrlGas0Register> Ctrl_gas_0;
    typedef Register<0xE4, 8, uint8_t> Gas_wait_0;
    typedef Register<0xE5, 8, uint8_t> Gas_wait_1;
    typedef Register<0xE6, 8, uint8_t> Gas_wait_2;
    typedef Register<0xE7, 8, uint8_t> Gas_wait_3;
    typedef Register<0xE8, 8, uint8_t> Gas_wait_4;
    typedef Register<0xE9, 8, uint8_t> Gas_wait_5;
    typedef Register<0xEA, 8, uint8_t> Gas_wait_6;
    typedef Register<0xEB, 8, uint8_t> Gas_wait_7;
    typedef Register<0xEC, 8, uint8_t> Gas_wait_8;
    typedef Register<0xED, 8, uint8_t> Gas_wait_9;
    typedef Register<0xDA, 8, uint8_t> Res_heat_0;
    typedef Register<0xDB, 8, uint8_t> Res_heat_1;
    typedef Register<0xDC, 8, uint8_t> Res_heat_2;
    typedef Register<0xDD, 8, uint8_t> Res_heat_3;
    typedef Register<0xDE, 8, uint8_t> Res_heat_4;
    typedef Register<0xDF, 8, uint8_t> Res_heat_5;
    typedef Register<0xE0, 8, uint8_t> Res_heat_6;
    typedef Register<0xE1, 8, uint8_t> Res_heat_7;
    typedef Register<0xE2, 8, uint8_t> Res_heat_8;
    typedef Register<0xE3, 8, uint8_t> Res_heat_9;
    typedef Register<0xD0, 8, uint8_t> Idac_heat_0;
    typedef Register<0xD1, 8, uint8_t> Idac_heat_1;
    typedef Register<0xD2, 8, uint8_t> Idac_heat_2;
    typedef Register<0xD3, 8, uint8_t> Idac_heat_3;
    typedef Register<0xD4, 8, uint8_t> Idac_heat_4;
    typedef Register<0xD5, 8, uint8_t> Idac_heat_5;
    typedef Register<0xD6, 8, uint8_t> Idac_heat_6;
    typedef Register<0xD7, 8, uint8_t> Idac_heat_7;
    typedef Register<0xD8, 8, uint8_t> Idac_heat_8;
    typedef Register<0xD9, 8, uint8_t> Idac_heat_9;
    typedef Register<0xAB, 8, GasRLsbRegister> Gas_r_lsb;
    typedef Register<0xAA, 8, uint8_t> Gas_r_msb;
    typedef Register<0xA6, 8, uint8_t> Hum_lsb;
    typedef Register<0xA5, 8, uint8_t> Hum_msb;
    typedef Register<0xA4, 8, TempXlsbRegister> Temp_xlsb;
    typedef Register<0xA3, 8, uint8_t> Temp_lsb;
    typedef Register<0xA2, 8, uint8_t> Temp_msb;
    typedef Register<0xA1, 8, PreasXlsbRegister> Press_xlsb;
    typedef Register<0xA0, 8, uint8_t> Press_lsb;
    typedef Register<0x9F, 8, uint8_t> Press_msb;
    typedef Register<0x9D, 8, EasStatus0Register> Eas_status_0;

    // factory calibration values. Datasheet page 18
    typedef Register<0xE9, 8, uint8_t> Par_t1_lsb;
    typedef Register<0xEA, 8, uint8_t> Par_t1_msb;
    typedef Register<0x8A, 8, uint8_t> Par_t2_lsb;
    typedef Register<0x8B, 8, uint8_t> Par_t2_msb;
    typedef Register<0x8C, 8, uint8_t> Par_t3;

    // unique chip identifier
    static const uint8_t BME680_ID = 0x61; 

    // Oversampling osrs values. Datasheet page 30
    static const uint8_t OVERSAMPLING_SKIPPED = 0x00;
    static const uint8_t OVERSAMPLING_1       = 0x01;
    static const uint8_t OVERSAMPLING_2       = 0x02;
    static const uint8_t OVERSAMPLING_4       = 0x03;
    static const uint8_t OVERSAMPLING_8       = 0x04;
    static const uint8_t OVERSAMPLING_16      = 0x05;

    // sensor mode. Datasheet page 29
    static const uint8_t SLEEP_MODE           = 0x00;
    static const uint8_t FORCED_MODE          = 0x01;

    // IIR Filter. Datasheet page 31
    static const uint8_t IIR_FILTER_COEF_0    = 0x00;
    static const uint8_t IIR_FILTER_COEF_1    = 0x01;
    static const uint8_t IIR_FILTER_COEF_3    = 0x02;
    static const uint8_t IIR_FILTER_COEF_7    = 0x03;
    static const uint8_t IIR_FILTER_COEF_15   = 0x04;
    static const uint8_t IIR_FILTER_COEF_31   = 0x05;
    static const uint8_t IIR_FILTER_COEF_63   = 0x06;
    static const uint8_t IIR_FILTER_COEF_127  = 0x07;

    // Gas sensor wait time multiplicator factor. Datasheet page 33
    static const uint8_t GAS_WAIT_TIME_MUL_1  = 0x00;
    static const uint8_t GAS_WAIT_TIME_MUL_4  = 0x01;
    static const uint8_t GAS_WAIT_TIME_MUL_16 = 0x02;
    static const uint8_t GAS_WAIT_TIME_MUL_64 = 0x03;

    // Heater profile set-point. Datasheet page 34
    static const uint8_t HEATER_PROFILE_SP_0    = 0x00;
    static const uint8_t HEATER_PROFILE_SP_1    = 0x01;
    static const uint8_t HEATER_PROFILE_SP_2    = 0x02;
    static const uint8_t HEATER_PROFILE_SP_3    = 0x03;
    static const uint8_t HEATER_PROFILE_SP_4    = 0x04;
    static const uint8_t HEATER_PROFILE_SP_5    = 0x05;
    static const uint8_t HEATER_PROFILE_SP_6    = 0x06;
    static const uint8_t HEATER_PROFILE_SP_7    = 0x07;
    static const uint8_t HEATER_PROFILE_SP_8    = 0x08;
    static const uint8_t HEATER_PROFILE_SP_9    = 0x09;

};


#endif // BME680_REGISTER_H_