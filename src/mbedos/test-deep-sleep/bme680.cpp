#include "BME680.h"

// no idea why this is not the same as the PDF
//const double BME680::const_array1[16] = {1,1,1,1,1,0.99,1,0.992,1,1,0.998,0.995,1,0.99,1,1};
//const double BME680::const_array2[16] = {8000000,4000000,2000000,1000000,499500.4995,248262.1648,125000,63004.03226,31281.28128,15625,7812.5,3906.25,1953.125,976.5625,488.28125,244.140625};

const uint64_t BME680::lookup_k1_range[16] = {
    2147483647UL, 2147483647UL, 2147483647UL, 2147483647UL, 2147483647UL,
    2126008810UL, 2147483647UL, 2130303777UL, 2147483647UL, 2147483647UL,
    2143188679UL, 2136746228UL, 2147483647UL, 2126008810UL, 2147483647UL,
    2147483647UL
};

const uint64_t BME680::lookup_k2_range[16] = {
    4096000000UL, 2048000000UL, 1024000000UL, 512000000UL,
    255744255UL, 127110228UL, 64000000UL, 32258064UL, 16016016UL,
    8000000UL, 4000000UL, 2000000UL, 1000000UL, 500000UL, 250000UL,
    125000UL
};

const double BME680::_lookup_k1_range[BME680_GAS_RANGE_RL_LENGTH] = {
    0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.8,
    0.0, 0.0, -0.2, -0.5, 0.0, -1.0, 0.0, 0.0
};

const double BME680::_lookup_k2_range[BME680_GAS_RANGE_RL_LENGTH] = {
    0.0, 0.0, 0.0, 0.0, 0.1, 0.7, 0.0, -0.8,
    -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
};

void BME680::setParallelMode()
{
    //Select oversampling for T, P, H
    setOversamplingTemperature(2);
    setOversamplingPressure(5);
    setOversamplingHumidity(1);
    
    //Select IIR filter for pressure & temperature
    setIIRfilterCoefficient(0);
    
    //Enable gas coversion
    runGasConversion();
    
    //Select heater set-points to be used
    setHeaterProfile(1);
    
    //Set wait time between TPHG submeasurements
    //Set gas_wait_shared<7:0> (time base unit is 0.477ms)
    setGasWaitShared(32, 1);
    
    //Define heater-on times
    //Convert durations to register codes
    //Set gas_wait_x<7:0> (time base unit is ms)
    setGasWaitTime(0,25,4);
    
#ifdef FIXED_POINT_COMPENSATION
    setTargetHeaterResistance(0, convertTemperatureResistanceInt(350,30));
#else
    setTargetHeaterResistance(0, convertTemperatureResistanceDouble(350,30));
#endif

    setMode(2);
}

void BME680::setForcedMode()
{
    //Select oversampling for T, P, H
    setOversamplingTemperature(2);
    setOversamplingPressure(5);
    setOversamplingHumidity(1);

    //Select IIR filter for pressure & temperature
    setIIRfilterCoefficient(0);

    //Enable gas coversion
    runGasConversion();

    //Select heater set-points to be used
    setHeaterProfile(1);

    //Define heater-on times
    //Convert durations to register codes
    //Set gas_wait_x<7:0> (time base unit is ms)
    setGasWaitTime(0,25,4);

#ifdef FIXED_POINT_COMPENSATION
    setTargetHeaterResistance(0, convertTemperatureResistanceInt(350,30));
#else
    setTargetHeaterResistance(0, convertTemperatureResistanceDouble(350,30));
#endif

    //Set mode to sequential mode
    //Set mode<1:0> to 0b01
    setMode(1);
}

