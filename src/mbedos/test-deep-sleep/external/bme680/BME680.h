#ifndef UK_AC_HERTS_SMARTLAB_BME680
#define UK_AC_HERTS_SMARTLAB_BME680

#include "mbed.h"
#include "stdint.h"

/*
* Use below macro for fixed Point Calculation
* else Floating Point calculation will be used
*/
#define FIXED_POINT_COMPENSATION

// no idea what it is for
//#define HEATER_C1_ENABLE

// Sensor Specific constants */
#define BME680_SLEEP_MODE               (0x00)
#define BME680_FORCED_MODE              (0x01)
#define BME680_PARALLEL_MODE            (0x02)
#define BME680_SEQUENTIAL_MODE          (0x03)
#define BME680_GAS_PROFILE_TEMPERATURE_MIN  (200)
#define BME680_GAS_PROFILE_TEMPERATURE_MAX  (400)
#define BME680_GAS_RANGE_RL_LENGTH      (16)
#define BME680_SIGN_BIT_MASK            (0x08)

#ifdef FIXED_POINT_COMPENSATION
//< Multiply by 1000, In order to convert float value into fixed point
#define BME680_MAX_HUMIDITY_VALUE       (102400)
#define BME680_MIN_HUMIDITY_VALUE       (0)
#else
#define BME680_MAX_HUMIDITY_VALUE       (double)(100.0)
#define BME680_MIN_HUMIDITY_VALUE       (double)(0.0)
#endif

/**
* !! MUST CALL init() FIRST !!
* read the chip id and calibration data of the BME680 sensor
* BME680 integrated environmental sensor. This API supports FIXED and FLOATING compenstion.
* By default it supports FIXED, to use FLOATING user need to disable "FIXED_POINT_COMPENSATION" in the BME680.h file.
*/
class BME680
{
private:
    static const int FREQUENCY_STANDARD = 100000;
    static const int FREQUENCY_FULL = 400000;
    static const int FREQUENCY_FAST = 1000000;
    static const int FREQUENCY_HIGH = 3200000;

    I2C _i2c_bus;
    int _addr;
    uint8_t data[30];

    //static const double const_array1[];
    //static const double const_array2[];
    static const uint64_t lookup_k1_range[];
    static const uint64_t lookup_k2_range[];
    static const double _lookup_k1_range[];
    static const double _lookup_k2_range[];

    /* For Calibration Data*/
    static const int DIG_T2_LSB_REG = 1;
    static const int DIG_T2_MSB_REG = 2;
    static const int DIG_T3_REG = 3;
    static const int DIG_P1_LSB_REG = 5;
    static const int DIG_P1_MSB_REG = 6;
    static const int DIG_P2_LSB_REG = 7;
    static const int DIG_P2_MSB_REG = 8;
    static const int DIG_P3_REG = 9;
    static const int DIG_P4_LSB_REG = 11;
    static const int DIG_P4_MSB_REG = 12;
    static const int DIG_P5_LSB_REG = 13;
    static const int DIG_P5_MSB_REG = 14;
    static const int DIG_P7_REG = 15;
    static const int DIG_P6_REG = 16;
    static const int DIG_P8_LSB_REG = 19;
    static const int DIG_P8_MSB_REG = 20;
    static const int DIG_P9_LSB_REG = 21;
    static const int DIG_P9_MSB_REG = 22;
    static const int DIG_P10_REG = 23;
    static const int DIG_H2_MSB_REG = 25;
    static const int DIG_H2_LSB_REG = 26;
    static const int DIG_H1_LSB_REG = 26;
    static const int DIG_H1_MSB_REG  = 27;
    static const int DIG_H3_REG = 28;
    static const int DIG_H4_REG = 29;
    static const int DIG_H5_REG = 30;
    static const int DIG_H6_REG = 31;
    static const int DIG_H7_REG = 32;
    static const int DIG_T1_LSB_REG = 33;
    static const int DIG_T1_MSB_REG = 34;
    static const int DIG_GH2_LSB_REG = 35;
    static const int DIG_GH2_MSB_REG = 36;
    static const int DIG_GH1_REG = 37;
    static const int DIG_GH3_REG = 38;

    static const int BME680_BIT_MASK_H1_DATA = 0x0F;

