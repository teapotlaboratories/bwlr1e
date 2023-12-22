#ifndef __BME688__H
#define __BME688__H

#include "mbed.h"
#include "bme68x_defs.h"
#include "bsec_interface.h"
#include "bsec_datatypes.h"
#include "bme68x.h"

#define BSEC_CHECK_INPUT(x, shift)  (x & (1 << (shift-1)))
#define BME688_CHIP_ID_LOCATION 0xD0
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
            kSensorBsecProcessFail,
            kSensorGetDataFail,
            kBsecInitFail,
            kBsecRunFail,
            kSensorInitFail
        };

        typedef struct
        {
            bsec_output_t output[BSEC_NUMBER_OUTPUTS];
            uint8_t n_outputs;
        } BsecOutputs;

        using Callback = void (*)(const bme68x_data data, const BsecOutputs outputs);

        uint32_t bme688_addr;
        uint32_t bme688_addr_8bit;
        PinName  i2c_sda;
        PinName  i2c_scl;
        I2C*     i2c_local;
    
        BME688( PinName i2c_sda, PinName i2c_scl, uint32_t bme688_addr );
        ReturnCode Initialise( bsec_virtual_sensor_t sensor_list[], uint8_t n_sensors, float sample_rate );

    private:    
        struct Bme688FetchedData
        {
            bme68x_data data[3];
            uint8_t     n_fields;
            uint8_t     i_fields;
        };
        
        // internal bsec configuration
        struct BsecConfiguration
        {            
            // bsec libray configuration
            bsec_bme_settings_t     sensor_conf;    // sensor settings used by bsec
            float                   temperature_offset;
            bsec_library_return_t   status; // last bsec api call return value

            // requested virtual sensor list and sample rate
            float                   sample_rate;
            uint8_t                 n_sensors;
            bsec_virtual_sensor_t   sensor_list[32]; // allocate 32 for now
        };

        struct Bme688Configuration
        {
            // store configuration for other bsec method to use
            bme68x_conf conf;         // sensor configuration
            bme68x_dev  dev;          // low-level sensor device structure definition
            int8_t      status;       // last low-level api call return value
            uint8_t     last_op_mode; // last operation mode of the sensor
        };

        // bsec internal variables
        BME688::BsecConfiguration   bsec;
        BME688::Bme688Configuration sensor;
        Callback                    callback;

        // BME688 sensor specific method
        ReturnCode InitialiseSensor();
        ReturnCode SetSensorTphOverSampling( const uint8_t os_temp, 
                                             const uint8_t os_pres, 
                                             const uint8_t os_hum );
        ReturnCode SetSensorOperationMode( const uint8_t op_mode );
        ReturnCode SetSensorHeaterProfile( const uint16_t temp, 
                                           const uint16_t dur );
        ReturnCode SetSensorHeaterProfile( uint16_t* const temp,
                                           uint16_t* const mul, 
                                           const uint16_t shared_heatr_dur, 
                                           const uint8_t profile_len );
        uint32_t GetSensorMeasurementDuration( const uint8_t op_mode );
        ReturnCode SetSensorToForcedMode( const bsec_bme_settings_t& bsec_bme_conf );
        ReturnCode SetSensorToParallelMode( const bsec_bme_settings_t& bsec_bme_conf );
        ReturnCode FetchSensorData( Bme688FetchedData &data_out );
        uint8_t GetSensorData( Bme688FetchedData &data_in,
                               bme68x_data &data_out );
        
        // BSEC specific method
        ReturnCode InitialiseBsec();
        ReturnCode SetTemperatureOffset( const float temp_offset );
        ReturnCode UpdateSubscription( bsec_virtual_sensor_t sensor_list[], 
                                       uint8_t n_sensors, 
                                       float sample_rate );
        ReturnCode ProcessData( const int64_t curr_time_ns, const bme68x_data &data );
        ReturnCode Run();
};


#endif