void BME680::setSequentialMode()
{
    //Select stand-by time between measurements
    setWakePeriod(1);

    //Select oversampling for T, P, H
    setOversamplingTemperature(2);
    setOversamplingPressure(5);
    setOversamplingHumidity(1);

    //Select IIR filter for pressure & temperature
    setIIRfilterCoefficient(0);

    //Enable gas coversion
    runGasConversion();

    //Select heater set-points to be used
    setHeaterProfile(1);

    //Define heater-on times
    //Convert durations to register codes
    //Set gas_wait_x<7:0> (time base unit is ms)
    setGasWaitTime(0,25,4);

    //Set heater temperatures
    //Convert temperature to register code
    //Set res_heat_x<7:0>

#ifdef FIXED_POINT_COMPENSATION
    setTargetHeaterResistance(0, convertTemperatureResistanceInt(350,30));
#else
    setTargetHeaterResistance(0, convertTemperatureResistanceDouble(350,30));
#endif

    //Set mode to sequential mode
    //Set mode<1:0> to 0b11
    setMode(3);
}

#ifdef FIXED_POINT_COMPENSATION
int32_t BME680::getCompensatedTemperature(int field)
{
    uint32_t v_uncomp_temperature_u32 = getUncompensatedTemp1Data(field);

    int32_t var1 = ((int32_t)v_uncomp_temperature_u32 >> 3) - ((int32_t)(par_T1 << 1));
    int32_t var2 = (var1 * (int32_t) par_T2) >> 11;
    int32_t var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * ((int32_t)(par_T3 << 4))) >> 14;
    t_fine = var2 + var3;
    return ((t_fine * 5) + 128) >> 8;
}

int16_t BME680::getTemperatureInt(int field)
{
    getCompensatedTemperature(field);
    return (((t_fine - 122880) * 25) + 128) >> 8;
}

int32_t BME680::getCompensateHumidity(int field)
{
    uint32_t v_uncomp_humidity_u32 = getUncompensatedHumidityData(field);

    int32_t temp_scaled = (t_fine * 5 + 128) >> 8;
    int32_t var1 = (int32_t)v_uncomp_humidity_u32 -
                   ((int32_t)((int32_t)par_H1 << 4)) -
                   (((temp_scaled * (int32_t)par_H3) /
                     ((int32_t)100)) >> 1);

    int32_t var2 = ((int32_t)par_H2 *
                    (((temp_scaled * (int32_t)par_H4) /
                      ((int32_t)100)) + (((temp_scaled *
                                           ((temp_scaled * (int32_t)par_H5) /
                                            ((int32_t)100))) >> 6) / ((int32_t)100)) + (int32_t)(1 << 14))) >> 10;

    int32_t var3 = var1 * var2;

    int32_t var4 = ((((int32_t)par_H6) << 7) +
                    ((temp_scaled * (int32_t)par_H7) /
                     ((int32_t)100))) >> 4;

    int32_t var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
    int32_t var6 = (var4 * var5) >> 1;

    int32_t humidity_comp = (var3 + var6) >> 12;
    if (humidity_comp > BME680_MAX_HUMIDITY_VALUE)
        humidity_comp = BME680_MAX_HUMIDITY_VALUE;
    else if (humidity_comp < BME680_MIN_HUMIDITY_VALUE)
        humidity_comp = BME680_MIN_HUMIDITY_VALUE;

    return humidity_comp;
}

uint16_t BME680::getHumidityInt(int field)
{
    uint32_t v_x1_u32 = (uint32_t) getCompensateHumidity(field);
    uint16_t v_x2_u32 = (uint16_t)(v_x1_u32 >> 1);
    return v_x2_u32;
}