    int8_t  par_T3;/**<calibration T3 data*/
    int8_t  par_P3;/**<calibration P3 data*/
    int8_t  par_P6;/**<calibration P6 data*/
    int8_t  par_P7;/**<calibration P7 data*/
    uint8_t  par_P10;/**<calibration P10 data*/
    int8_t  par_H3;/**<calibration H3 data*/
    int8_t  par_H4;/**<calibration H4 data*/
    int8_t  par_H5;/**<calibration H5 data*/
    uint8_t  par_H6;/**<calibration H6 data*/
    int8_t  par_H7;/**<calibration H7 data*/
    int8_t  par_GH1;/**<calibration GH1 data*/
    uint8_t  res_heat_range;/**<resistance calculation*/
    int8_t  res_heat_val; /**<correction factor*/
    int8_t  range_switching_error;/**<range switching error*/
    int16_t par_GH2;/**<calibration GH2 data*/
    uint16_t par_T1;/**<calibration T1 data*/
    int16_t par_T2;/**<calibration T2 data*/
    uint16_t par_P1;/**<calibration P1 data*/
    int16_t par_P2;/**<calibration P2 data*/
    int16_t par_P4;/**<calibration P4 data*/
    int16_t par_P5;/**<calibration P5 data*/
    int16_t par_P8;/**<calibration P8 data*/
    int16_t par_P9;/**<calibration P9 data*/
    uint16_t par_H1;/**<calibration H1 data*/
    uint16_t par_H2;/**<calibration H2 data*/
    int32_t t_fine;/**<calibration T_FINE data*/
    int8_t  par_GH3;/**<calibration GH3 data*/

    void readRegister(int reg, int size = 1);

    void writeRegister(int reg, int value);

public:

    /**
    * TPHG measurements are performed.
    * continuously until mode change.
    * Between each cycle, the sensor enters stand-by for a period of time according to the odr<3:0> control register.
    * Gas sensor heater only operates during gas sub-measurement.
    * 100 ms gas wait time, T:X2, P:X16, H:X1
    */
    void setSequentialMode();

    /**
    * Single TPHG cycle is performed.
    * Sensor automatically returns to sleep mode afterwards.
    * Gas sensor heater only operates during gas sub-measureme.
    */
    void setForcedMode();

    /**
    * TPHG measurements are performed continuously until mode change.
    * No stand-by occurs between consecutive TPHG cycles.
    * Gas sensor heater operates in parallel with TPH measurements.
    */
    void setParallelMode();

    /*
    * @param sda I2C sda signal
    * @param scl I2C scl signal
    * @param SDO Slave address LSB (High->true, Low->false)
    */
    BME680(PinName sda, PinName scl, bool SDO);

    /**
    * !! MUST CALL THIS FIRST !!
    * read the chip id and calibration data of the BME680 sensor
    */
    bool init();
    // DATA #########################################################################

#ifdef FIXED_POINT_COMPENSATION
    /**
    *  This function is used to convert the uncompensated
    *  temperature data to compensated temperature data using
    *  compensation formula(integer version)
    *  @note Returns the value in 0.01 degree Centigrade
    *  Output value of "5123" equals 51.23 DegC.
    *
    * @param field 0-2
    *
    *  @return Returns the compensated temperature data
    *
    */
    int32_t getCompensatedTemperature(int field = 0);

    /**
    * Reads actual temperature from uncompensated temperature
    * @note Returns the value with 500LSB/DegC centred around 24 DegC
    * output value of "5123" equals(5123/500)+24 = 34.246DegC
    *
    *
    *  @param v_uncomp_temperature_u32: value of uncompensated temperature
    *  @param bme680: structure pointer.
    *
    *
    *  @return Return the actual temperature as s16 output
    *
    */
    int16_t getTemperatureInt(int field = 0);

    /**
    *  @brief This function is used to convert the uncompensated
    *  humidity data to compensated humidity data using
    *  compensation formula(integer version)
    *
    *  @note Returns the value in %rH as unsigned 32bit integer
    *  in Q22.10 format(22 integer 10 fractional bits).
    *  @note An output value of 42313
    *  represents 42313 / 1024 = 41.321 %rH
    *
    *
    *
    *  @param  v_uncomp_humidity_u32: value of uncompensated humidity
    *  @param bme680: structure pointer.
    *
    *  @return Return the compensated humidity data
    *
    */
    int32_t getCompensateHumidity(int field = 0);

