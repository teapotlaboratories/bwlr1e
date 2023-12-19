#ifndef __BME688__H
#define __BME688__H

#include "mbed.h"
#include "bme68x_defs.h"
#include "bsec_interface.h"
#include "bsec_datatypes.h"
#include "bme68x.h"

#define BME688_CHIP_ID_LOCATION 0xD0
#define BSEC_REQUESTED_VIRTUAL_SENSORS_NUMBER 4
#define BSEC_TEMPERATURE_OFFSET 7.0f
#define BSEC_TOTAL_HEAT_DUR UINT16_C(140)

class BME688{

    // enum and struct definition
    public:
        enum class ReturnCode
        {
            // success code
            kOk = 0,
            // error code
            kError,
            kSensorStructureFail,
            kSensorConfigFail,
            kSensorHeaterFail,
            kSensorSetOperationFail,
            kSensorOperationSeqFail,
            kSensorBsecFail,
            kSensorBsecSubscriptionFail,
            kSensorGetDataFail,
        };

        // processed sensor output
        struct SensorData
        {
            float   temperature;
            float   humidity;
            float   co2;
            uint8_t co2_accuracy;
            float   iaq;
            uint8_t iaq_accuracy;
        };

        // internal bsec configuration
        struct BsecConfiguration
        {            
            // sensor configuration
            bme68x_data                 sensor_data[3];
            bme68x_dev                  sensor_structure;
            bme68x_conf                 sensor_config;
            bme68x_heatr_conf           sensor_heater_config;
            uint8_t                     last_op_mode; 

            // bsec libray configuration
            bsec_bme_settings_t         bme_conf;
            uint8_t                     op_mode; // current sensor operation mode
            uint8_t                     data_fields;
            bsec_sensor_configuration_t requested_virtual_sensors[4];
            uint8_t                     number_required_sensor_settings;
            uint8_t                     requested_virtual_sensors_number;
            int64_t                     current_time_ns;
            bsec_output_t               outputs[BSEC_NUMBER_OUTPUTS];
            float                       temperature_offset;
            bool                        new_data_available;
            uint16_t                    temp_profile[2];
            uint16_t                    dur_profile[2];
        };

    public:
        bool     new_data_available;
        uint32_t bme688_addr;
        uint32_t bme688_addr_8bit;
        PinName  i2c_sda;
        PinName  i2c_scl;
        I2C*     i2c_local;
    
        BME688      ( PinName i2c_sda, PinName i2c_scl, uint32_t bme688_addr );
        void        DoMeasurements();
        SensorData  GetLatest();
        bool        IsNewDataAvailable();
        ReturnCode  Initialise();
        void        DumpData();

    private:    
        // bsec internal variables
        BME688::BsecConfiguration bsec_conf;
        SensorData                sensor_data;

        // BME688 sensor specific method
        ReturnCode InitialiseSensor();
        ReturnCode InitialiseSensorFilterSettings();
        ReturnCode InitialiseSensorHeaterSettings();
        ReturnCode SetSequentialMode();
        ReturnCode SetTphOverSampling( const uint8_t os_temp, 
                                       const uint8_t os_pres, 
                                       const uint8_t os_hum );
        ReturnCode SetOperationMode(const uint8_t op_mode);
        ReturnCode SetHeaterProfile( const uint16_t temp, 
                                     const uint16_t dur );
        ReturnCode SetHeaterProfile( uint16_t* const temp,
                                     uint16_t* const mul, 
                                     const uint16_t sharedHeatrDur, 
                                     const uint8_t profileLen );
        uint32_t GetMeasurementDuration(const uint8_t op_mode);

        // BSEC specific method
        ReturnCode InitialiseBsec();
        ReturnCode UpdateSubscription();
        ReturnCode ProcessData();
        void       BsecProcessing();
        ReturnCode SetBme68xConfigForced( const bsec_bme_settings_t& bsec_bme_conf );
        ReturnCode SetBme68xConfigParallel( const bsec_bme_settings_t& bsec_bme_conf );
};


#endif