int32_t BME680::getCompensatePressure(int field)
{
    uint32_t v_uncomp_pressure_u32 = getUncompensatedPressureData(field);

    int32_t var1 = (((int32_t)t_fine) >> 1) - 64000;
    int32_t var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * (int32_t)par_P6) >> 2;
    var2 = var2 + ((var1 * (int32_t)par_P5) << 1);
    var2 = (var2 >> 2) + ((int32_t)par_P4 << 16);
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) *
             ((int32_t)par_P3 << 5)) >> 3) +
           (((int32_t)par_P2 * var1) >> 1);
    var1 = var1 >> 18;
    var1 = ((32768 + var1) * (int32_t)par_P1) >> 15;
    int32_t pressure_comp = 1048576 - v_uncomp_pressure_u32;
    pressure_comp = (int32_t)((pressure_comp - (var2 >> 12)) * ((int32_t)3125));
    int32_t var4 = (1 << 31);
    if (pressure_comp >= var4)
        pressure_comp = ((pressure_comp / (int32_t)var1) << 1);
    else
        pressure_comp = ((pressure_comp << 1) / (int32_t)var1);
    var1 = ((int32_t)par_P9 * (int32_t)(((pressure_comp >> 3) *
                                         (pressure_comp >> 3)) >> 13)) >> 12;
    var2 = ((int32_t)(pressure_comp >> 2) *
            (int32_t)par_P8) >> 13;
    int32_t var3 = ((int32_t)(pressure_comp >> 8) * (int32_t)(pressure_comp >> 8) *
                    (int32_t)(pressure_comp >> 8) *
                    (int32_t)par_P10) >> 17;

    pressure_comp = (int32_t)(pressure_comp) + ((var1 + var2 + var3 +
                    ((int32_t)par_P7 << 7)) >> 4);

    return pressure_comp;
}

uint32_t BME680::getPressureInt(int field)
{
    uint32_t pressure = (uint32_t)getCompensatePressure(field);
    pressure = (uint32_t)(pressure >> 1);
    return pressure;
}

uint8_t BME680::convertTemperatureResistanceInt(uint16_t heater, int16_t ambient)
{
    uint8_t res_heat = 0;


    if ((heater >= BME680_GAS_PROFILE_TEMPERATURE_MIN)
            && (heater <= BME680_GAS_PROFILE_TEMPERATURE_MAX)) {

        int32_t var1 = (((int32_t)ambient * par_GH3) / 10) << 8;
        int32_t var2 = (par_GH1 + 784) *
                       (((((par_GH2 + 154009) *
                           heater * 5) / 100) + 3276800) / 10);
        int32_t var3 = var1 + (var2 >> 1);
        int32_t var4 = (var3 / (res_heat_range + 4));

        int32_t var5 = (131 * res_heat_val) + 65536;

        int32_t res_heat_x100 = (int32_t)(((var4 / var5) - 250) * 34);
        res_heat = (uint8_t) ((res_heat_x100 + 50) / 100);

    }
    return res_heat;
}

int32_t BME680::getCalculateGasInt(int field)
{
    uint8_t gas_range_u8 = getGasResistanceRange(field);
    uint16_t gas_adc_u16 = getUncompensatedGasResistanceData(field);

    int64_t var1 = (int64_t)((1340 + (5 * (int64_t)range_switching_error)) *
                             ((int64_t)lookup_k1_range[gas_range_u8])) >> 16;
    int64_t var2 = (int64_t)((int64_t)gas_adc_u16 << 15) - (int64_t)(1 << 24) + var1;
    int32_t gas_res = (int32_t)(((((int64_t)lookup_k2_range[gas_range_u8] *
                                   (int64_t)var1) >> 9) + (var2 >> 1)) / var2);
    return gas_res;
}

#else
double BME680::getTemperatureDouble(int field)
{
    uint32_t uncom_temperature_u32 = getTemp1Data(field);

    double data1_d  = ((((double)uncom_temperature_u32 / 16384.0)
                        - ((double)par_T1 / 1024.0))
                       * ((double)par_T2));
    /* calculate x2 data */
    double data2_d  = (((((double)uncom_temperature_u32 / 131072.0) -
                         ((double)par_T1 / 8192.0)) *
                        (((double)uncom_temperature_u32 / 131072.0) -
                         ((double)par_T1 / 8192.0))) *
                       ((double)par_T3 * 16.0));
    /* t fine value*/
    t_fine = (int32_t)(data1_d + data2_d);
    /* compensated temperature data*/
    return (data1_d + data2_d) / 5120.0;
}