    /**
    * @brief Reads actual humidity from uncompensated humidity
    * @note Returns the value in %rH as unsigned 16bit integer
    * @note An output value of 42313
    * represents 42313/512 = 82.643 %rH
    *
    *
    *
    *  @param v_uncomp_humidity_u32: value of uncompensated humidity
    *  @param bme680: structure pointer.
    *
    *  @return Return the actual relative humidity output as u16
    *
    */
    uint16_t getHumidityInt(int field = 0);

    /**
    * @brief This function is used to convert the uncompensated
    *  pressure data to compensated pressure data data using
    *  compensation formula(integer version)
    *
    * @note Returns the value in Pascal(Pa)
    * Output value of "96386" equals 96386 Pa =
    * 963.86 hPa = 963.86 millibar
    *
    *
    *
    *  @param v_uncomp_pressure_u32 : value of uncompensated pressure
    *  @param bme680: structure pointer.
    *
    *  @return Return the compensated pressure data
    *
    */
    int32_t getCompensatePressure(int field = 0);

    /**
     * @brief Reads actual pressure from uncompensated pressure
     * @note Returns the value in Pa.
     * @note Output value of "12337434"
     * @note represents 12337434 / 128 = 96386.2 Pa = 963.862 hPa
     *
     *
     *
     *  @param v_uncomp_pressure_u32 : value of uncompensated pressure
     *  @param bme680: structure pointer.
     *
     *  @return the actual pressure in u32
     *
    */
    uint32_t getPressureInt(int field = 0);

    /**
     *  @brief This function is used to convert temperature to resistance
     *  using the integer compensation formula
     *
     *  @param heater_temp_u16: The value of heater temperature
     *  @param ambient_temp_s16: The value of ambient temperature
     *  @param bme680: structure pointer.
     *
     *  @return calculated resistance from temperature
     *
     *
     *
    */
    uint8_t convertTemperatureResistanceInt(uint16_t heater, int16_t ambient);


    /**
     *  @brief This function is used to convert uncompensated gas data to
     *  compensated gas data using compensation formula(integer version)
     *
     *  @param gas_adc_u16: The value of gas resistance calculated
     *       using temperature
     *  @param gas_range_u8: The value of gas range form register value
     *  @param bme680: structure pointer.
     *
     *  @return calculated compensated gas from compensation formula
     *  @retval compensated gas data
     *
     *
    */
    int32_t getCalculateGasInt(int field = 0);

#else
    /**
    * This function used to convert temperature data
    * to uncompensated temperature data using compensation formula
    * @note returns the value in Degree centigrade
    * @note Output value of "51.23" equals 51.23 DegC.
    * @param field 0-2
    * @return  Return the actual temperature in floating point
    */
    double getTemperatureDouble(int field = 0);

    /**
    * @brief This function is used to convert the uncompensated
    *  humidity data to compensated humidity data data using
    *  compensation formula
    * @note returns the value in relative humidity (%rH)
    * @note Output value of "42.12" equals 42.12 %rH
    *
    *  @param uncom_humidity_u16 : value of uncompensated humidity
    *  @param comp_temperature   : value of compensated temperature
    *  @param bme680: structure pointer.
    *
    *
    *  @return Return the compensated humidity data in floating point
    *
    */
    double getHumidityDouble(int field = 0);

    /**
     * @brief This function is used to convert the uncompensated
     * pressure data to compensated data using compensation formula
     * @note Returns pressure in Pa as double.
     * @note Output value of "96386.2"
     * equals 96386.2 Pa = 963.862 hPa.
     *
     *
     *  @param uncom_pressure_u32 : value of uncompensated pressure
     *  @param bme680: structure pointer.
     *
     *  @return  Return the compensated pressure data in floating point
     *
    */
    double getPressureDouble(int field = 0);

    /**
     *  @brief This function is used to convert temperature to resistance
     *  using the compensation formula
     *
     *  @param heater_temp_u16: The value of heater temperature
     *  @param ambient_temp_s16: The value of ambient temperature
     *  @param bme680: structure pointer.
     *
     *  @return calculated resistance from temperature
     *
     *
     *
    */
    double convertTemperatureResistanceDouble(uint16_t heater, int16_t ambient);

