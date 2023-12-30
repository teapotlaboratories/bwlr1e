#ifndef BME688_H
#define BME688_H

#include "mbed.h"
#include "bme68x_defs.h"
#include "bme68x.h"
#include "bsec_datatypes.h"
#include "bsec_interface.h"

namespace teapotlabs {
namespace sensors {
namespace bme688 {

/* NOTE: defining constexpr in header might not be the best way to replace macro */
constexpr uint8_t kBme688ChipId = 0x61;
constexpr uint8_t kBme688ChipIdAddr = 0xD0;
constexpr uint16_t kBsecTotalHeatDur = UINT16_C(140);

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
    kSensorInitFail,
    kNullPointer,
    kSensorReadRegisterFail,
    kSensorChipIdUnknown,
    kSensorGetChipIdFail
};

using Callback = void (*)( const bme68x_data data, bsec_output_t* const outputs, const uint8_t n_outputs );

class BME688{

    public:

        uint32_t bme688_addr;
        uint32_t bme688_addr_8bit;
        PinName  i2c_sda;
        PinName  i2c_scl;
        I2C*     i2c_local;
    
        BME688( PinName i2c_sda, PinName i2c_scl, uint32_t bme688_addr );

        /**
         * @brief Function to initialize the library
         * @param sensor_list   : The list of output sensors
         * @param n_sensors     : Number of outputs requested
         * @param sample_rate   : The sample rate of requested sensors
         * @return ReturnCode::kOk if everything initialized correctly
         */
        ReturnCode Initialise( bsec_virtual_sensor_t* sensor_list, uint8_t n_sensors, float sample_rate );

        /**
         * @brief Set callback to notify data ready to user
         * @param cb   : User callback
         * @return ReturnCode::kOk if cb is valid
         */
        ReturnCode SetCallback( Callback cb );

        /**
         * @brief Set temperature offset for post processing sensor reading 
         * @param temp_offset   : temperature to offset sensor readings
         * @return ReturnCode::kOk if cb is valid
         */
        ReturnCode SetTemperatureOffset( const float temp_offset );

        /**
         * @brief Get next time Run() need to be called in ns
         * @return target time in nanoseconds for next Run() call
         */
        int64_t GetNextRunTimeNs();

        /**
         * @brief Call from the user to read data from the BME68X using parallel mode/forced mode, process and store outputs
         * @return ReturnCode::kOk if processing is succesful
         */
        ReturnCode Run();

        /**
         * @brief Get last bme688 library call status
         * @return last status of a bme68x api call
         */
        int8_t GetLastSensorCallStatus();


        /**
         * @brief Get last bme68x library call status
         * @return last status of a bme68x api call
         */

        /**
         * @brief Get last bsec library call status
         * @return last status of a bsec api call
         */
        bsec_library_return_t GetLastBsecCallStatus();

    private:    
        /* local struct definition */
        struct Bme688FetchedData
        {
            bme68x_data data[3];
            uint8_t     n_fields;
            uint8_t     i_fields;
        };
        
        /* bme688 sensor internal variables
           store configuration for other bsec method to use */
        struct
        {
            bme68x_conf conf;         // sensor configuration
            bme68x_dev  dev;          // low-level sensor device structure definition
            int8_t      status;       // last low-level api call return value
            uint8_t     last_op_mode; // last operation mode of the sensor
        } sensor;

        /* bsec internal variables */
        struct
        {            
            // bsec libray configuration
            bsec_bme_settings_t     sensor_conf;    // sensor settings used by bsec
            float                   temperature_offset;
            bsec_library_return_t   status; // last bsec api call return value

            // requested virtual sensor list and sample rate
            float                   sample_rate;
            uint8_t                 n_sensors;
            bsec_virtual_sensor_t   sensor_list[32]; // allocate 32 for now
        } bsec;
        Callback    callback;

        /* BME688 sensor specific method */
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

        /* BSEC sensor specific method */
        ReturnCode InitialiseBsec();
        ReturnCode UpdateSubscription( bsec_virtual_sensor_t* sensor_list, 
                                       uint8_t n_sensors, 
                                       float sample_rate );
        ReturnCode ProcessData( const int64_t curr_time_ns, const bme68x_data &data );


        ReturnCode ReadRegister( const uint8_t reg_addr, uint8_t* const reg_data, uint32_t length );
        ReturnCode GetChipId( uint8_t& chip_id_out );
};

} // bme688
} // namespace sensors
} // namespace teapotlabs

#endif