double BME680::getHumidityDouble(int field)
{
    double comp_temperature = getTemperatureDouble(field);
    uint16_t uncom_humidity_u16 = getHumidityData(field);

    double var1 = (double)((double)uncom_humidity_u16) - (((double)
                  par_H1 * 16.0) +
                  (((double)par_H3 / 2.0)
                   * comp_temperature));

    double var2 = var1 * ((double)(
                              ((double) par_H2 / 262144.0)
                              *(1.0 + (((double)par_H4 / 16384.0)
                                       * comp_temperature) + (((double)par_H5
                                               / 1048576.0) * comp_temperature
                                               * comp_temperature))));
    double var3 = (double) par_H6 / 16384.0;
    double var4 = (double) par_H7 / 2097152.0;

    double humidity_comp = var2 +
                           ((var3 + (var4 * comp_temperature)) * var2 * var2);
    if (humidity_comp > BME680_MAX_HUMIDITY_VALUE)
        humidity_comp = BME680_MAX_HUMIDITY_VALUE;
    else if (humidity_comp < BME680_MIN_HUMIDITY_VALUE)
        humidity_comp = BME680_MIN_HUMIDITY_VALUE;
    return humidity_comp;
}

double BME680::getPressureDouble(int field)
{
    uint32_t uncom_pressure_u32 = getPressureData(field);

    double data1_d = (((double)t_fine / 2.0) - 64000.0);
    double data2_d = data1_d * data1_d * (((double)par_P6) / (131072.0));
    data2_d = data2_d + (data1_d * ((double)par_P5) * 2.0);
    data2_d = (data2_d / 4.0) + (((double)par_P4) * 65536.0);
    data1_d = (((((double)par_P3 * data1_d * data1_d) / 16384.0) + ((double)par_P2 * data1_d)) / 524288.0);
    data1_d = ((1.0 + (data1_d / 32768.0)) * ((double)par_P1));
    double pressure_comp = (1048576.0 - ((double)uncom_pressure_u32));
    /* Avoid exception caused by division by zero */
    if ((int)data1_d != 0) {
        pressure_comp = (((pressure_comp - (data2_d / 4096.0)) * 6250.0) / data1_d);
        data1_d = (((double)par_P9) * pressure_comp * pressure_comp) /  2147483648.0;
        data2_d = pressure_comp * (((double)par_P8) / 32768.0);
        double data3_d = ((pressure_comp / 256.0) * (pressure_comp / 256.0) * (pressure_comp / 256.0) * (par_P10 / 131072.0));
        pressure_comp = (pressure_comp + (data1_d + data2_d + data3_d + ((double)par_P7 * 128.0)) / 16.0);
        return pressure_comp;
    } else
        return 0;
}

double BME680::convertTemperatureResistanceDouble(uint16_t heater, int16_t ambient)
{
    double var1 = 0;
    double var2 = 0;
    double var3 = 0;
    double var4 = 0;
    double var5 = 0;
    double res_heat = 0;

    if ((heater >= BME680_GAS_PROFILE_TEMPERATURE_MIN)
            && (heater <= BME680_GAS_PROFILE_TEMPERATURE_MAX)) {
#ifdef  HEATER_C1_ENABLE
        var1 = (((double)par_GH1 / (16.0)) + 49.0);
        var2 = ((((double)par_GH2 / (32768.0)) * (0.0005)) + 0.00235);
#endif
        var3 = ((double)par_GH3 / (1024.0));
        var4 = (var1 * (1.0 + (var2 * (double)heater)));
        var5 = (var4 + (var3 * (double)ambient));

#ifdef  HEATER_C1_ENABLE
        res_heat = (uint8_t)(3.4 * ((var5 * (4 / (4 + (double)res_heat_range)) * (1/(1 + ((double)res_heat_val * 0.002)))) - 25));
#else
        res_heat = (((var5 * (4.0 / (4.0 + (double)res_heat_range))) - 25.0) * 3.4);
#endif

    }
    return (uint8_t)res_heat;
}