    /**
     *  @brief This function is used to convert uncompensated gas data to
     *  compensated gas data using compensation formula
     *
     *  @param gas_adc_u16: The value of gas resistance calculated
     *       using temperature
     *  @param gas_range_u8: The value of gas range form register value
     *  @param bme680: structure pointer.
     *
     *  @return calculated compensated gas from compensation formula
     *  @retval compensated gas
     *
     *
    */
    double getCalculateGasDouble(int field = 0);
#endif


    /**
    * [press_msb] [press_lsb] [press_xlsb]
    * Pressure, temperature, humidity and gas data of BME680 are stored in 3 data field registers
    * named field0, field1, and field2. The data fields are updated sequentially and always results of
    * the three latest measurements are available for the user; if the last but one conversion was written
    * to field number k, the current conversion results are written to field with number (k+1) mod 3. All
    * data outputs from data fields are buffered using shadowing registers to ensure keeping stable
    * data if update of the data registers comes simultaneously with serial interface reading out.
    * Note: Only field0 will be updated in forced mode
    * @param field 0-2
    */
    uint32_t getUncompensatedPressureData(int field = 0);

    /**
    * [temp1_msb] [temp1_lsb] [temp1_xlsb]
    * Pressure, temperature, humidity and gas data of BME680 are stored in 3 data field registers
    * named field0, field1, and field2. The data fields are updated sequentially and always results of
    * the three latest measurements are available for the user; if the last but one conversion was written
    * to field number k, the current conversion results are written to field with number (k+1) mod 3. All
    * data outputs from data fields are buffered using shadowing registers to ensure keeping stable
    * data if update of the data registers comes simultaneously with serial interface reading out.
    * Note: Only field0 will be updated in forced mode
    * @param field 0-2
    */
    uint32_t getUncompensatedTemp1Data(int field = 0);

    /**
    * [hum_msb] [hum_lsb]
    * Pressure, temperature, humidity and gas data of BME680 are stored in 3 data field registers
    * named field0, field1, and field2. The data fields are updated sequentially and always results of
    * the three latest measurements are available for the user; if the last but one conversion was written
    * to field number k, the current conversion results are written to field with number (k+1) mod 3. All
    * data outputs from data fields are buffered using shadowing registers to ensure keeping stable
    * data if update of the data registers comes simultaneously with serial interface reading out.
    * Note: Only field0 will be updated in forced mode
    * @param field 0-2
    */
    uint32_t getUncompensatedHumidityData(int field = 0);

    /**
    * [gas_rl]
    * Pressure, temperature, humidity and gas data of BME680 are stored in 3 data field registers
    * named field0, field1, and field2. The data fields are updated sequentially and always results of
    * the three latest measurements are available for the user; if the last but one conversion was written
    * to field number k, the current conversion results are written to field with number (k+1) mod 3. All
    * data outputs from data fields are buffered using shadowing registers to ensure keeping stable
    * data if update of the data registers comes simultaneously with serial interface reading out.
    * Note: Only field0 will be updated in forced mode
    * @param field 0-2
    */
    uint16_t getUncompensatedGasResistanceData(int field = 0);

    /**
    * [gas_range_rl]
    * Pressure, temperature, humidity and gas data of BME680 are stored in 3 data field registers
    * named field0, field1, and field2. The data fields are updated sequentially and always results of
    * the three latest measurements are available for the user; if the last but one conversion was written
    * to field number k, the current conversion results are written to field with number (k+1) mod 3. All
    * data outputs from data fields are buffered using shadowing registers to ensure keeping stable
    * data if update of the data registers comes simultaneously with serial interface reading out.
    * Contains ADC range of measured gas resistance
    * Note: Only field0 will be updated in forced mode
    * @param field 0-2
    */
    uint8_t getGasResistanceRange(int field = 0);


    // STATUS #########################################################################

    /**
    * [new_data_x]
    * The measured data are stored into the output data registers at the end of each TPHG conversion
    * phase along with status flags and index of measurement. The part of the register map for output
    * data storage is composed of 3 data fields (TPHG data field0|1|2)) keeping results from the last 3
    * measurements. Availability of new (yet unread) results is indicated by new_data_0|1|2 flags.
    * @param field 0-2
    */
    bool isNewData(int field = 0);

    /**
    * [gas_measuring]
    * Measuring bit is set to “1‟ only during gas measurements, goes to “0‟ as soon as measurement
    * is completed and data transferred to data registers. The registers storing the configuration values
    * for the measurement (gas_wait_shared, gas_wait_x, res_heat_x, idac_heat_x, image registers)
    * should not be changed when the device is measuring.
    * @param field 0-2
    */
    bool isGasMeasuring(int field = 0);