double BME680::getCalculateGasDouble(int field)
{
    uint8_t gas_range_u8 = getGasResistanceRange(field);
    uint16_t gas_adc_u16 = getGasResistanceData(field);
    double gas_res_d = 0;


#ifdef HEATER_C1_ENABLE

    double var1 = 0;
    double var2 = 0;
    double var3 = 0;


    var1 = (1340.0 + (5.0 * range_switching_error));
    var2 = (var1) * (1.0 + _lookup_k1_range[gas_range_u8]/100.0);
    var3 = 1.0 + (_lookup_k2_range[gas_range_u8]/100.0);

    gas_res_d = 1.0 / (double)(var3 * (0.000000125) *
                               (double)(1 << gas_range_u8)
                               * (((((double)gas_adc_u16) - 512.00)/var2) + 1.0));

#else
    gas_res_d = 1.0 / ((0.000000125) * (double)(1 << gas_range_u8) *
                       ((((double)(gas_adc_u16) - 512.00) / 1365.3333) + 1.0));
#endif
    return gas_res_d;
}
#endif



BME680::BME680(PinName sda, PinName scl, bool SDO)
    :_i2c_bus(sda, scl)
{
    if (SDO)
        _addr = 0x77 << 1;
    else _addr = 0x76 << 1;

    _i2c_bus.frequency(FREQUENCY_STANDARD);
}

bool BME680::init()
{
    if (getChipID() != 0x61)
        return false;

    uint8_t cali[41];
    readRegister(0x89, 25);
    memcpy(cali, data, 25);
    readRegister(0xE1, 16);
    memcpy(cali + 25, data, 16);

    /* read temperature calibration*/
    par_T1 = (cali[DIG_T1_MSB_REG] << 8) | cali[DIG_T1_LSB_REG];
    par_T2 = (cali[DIG_T2_MSB_REG] << 8) | cali[DIG_T2_LSB_REG];
    par_T3 = cali[DIG_T3_REG];

    /* read pressure calibration*/
    par_P1 = (cali[DIG_P1_MSB_REG] << 8) | cali[DIG_P1_LSB_REG];
    par_P2 = (cali[DIG_P2_MSB_REG] << 8) | cali[DIG_P2_LSB_REG];
    par_P3 = cali[DIG_P3_REG];
    par_P4 = (cali[DIG_P4_MSB_REG] << 8) | cali[DIG_P4_LSB_REG];
    par_P5 = (cali[DIG_P5_MSB_REG] << 8) | cali[DIG_P5_LSB_REG];
    par_P6 = cali[DIG_P6_REG];
    par_P7 = cali[DIG_P7_REG];
    par_P8 = (cali[DIG_P8_MSB_REG] << 8) | cali[DIG_P8_LSB_REG];
    par_P9 = (cali[DIG_P9_MSB_REG] << 8) | cali[DIG_P9_LSB_REG];
    par_P10 = cali[DIG_P10_REG];

    /* read humidity calibration*/
    par_H1 = (cali[DIG_H1_MSB_REG] << 4) | (cali[DIG_H1_LSB_REG] & BME680_BIT_MASK_H1_DATA);
    par_H2 = (cali[DIG_H2_MSB_REG] << 4) | (cali[DIG_H2_LSB_REG] >> 4);
    par_H3 = cali[DIG_H3_REG];
    par_H4 = cali[DIG_H4_REG];
    par_H5 = cali[DIG_H5_REG];
    par_H6 = cali[DIG_H6_REG];
    par_H7 = cali[DIG_H7_REG];

    /* read gas calibration*/
    par_GH1 = cali[DIG_GH1_REG];
    par_GH2 = (cali[DIG_GH2_MSB_REG] <<8) | cali[DIG_GH2_LSB_REG];
    par_GH3 = cali[DIG_GH3_REG];

    /**<resistance calculation*/
    readRegister(0x02);
    res_heat_range = (data[0] >> 4) & 0x03;

    /**<correction factor*/
    readRegister(0x00);
    res_heat_val = data[0];

    /**<range switching error*/
    readRegister(0x04);
    range_switching_error = (data[0] & 0xF0) >> 4;
    /*
       uint16_t BME680::getParG1()
    {
       readRegister(0xEB, 2);
       return (data[1] << 8) | data[0];
    }

    uint8_t BME680::getParG2()
    {
       readRegister(0xED);
       return data[0];
    }

    uint8_t BME680::getParG3()
    {
       readRegister(0xEE);
       return data[0];
    }
    */
    return true;
}

uint32_t BME680::getUncompensatedPressureData(int field)
{
    readRegister(0x1F + field * 0x11, 3);
    return (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
}

uint32_t BME680::getUncompensatedTemp1Data(int field)
{
    readRegister(0x22 + field * 0x11, 3);
    return (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
}

uint32_t BME680::getUncompensatedHumidityData(int field)
{
    readRegister(0x25 + field * 0x11, 2);
    return (data[0] << 8) | data[1];
}

uint16_t BME680::getUncompensatedGasResistanceData(int field)
{
    readRegister(0x2A + field * 0x11, 2);
    return (data[0] << 2) | (data[1] >> 6);
}

uint8_t BME680::getGasResistanceRange(int field)
{
    readRegister(0x2B + field * 0x11);
    return data[0] & 0x0F;
}

bool BME680::isNewData(int field)
{
    readRegister(0x1D + field * 0x11);
    return (data[0] & 0x80) == 0x80 ? true : false;
}

bool BME680::isGasMeasuring(int field)
{
    readRegister(0x1D + field * 0x11);
    return (data[0] & 0x40) == 0x40 ? true : false;
}

bool BME680::isMeasuring(int field)
{
    readRegister(0x1D + field * 0x11);
    return (data[0] & 0x20) == 0x20 ? true : false;
}

int BME680::getGasMeasurementIndex(int field)
{
    readRegister(0x1D + field * 0x11);
    return data[0] & 0x0F;
}

int BME680::getSubMeasurementIndex(int field)
{
    readRegister(0x1E + field * 0x11);
    return data[0];
}

bool BME680::isGasValid(int field)
{
    readRegister(0x2B + field * 0x11);
    return (data[0] & 0x20) == 0x20 ? true : false;
}

bool BME680::isHeaterStable(int field)
{
    readRegister(0x2B + field * 0x11);
    return (data[0] & 0x10) == 0x10 ? true : false;
}

uint8_t BME680::getHeaterCurrent(int setPoint)
{
    readRegister(0x50 + setPoint);
    return data[0] >> 1;
}

void BME680::setHeaterCurrent(int setPoint, uint8_t value)
{
    writeRegister(0x50 + setPoint, value << 1);
}

int8_t BME680::getTargetHeaterResistance(int setPoint)
{
    readRegister(0x5A + setPoint);
    return data[0];
}

void BME680::setTargetHeaterResistance(int setPoint, int8_t value)
{
    writeRegister(0x5A + setPoint, value);
}

int BME680::getGasWaitTime(int setPoint)
{
    readRegister(0x64 + setPoint);
    return (data[0] & 0x3F) * (data[0] >> 6);
}

void BME680::setGasWaitTime(int setPoint, int time, int multiplication)
{
    writeRegister(0x64 + setPoint, (multiplication << 6) | (time & 0x3F));
}

int BME680::getGasWaitShared()
{
    readRegister(0x6E);
    return (data[0] & 0x1F) * (data[0] >> 6);
}

void BME680::setGasWaitShared(int time, int multiplication)
{
    writeRegister(0x6E, (multiplication << 6) | (time & 0x1F));
}

void BME680::setHeaterOff()
{
    readRegister(0x70);
    data[0] |= 0x08;
    writeRegister(0x70, data[0]);
}

int BME680::getHeaterProfile()
{
    readRegister(0x70);
    return data[0] &= 0x08;
}

void BME680::setHeaterProfile(int vlaue)
{
    readRegister(0x71);
    data[0] &= 0xF0;
    data[0] |= vlaue & 0x0F;
    writeRegister(0x71, data[0]);
}

void BME680::runGasConversion()
{
    readRegister(0x71);
    data[0] |= 0x10;
    writeRegister(0x71, data[0]);
}

float BME680::getWakePeriod()
{
    readRegister(0x71);
    int temp = (data[0] & 0x80) >> 4;
    readRegister(0x75);
    temp |= data[0] >> 5;

    switch(temp) {
        case 0:
            return 0.59f;
        case 1:
            return 62.5f;
        case 2:
            return 125;
        case 3:
            return 250;
        case 4:
            return 500;
        case 5:
            return 1000;
        case 6:
            return 10;
        case 7:
            return 20;
        default:
            return 0;
    }
}

void  BME680::setWakePeriod(int value)
{
    readRegister(0x71);
    data[0] = (data[0] & 0x7F) | ((value & 0x0F) >> 3);
    writeRegister(0x71, data[0]);

    readRegister(0x75);
    data[0] = (data[0] & 0x1F) | ((value & 0x07) << 5);
    writeRegister(0x75, data[0]);
}

int BME680::getOversamplingHumidity()
{
    readRegister(0x72);
    switch (data[0] & 0x07) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 8;
        case 5:
            return 16;
    }

    return 0;
}

void BME680::setOversamplingHumidity(int value)
{
    readRegister(0x72);
    data[0] = (data[0] & 0xF8) | (value & 0x07);
    writeRegister(0x72, data[0]);
}

int BME680::getOversamplingPressure()
{
    readRegister(0x74);
    switch ((data[0] & 0x1C) >> 2) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 8;
        case 5:
            return 16;
    }

    return 0;
}