    /**
    * [measuring]
    * Measuring status will be set to ‘1’ whenever a conversion (Pressure, Temperature, humidity &
    * gas) is running and back to ‘0’ when the results have been transferred to the data registers.
    * @param field 0-2
    */
    bool isMeasuring(int field = 0);

    /**
    * [gas_meas_index_x]
    * User can program a sequence of up to 10 conversions by setting nb_conv<3:0>. Each conversion
    * has its own heater resistance target but 3 field registers to store conversion results. The actual
    * gas conversion number in the measurement sequence (up to 10 conversions numbered from 0
    * to 9) is stored in gas_meas_index register.
    * @param field 0-2
    */
    int getGasMeasurementIndex(int field = 0);

    /**
    * [sub_meas_index_x]
    * sub_meas_index_x registers form “virtual time sensor” and contain a snapshot of the internal 8
    * bit conversion counter. Conversion counter is incremented with each TPHG conversion; the
    * counter thus contains the number of conversions modulo 256 executed since the last change of
    * device mode.
    * Note: This index is incremented only if gas conversion is active.
    * @param field 0-2
    */
    int getSubMeasurementIndex(int field = 0);

    /**
    * [gas_valid_rl]
    * In parallel mode, each TPHG sequence contains a gas measurement slot, either a real one which
    * result is used or a dummy one to keep a constant sampling rate and predictable device timing. A
    * real gas conversion (i.e., not a dummy one) is indicated by the gas_valid_rl status register.
    * @param field 0-2
    */
    bool isGasValid(int field = 0);

    /**
    * [heat_stab_rl]
    * Heater temperature stability for target heater resistance is indicated heat_stab_x status bits.
    * @param field 0-2
    */
    bool isHeaterStable(int field = 0);

    // GAS CONTROL #########################################################################

    /**
    * [idac_heat_x]
    * BME680 contains a heater control block that will inject enough current into the heater resistance
    * to achieve the requested heater temperature. There is a control loop which periodically measures
    * heater resistance value and adapts the value of current injected from a DAC.
    * BME680 heater operation could be speeded up by setting an initial heater current for a target
    * heater temperature by using register idac_heat_x<7:0>. This step is optional since the control
    * loop will find the current after a few iterations anyway.
    * Current injected to the heater in mA = (idac_heat_7_1 + 1) / 8
    * Where: idac_heat_7_1 = decimal value stored in idac_heat<7:1> (unsigned, value from 0 to 127)
    * @param setPoint 0-9
    */
    uint8_t getHeaterCurrent(int setPoint);

    /**
    * [idac_heat_x]
    * BME680 contains a heater control block that will inject enough current into the heater resistance
    * to achieve the requested heater temperature. There is a control loop which periodically measures
    * heater resistance value and adapts the value of current injected from a DAC.
    * BME680 heater operation could be speeded up by setting an initial heater current for a target
    * heater temperature by using register idac_heat_x<7:0>. This step is optional since the control
    * loop will find the current after a few iterations anyway.
    * Current injected to the heater in mA = (idac_heat_7_1 + 1) / 8
    * Where: idac_heat_7_1 = decimal value stored in idac_heat<7:1> (unsigned, value from 0 to 127)
    * @param setPoint 0-9
    */
    void setHeaterCurrent(int setPoint, uint8_t value);

    /**
    * [res_heat_x]
    * Target heater resistance is programmed by user through res_heat_x<7:0> registers.
    * res_heat_x = 3.4* ((R_Target*(4/4+res_heat_range))-25) / ((res_heat_val * 0.002) + 1))
    * Where
    * R_Target is the target heater resistance in Ohm
    * res_heat_x is the decimal value that needs to be stored in register with same name
    * res_heat_range is heater range stored in register address 0x02 <5:4>
    * res_heat_val is heater resistance correction factor stored in register address 0x00 (signed, value from -128 to 127)
    * @param setPoint 0-9
    */
    int8_t getTargetHeaterResistance(int setPoint);