void BME680::setOversamplingPressure(int value)
{
    readRegister(0x74);
    data[0] = (data[0] & 0xE3) | ((value & 0x07) << 2);
    writeRegister(0x74, data[0]);
}

int BME680::getOversamplingTemperature()
{
    readRegister(0x74);
    switch ((data[0] & 0xE0) >> 5) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 8;
        case 5:
            return 16;
    }

    return 0;
}

void BME680::setOversamplingTemperature(int value)
{
    readRegister(0x74);
    data[0] = (data[0] & 0x1F) | ((value & 0x07) << 5);
    writeRegister(0x74, data[0]);
}

int BME680::getIIRfilterCoefficient()
{
    readRegister(0x75);
    switch ((data[0] & 0x1C) >> 2) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 3;
        case 3:
            return 7;
        case 4:
            return 15;
        case 5:
            return 31;
        case 6:
            return 63;
        case 7:
            return 127;
    }
    return 0;
}

void BME680::setIIRfilterCoefficient(int value)
{
    readRegister(0x75);
    data[0] = (data[0] & 0xE3) | ((value & 0x07) << 2);
    writeRegister(0x75, data[0]);
}

int BME680::getMode()
{
    readRegister(0x74);
    return data[0] & 0x03;
}

void BME680::setMode(int mode)
{
    readRegister(0x74);
    data[0] = (data[0] & 0xFC) | (mode & 0x03);
    writeRegister(0x74, data[0]);
}

int BME680::getChipID()
{
    readRegister(0xD0);
    return data[0];
}

// void BME680::readRegister(int reg, int size)
// {
//     uint8_t reg_8bit = reg;

//     _i2c_bus.write ( _addr, (char*)&reg_8bit, 1, true );
//     _i2c_bus.read  ( _addr, (char*)&data[0], size );

// }

void BME680::readRegister(int reg, int size)
{
    _i2c_bus.start();
    _i2c_bus.write(_addr);
    _i2c_bus.write(reg);
    _i2c_bus.start();
    _i2c_bus.write(_addr | 0x01);
    int i = 0;
    for (; i< size -1; i++)
        data[i] = _i2c_bus.read(1);
    data[i] = _i2c_bus.read(0);
    _i2c_bus.stop();
}

void BME680::writeRegister(int reg, int value)
{
    _i2c_bus.start();
    _i2c_bus.write(_addr);
    _i2c_bus.write(reg);
    _i2c_bus.write(value);
    _i2c_bus.stop();
}