    /**
    * [res_heat_x]
    * Target heater resistance is programmed by user through res_heat_x<7:0> registers.
    * res_heat_x = 3.4* ((R_Target*(4/4+res_heat_range))-25) / ((res_heat_val * 0.002) + 1))
    * Where
    * R_Target is the target heater resistance in Ohm
    * res_heat_x is the decimal value that needs to be stored in register with same name
    * res_heat_range is heater range stored in register address 0x02 <5:4>
    * res_heat_val is heater resistance correction factor stored in register address 0x00 (signed, value from -128 to 127)
    * @param setPoint 0-9
    */
    void setTargetHeaterResistance(int setPoint, int8_t value);

    /**
    * [gas_wait_x]
    * gas_wait_x controls heater timing of the gas sensor. Functionality of this register will vary based on power modes.
    * Forced Mode & Sequential mode
    * Time between beginning of heat phase and start of sensor resistance conversion depend on gas_wait_x settings as mentioned below.
    * Parallel Mode
    * The number of TPHG sub-measurement sequences within the one Gas conversion for one target
    * temperature resistance is defined by gas_wait_x settings.
    * Note: Please take care about gas_wait_x on shifting modes between parallel & sequential/forced mode as register functionality will change.
    * @return result * 0.477 ms
    * @param setPoint 0-9
    */
    int getGasWaitTime(int setPoint);

    /**
    * [gas_wait_x]
    * gas_wait_x controls heater timing of the gas sensor. Functionality of this register will vary based on power modes.
    * Forced Mode & Sequential mode
    * Time between beginning of heat phase and start of sensor resistance conversion depend on gas_wait_x settings as mentioned below.
    * Parallel Mode
    * The number of TPHG sub-measurement sequences within the one Gas conversion for one target
    * temperature resistance is defined by gas_wait_x settings.
    * Note: Please take care about gas_wait_x on shifting modes between parallel & sequential/forced mode as register functionality will change.
    * @return result * 0.477 ms
    * @param setPoint 0-9
    * @param time 64 timer values with 1ms step sizes, all zeros means no wait.
    * @param multiplication [0, 1, 2, 3] -> [1, 4, 16, 64]
    */
    void setGasWaitTime(int setPoint, int time, int multiplication);

    /**
    * [gas_wait_shared]
    * The programmable wait time between two TPHG sub-measurement sequences of parallel mode depends on gas_wait_shared settings as follows
    */
    int getGasWaitShared();

    /**
    * [gas_wait_shared]
    * The programmable wait time between two TPHG sub-measurement sequences of parallel mode depends on gas_wait_shared settings as follows
    * @param setPoint 0-9
    * @param time 64 timer values with 0.477 ms step sizes, all zeros means no wait.
    * @param multiplication [0x00, 0x01, 0x10, 0x11] -> [1, 4, 16, 64]
    */
    void setGasWaitShared(int time, int multiplication);

    /**
    * [heat_off]
    * Turn off current injected to heater
    */
    void setHeaterOff();

    /**
    * [nb_conv]
    * is used to select heater set-points of BME680
    * Sequential & Parallel Mode
    * User can program a sequence of up to 10 conversions by setting nb_conv<3:0>. Each conversion has its own heater resistance target but 3 field registers to store conversion results. The actual
    * gas conversion number in the measurement sequence (up to 10 conversions numbered from 0 to 9) is stored in gas measurement index register
    * In parallel mode, no TPH conversions are ran at all. In sequential mode, TPH conversions are run according to osrs_t|p|h settings, gas is skipped
    * @return Sequential & Parallel : number of profiles (0-10), 0 means no gas conversion
    * @return Forced : indicates index of heater profile
    */
    int getHeaterProfile();

    /**
    * [nb_conv]
    * is used to select heater set-points of BME680
    * Sequential & Parallel Mode
    * User can program a sequence of up to 10 conversions by setting nb_conv<3:0>. Each conversion has its own heater resistance target but 3 field registers to store conversion results. The actual
    * gas conversion number in the measurement sequence (up to 10 conversions numbered from 0 to 9) is stored in gas measurement index register
    * In parallel mode, no TPH conversions are ran at all. In sequential mode, TPH conversions are run according to osrs_t|p|h settings, gas is skipped
    * @param Sequential & Parallel : number of profiles (0-10), 0 means no gas conversion
    * @param Forced : indicates index of heater profile
    */
    void setHeaterProfile(int value);

    /**
    * [run_gas_l]
    * The gas conversions are started only in appropriate mode if run_gas_l=1
    */
    void runGasConversion();

    /**
    * [odr]
    * Wake period in sequential mode – odr
    * In the sequential mode operation the device periodically enters stand-by state and returns to an operational state after a given wake-up period. Wake period can be programmed by odr<3:0> register as shown below
    * @return in ms, 0 means device does not go to standby
    */
    float getWakePeriod();

    /**
    * [odr]
    * Wake period in sequential mode – odr
    * In the sequential mode operation the device periodically enters stand-by state and returns to an operational state after a given wake-up period. Wake period can be programmed by odr<3:0> register as shown below
    * @param value : [0 - 8+] [0.59,62.5,125,250,500,1000,10,20,no standby]
    */
    void setWakePeriod(int value);

    // PRESSURE TEMPERATURE HUMIDITY CONTROL #########################################################################

    /**
    * [osrs_h]
    * @return value : [0,1,2,4,8,16] -> [skip,X1,X2,X4,X8,X16], 0 means skipped (output set to 0x8000)
    */
    int getOversamplingHumidity();

    /**
    * [osrs_h]
    * @param value : [0,1,2,3,4,5] -> [skip,X1,X2,X4,X8,X16], 0 means skipped (output set to 0x8000)
    */
    void setOversamplingHumidity(int value);

    /**
    * [osrs_p]
    * @return value : [0,1,2,4,8,16] -> [skip,X1,X2,X4,X8,X16], 0 means skipped (output set to 0x8000)
    */
    int getOversamplingPressure();

    /**
    * [osrs_p]
    * @param value : [0,1,2,3,4,5] -> [skip,X1,X2,X4,X8,X16], 0 means skipped (output set to 0x8000)
    */
    void setOversamplingPressure(int value);

    /**
    * [osrs_t]
    * @return value : [0,1,2,4,8,16] -> [skip,X1,X2,X4,X8,X16], 0 means skipped (output set to 0x8000)
    */
    int getOversamplingTemperature();

    /**
    * [osrs_t]
    * @param value : [0,1,2,3,4,5] -> [skip,X1,X2,X4,X8,X16], 0 means skipped (output set to 0x8000)
    */
    void setOversamplingTemperature(int value);

    /**
    * [filter]
    * IIR filter control
    * IIR filter applies to temperature and pressure data but not to humidity and gas data. The data
    * coming from the ADC are filtered and then loaded into the data registers. The T, P result registers
    * are updated together at the same time at the end of measurement. IIR filter output resolution is
    * 20 bits. The T, P result registers are reset to value 0x80000 when the T, P measurements have
    * been skipped (osrs_x=”000‟). The appropriate filter memory is kept unchanged (the value fromt he last measurement is kept). When the appropriate OSRS register is set back to nonzero, then
    * the first value stored to the T, P result register is filtered.
    * @return value : [0,1,3,7,15,31,63,127]
    */
    int getIIRfilterCoefficient();

    /**
    * [filter]
    * IIR filter control
    * IIR filter applies to temperature and pressure data but not to humidity and gas data. The data
    * coming from the ADC are filtered and then loaded into the data registers. The T, P result registers
    * are updated together at the same time at the end of measurement. IIR filter output resolution is
    * 20 bits. The T, P result registers are reset to value 0x80000 when the T, P measurements have
    * been skipped (osrs_x=”000‟). The appropriate filter memory is kept unchanged (the value fromt he last measurement is kept). When the appropriate OSRS register is set back to nonzero, then
    * the first value stored to the T, P result register is filtered.
    * @param value : [0,1,2,3,4,5,6,7] -> [0,1,3,7,15,31,63,127]
    */
    void setIIRfilterCoefficient(int value);

    // GENERAL CONTROL #########################################################################

    /**
    * [mode]
    * Four measurement modes are available for BME680; that is sleep, sequential, parallel and forced
    * mode.Four measurement modes are available for BME680; that is sleep, sequential, parallel and forced mode.
    * @param mode : [0,1,2,3] -> [Sleep, Forced, Parallel, Sequential]
    */
    void setMode(int mode);

    /**
    * [mode]
    * Four measurement modes are available for BME680; that is sleep, sequential, parallel and forced
    * mode.Four measurement modes are available for BME680; that is sleep, sequential, parallel and forced mode.
    * @return value : [0,1,2,3] -> [Sleep, Forced, Parallel, Sequential]
    */
    int getMode();

    /**
    * [chip_id]
    * Chip id of the device, this should give 0x61
    */
    int getChipID();
};

#endif