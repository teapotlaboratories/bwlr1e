#include "BME688.h"
#include "Defer.h"

#define BSEC_CHECK_INPUT(x, shift)  (x & (1 << (shift-1)))

using namespace std::chrono;
using namespace teapotlabs::utils;

namespace teapotlabs {
namespace sensors {
namespace bme688 {

// *********************************************************************
// SENSOR STATIC CALLBACK DEFINITION
// *********************************************************************
static int8_t SensorInternalReadRegisterCb( uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr )
{
    // check user intf_ptr object
    if( intf_ptr == nullptr )
    {
        return 0xFF; // return error
    }

    BME688* bme688 = (BME688*) intf_ptr;
    if( bme688->i2c_local == nullptr )
    {
        // return error
        return 0xFF;
    }
    
    // local variable definition
    int8_t rslt = 0; // Return 0 for Success, non-zero for failure
    uint32_t aux  = 0;
    aux = bme688->i2c_local->write ( bme688->bme688_addr_8bit, (char*)&reg_addr, 1, true );
    if ( aux != 0 ) {
        return 0xFF; // return error
    } 

    aux = bme688->i2c_local->read  ( bme688->bme688_addr_8bit, (char*)&reg_data[0], length );
    if ( aux != 0 ) {
        return 0xFF; // return error
    } 
 
    return rslt;
}

static int8_t SensorInternalWriteRegisterCb( uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr )
{
    // check user intf_ptr object
    if( intf_ptr == nullptr )
    {
        return 0xFF; // return error
    }

    // check callback obeject pointer
    BME688* bme688 = (BME688*) intf_ptr;
    if( bme688->i2c_local == nullptr )
    {
        return 0xFF; // return error
    }

    // local variable definition
      int8_t rslt    = 0; // Return 0 for Success, non-zero for failure
    uint32_t aux     = 0;
        char cmd[16] = {0};
    uint32_t i       = 0;

    // Prepare the data to be sent
    cmd[0] = reg_addr;
    for ( i = 1; i <= length; i++ ) {
        cmd[i] = reg_data[i - 1];
    }

    // Write data
    aux = bme688->i2c_local->write( bme688->bme688_addr_8bit, &cmd[0], length + 1, false );
    if ( aux != 0 ) {
        return 0xFF; // return error
    } 

    return rslt;
}

static void SensorInternalDelayUsCb(uint32_t time_us, void *intf_ptr)
{
    /* use wait_us to wait without sleep
       if system goes to sleep, I2C comm need to be re-init */
    wait_us ( time_us );
}

// *********************************************************************
// PUBLIC
// *********************************************************************

BME688::BME688( PinName i2c_sda, PinName i2c_scl, uint32_t bme688_addr ):bsec({0}),
                                                                         sensor({0})
{    
    this->i2c_sda = i2c_sda;
    this->i2c_scl = i2c_scl;
    this->bme688_addr = bme688_addr;
    this->bme688_addr_8bit = ( bme688_addr << 1 );

    this->bsec.temperature_offset = 0.0f;
    this->callback = nullptr;
}

ReturnCode BME688::Initialise( bsec_virtual_sensor_t* sensor_list, uint8_t n_sensors, float sample_rate )
{
    /* enable I2C */
    I2C i2c_temp( i2c_sda, i2c_scl ); // will be freed after function return
    this->i2c_local = &i2c_temp;      // set i2c object for bsec to use, see SensorInternalWriteRegisterCb and SensorInternalReadRegisterCb
    Defer<I2C*> i2c_defer( this->i2c_local, nullptr );  // defer changing i2c_local to nullptr at function return

    ReturnCode result;

    /* copy virtual sensor list and sample rate */
    this->bsec.sample_rate = sample_rate;
    this->bsec.n_sensors = n_sensors;
    memcpy(this->bsec.sensor_list, sensor_list, n_sensors);
    
    uint8_t chip_id;
    result = this->GetChipId( chip_id );
    if( result != ReturnCode::kOk )
    {
        return result;
    }
    if( chip_id != kBme688ChipId )
    {
        return ReturnCode::kSensorChipIdUnknown;
    }

    /* initialise bme688 sensor */ 
    result = this->InitialiseSensor();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    /* initialise bsec library */
    result = this->InitialiseBsec();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    /* tell bsec library which virtual sensor to process */
    result = this->UpdateSubscription( this->bsec.sensor_list,
                                       this->bsec.n_sensors,
                                       this->bsec.sample_rate );
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    return ReturnCode::kOk;
}

ReturnCode BME688::SetCallback( Callback cb )
{
    if( cb == nullptr )
    {
        return ReturnCode::kNullPointer;
    }
 
    this->callback = cb;
    return ReturnCode::kOk;
}

ReturnCode BME688::SetTemperatureOffset( const float temp_offset )
{
    this->bsec.temperature_offset = temp_offset;
    return ReturnCode::kOk;
}

/**
 * @brief Callback from the user to read data from the BME68X using parallel mode/forced mode, process and store outputs
 */
ReturnCode BME688::Run(void)
{
    /* enable I2C */
    I2C i2c_temp( i2c_sda, i2c_scl ); // will be freed after function return
    this->i2c_local = &i2c_temp;      // set i2c object for bsec to use, see SensorInternalWriteRegisterCb and SensorInternalReadRegisterCb
    Defer<I2C*> i2c_defer( this->i2c_local, nullptr );  // defer changing i2c_local to nullptr at function return


    ReturnCode ret = ReturnCode::kError;

    int64_t curr_time_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(Kernel::Clock::now()).time_since_epoch().count();
    this->sensor.last_op_mode = this->bsec.sensor_conf.op_mode;

    if (curr_time_ns >= this->bsec.sensor_conf.next_call)   
    {
        /* Provides the information about the current sensor configuration that is
           necessary to fulfill the input requirements, eg: operation mode, timestamp
           at which the sensor data shall be fetched etc */
        this->bsec.status = bsec_sensor_control( curr_time_ns, 
                                                 &(this->bsec.sensor_conf) );
        if (this->bsec.status != BSEC_OK)
            return ReturnCode::kBsecRunFail;

        switch ( this->bsec.sensor_conf.op_mode )
        {
            case BME68X_FORCED_MODE:
                ret = SetSensorToForcedMode( this->bsec.sensor_conf );
                break;
            case BME68X_PARALLEL_MODE:
                if ( this->sensor.last_op_mode != this->bsec.sensor_conf.op_mode )
                {
                    ret = SetSensorToParallelMode( this->bsec.sensor_conf );
                }
                break;

            case BME68X_SLEEP_MODE:
                if ( this->sensor.last_op_mode != this->bsec.sensor_conf.op_mode )
                {
                    ret = SetSensorOperationMode( BME68X_SLEEP_MODE );
                }
                break;
        }

        if( ret != ReturnCode::kOk )
            return ret;

        if( this->bsec.sensor_conf.trigger_measurement && 
            this->bsec.sensor_conf.op_mode != BME68X_SLEEP_MODE )
        {
            Bme688FetchedData raw_data;
            ret = FetchSensorData( raw_data );
            if( ret == ReturnCode::kOk )
            {
                uint8_t n_fields_left = 0;
                bme68x_data data;
                do
                {
                    n_fields_left = GetSensorData( raw_data, data );
                    /* check for valid gas data */
                    if( data.status & BME68X_GASM_VALID_MSK )
                    {
                        ret = ProcessData(curr_time_ns, data);
                        if( ret != ReturnCode::kOk )
                        {
                            return ret;
                        }
                    }
                } while (n_fields_left);
            }

        }

    }
    return ReturnCode::kOk;
}

int8_t BME688::GetLastSensorCallStatus()
{
    return this->sensor.status;
}

bsec_library_return_t BME688::GetLastBsecCallStatus()
{
    return this->bsec.status;
}


// *********************************************************************
// PRIVATE
// *********************************************************************

ReturnCode BME688::InitialiseSensor()
{
    sensor.dev.intf_ptr =  this;
    sensor.dev.intf     =  BME68X_I2C_INTF;
    sensor.dev.read     =  SensorInternalReadRegisterCb;
    sensor.dev.write    =  SensorInternalWriteRegisterCb;
    sensor.dev.delay_us =  SensorInternalDelayUsCb;
    sensor.dev.amb_temp =  25;
    
    this->sensor.status = bme68x_init(&sensor.dev);
    if( this->sensor.status != BME68X_OK )
    {
        return ReturnCode::kSensorInitFail;
    }
    return ReturnCode::kOk;
}

ReturnCode BME688::InitialiseBsec()
{
    this->bsec.status = bsec_init();
    if( this->bsec.status != BSEC_OK )
    {
        return ReturnCode::kBsecInitFail;
    }
    return ReturnCode::kOk;
}

ReturnCode BME688::UpdateSubscription(bsec_virtual_sensor_t* sensor_list, uint8_t n_sensors, float sample_rate)
{
    bsec_sensor_configuration_t virtual_sensors[BSEC_NUMBER_OUTPUTS];
    bsec_sensor_configuration_t sensor_settings[BSEC_MAX_PHYSICAL_SENSOR];
    uint8_t                     n_sensor_settings = BSEC_MAX_PHYSICAL_SENSOR;

    for (uint8_t i = 0; i < n_sensors; i++)
    {
        virtual_sensors[i].sensor_id = sensor_list[i];
        virtual_sensors[i].sample_rate = sample_rate;
    }

    /* Subscribe to library virtual sensors outputs */
    this->bsec.status = bsec_update_subscription( virtual_sensors, 
                                                  n_sensors, 
                                                  sensor_settings, 
                                                  &n_sensor_settings); // TODO: need to check if all sensor allocated in n_sensor_settings
    if (this->bsec.status != BSEC_OK)
    {
        return ReturnCode::kSensorBsecSubscriptionFail;
    }

    return ReturnCode::kOk;
}

int64_t BME688::GetNextRunTimeNs(){
    return this->bsec.sensor_conf.next_call;
}

/**
 * @brief Set the BME68X sensor configuration to forced mode
 */
ReturnCode BME688::SetSensorToForcedMode( const bsec_bme_settings_t &conf )
{
    ReturnCode status = ReturnCode::kError;
    
    // Set the filter, odr, temperature, pressure and humidity settings */
    status = SetSensorTphOverSampling( conf.temperature_oversampling, 
                                       conf.pressure_oversampling, 
                                       conf.humidity_oversampling );
    if( status != ReturnCode::kOk )
    {
        return status;
    }

    // set heater profile
    status = SetSensorHeaterProfile( conf.heater_temperature, 
                                     conf.heater_duration );
    if( status != ReturnCode::kOk )
    {
        return status;
    }

    // set operation mode
    status = SetSensorOperationMode( BME68X_FORCED_MODE );
    if( status != ReturnCode::kOk )
    {
        return status;
    }

    this->sensor.last_op_mode = BME68X_FORCED_MODE;
    return status;
}


/**
 * @brief Set the BME68X sensor configuration to parallel mode
 */
ReturnCode BME688::SetSensorToParallelMode( const bsec_bme_settings_t& conf )
{
    uint16_t shared_heater_dur = 0;

    ReturnCode status = ReturnCode::kError;
    
    // Set the filter, odr, temperature, pressure and humidity settings */
    status = SetSensorTphOverSampling( conf.temperature_oversampling, 
                                       conf.pressure_oversampling, 
                                       conf.humidity_oversampling );
    if( status != ReturnCode::kOk )
    {
        return status;
    }

    // calculate heater duration
    shared_heater_dur = kBsecTotalHeatDur - (GetSensorMeasurementDuration(BME68X_PARALLEL_MODE) / INT64_C(1000));

    // set heater profile
    status = SetSensorHeaterProfile( (uint16_t*) conf.heater_temperature_profile, 
                                     (uint16_t*) conf.heater_duration_profile,
                                     shared_heater_dur,
                                     conf.heater_profile_len );
    if( status != ReturnCode::kOk )
    {
        return status;
    }

    // set bme688 operation mode
    status = SetSensorOperationMode( BME68X_PARALLEL_MODE );
    if( status != ReturnCode::kOk )
    {
        return status;
    }

    this->sensor.last_op_mode = BME68X_PARALLEL_MODE;
    return status;
}

/**
 * @brief Function to set the Temperature, Pressure and Humidity over-sampling
 */
ReturnCode BME688::SetSensorTphOverSampling( const uint8_t os_temp, 
                                                     const uint8_t os_pres, 
                                                     const uint8_t os_hum )
{
    this->sensor.status = bme68x_get_conf(&sensor.conf, &sensor.dev);
    if( this->sensor.status == BME68X_OK )
    {
        sensor.conf.os_hum = os_hum;
        sensor.conf.os_pres = os_pres;
        sensor.conf.os_temp = os_temp;

        this->sensor.status = bme68x_set_conf(&sensor.conf, &sensor.dev);
        if( this->sensor.status == BME68X_OK )
        {
            return ReturnCode::kOk;
        }
    }

    return ReturnCode::kSensorConfigFail;
}

/**
 * @brief Function to set the heater profile for Forced mode
 */
ReturnCode BME688::SetSensorHeaterProfile( const uint16_t temp, 
                                             const uint16_t dur )
{
    bme68x_heatr_conf heater_conf
    {
        .enable = BME68X_ENABLE,
        .heatr_temp = temp,
        .heatr_dur = dur
    };

    this->sensor.status = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heater_conf, &sensor.dev);
    if( this->sensor.status == BME68X_OK )
    {
        return ReturnCode::kOk;
    }

    return ReturnCode::kSensorHeaterFail;
}

/**
 * @brief Function to set the heater profile for Parallel mode
 */
ReturnCode BME688::SetSensorHeaterProfile( uint16_t* const temp, 
                                             uint16_t* const mul, 
                                             const uint16_t shared_heater_dur, 
                                             const uint8_t profile_len )
{
    bme68x_heatr_conf heater_conf
    {
        .enable = BME68X_ENABLE,
        .heatr_temp_prof = temp,
        .heatr_dur_prof = mul,
        .profile_len = profile_len,
        .shared_heatr_dur = shared_heater_dur
    };

    this->sensor.status = bme68x_set_heatr_conf( BME68X_PARALLEL_MODE, &heater_conf, &sensor.dev);
	if( this->sensor.status == BME68X_OK )
    {
        return ReturnCode::kOk;
    }

    return ReturnCode::kSensorHeaterFail;
}


/**
 * @brief Function to set the operation mode
 */
ReturnCode BME688::SetSensorOperationMode(const uint8_t op_mode)
{
    this->sensor.status = bme68x_set_op_mode(op_mode, &sensor.dev);
	if( this->sensor.status == BME68X_OK && 
                    op_mode != BME68X_SLEEP_MODE )
    {
		this->sensor.last_op_mode = op_mode;
    }

    if( this->sensor.status == BME68X_OK )
    {
        return ReturnCode::kOk;
    }
    return ReturnCode::kSensorSetOperationFail;
}

/**
 * @brief Function to get the measurement duration in microseconds
 */
uint32_t BME688::GetSensorMeasurementDuration(const uint8_t op_mode)
{
    uint8_t target_op_mode = op_mode;
	if (target_op_mode == BME68X_SLEEP_MODE)
    {
		target_op_mode = this->sensor.last_op_mode;
    }

	return bme68x_get_meas_dur(target_op_mode, &sensor.conf, &sensor.dev);
}

/**
 * @brief Function to fetch data from the sensor into the local buffer
 */
ReturnCode BME688::FetchSensorData( Bme688FetchedData &data_out )
{
	data_out.n_fields = 0;
	data_out.i_fields = 0;
	this->sensor.status = bme68x_get_data( this->sensor.last_op_mode,
                                           data_out.data, 
                                           &(data_out.n_fields), 
                                           &(this->sensor.dev) );

    if ( this->sensor.status != BME68X_OK )
    {
        return ReturnCode::kSensorGetDataFail;
    }

    return ReturnCode::kOk;
}

/**
 * @brief Function to get a single data field
 */
uint8_t BME688::GetSensorData( Bme688FetchedData &data_in,
                               bme68x_data &data_out )
{
	if (this->sensor.last_op_mode == BME68X_FORCED_MODE)
	{
		data_out = data_in.data[0];
	} 
    else
	{
		if( data_in.n_fields )
		{
			/* iFields spans from 0-2 while nFields spans from
			 * 0-3, where 0 means that there is no new data
			 */
			data_out = data_in.data[data_in.i_fields];
			data_in.i_fields++;

			/* Limit reading continuously to the last fields read */
			if ( data_in.i_fields >= data_in.n_fields )
			{
				data_in.i_fields = data_in.n_fields - 1;
            	return 0;
			}

			/* Indicate if there is something left to read */
        	return (data_in.n_fields) - (data_in.i_fields);
		}
	}

    return 0;
}

/**
 * @brief Reads data from the BME68X sensor and process it
 */
ReturnCode BME688::ProcessData(const int64_t curr_time_ns, const bme68x_data &data)
{
    bsec_input_t inputs[BSEC_MAX_PHYSICAL_SENSOR]; /* Temp, Pres, Hum & Gas */
    uint8_t n_inputs = 0;
    /* Checks all the required sensor inputs, required for the BSEC library for the requested outputs */
    if (BSEC_CHECK_INPUT(this->bsec.sensor_conf.process_data, BSEC_INPUT_HEATSOURCE))
    {
        inputs[n_inputs].sensor_id = BSEC_INPUT_HEATSOURCE;
        inputs[n_inputs].signal = this->bsec.temperature_offset;
        inputs[n_inputs].time_stamp = curr_time_ns;
        n_inputs++;
    }
    if (BSEC_CHECK_INPUT(this->bsec.sensor_conf.process_data, BSEC_INPUT_TEMPERATURE))
    {
#ifdef BME68X_USE_FPU
        inputs[n_inputs].signal = data.temperature;
#else
        inputs[n_inputs].signal = data.temperature / 100.0f;
#endif
        inputs[n_inputs].sensor_id = BSEC_INPUT_TEMPERATURE;
        inputs[n_inputs].time_stamp = curr_time_ns;
        n_inputs++;
    }
    if (BSEC_CHECK_INPUT(this->bsec.sensor_conf.process_data, BSEC_INPUT_HUMIDITY))
    {
#ifdef BME68X_USE_FPU
        inputs[n_inputs].signal = data.humidity;
#else
        inputs[n_inputs].signal = data.humidity / 1000.0f;
#endif
        inputs[n_inputs].sensor_id = BSEC_INPUT_HUMIDITY;
        inputs[n_inputs].time_stamp = curr_time_ns;
        n_inputs++;
    }
    if (BSEC_CHECK_INPUT(this->bsec.sensor_conf.process_data, BSEC_INPUT_PRESSURE))
    {
        inputs[n_inputs].sensor_id = BSEC_INPUT_PRESSURE;
        inputs[n_inputs].signal = data.pressure;
        inputs[n_inputs].time_stamp = curr_time_ns;
        n_inputs++;
    }
    if (BSEC_CHECK_INPUT(this->bsec.sensor_conf.process_data, BSEC_INPUT_GASRESISTOR) &&
            (data.status & BME68X_GASM_VALID_MSK))
    {
        inputs[n_inputs].sensor_id = BSEC_INPUT_GASRESISTOR;
        inputs[n_inputs].signal = data.gas_resistance;
        inputs[n_inputs].time_stamp = curr_time_ns;
        n_inputs++;
    }
    if (BSEC_CHECK_INPUT(this->bsec.sensor_conf.process_data, BSEC_INPUT_PROFILE_PART) &&
            (data.status & BME68X_GASM_VALID_MSK))
    {
        inputs[n_inputs].sensor_id = BSEC_INPUT_PROFILE_PART;
        inputs[n_inputs].signal = (this->sensor.last_op_mode == BME68X_FORCED_MODE) ? 0 : data.gas_index;
        inputs[n_inputs].time_stamp = curr_time_ns;
        n_inputs++;
    }

    bsec_output_t outputs[BSEC_NUMBER_OUTPUTS];
    uint8_t n_outputs;

    if (n_inputs > 0)
    {

        n_outputs = BSEC_NUMBER_OUTPUTS;
        memset(outputs, 0, sizeof(outputs));

        /* Processing of the input signals and returning of output samples is performed by bsec_do_steps() */
        this->sensor.status = bsec_do_steps( inputs, n_inputs, outputs, &n_outputs );

        if (this->sensor.status != BSEC_OK)
        {
            return ReturnCode::kSensorBsecProcessFail;
        }

        // let user know that data output is ready
        if( callback != nullptr )
        {
            this->callback(data, outputs, n_outputs);
        }
    }
    return ReturnCode::kOk;
}

ReturnCode BME688::GetChipId( uint8_t& chip_id_out )
{
    if( ReadRegister( kBme688ChipIdAddr, &chip_id_out, 1 ) != ReturnCode::kOk )
    {
        return ReturnCode::kSensorGetChipIdFail;
    }
    return ReturnCode::kOk;
}

ReturnCode BME688::ReadRegister( const uint8_t reg_addr, uint8_t* const reg_data, uint32_t length )
{
    // re-use static function from bme68x_init()
    if( SensorInternalReadRegisterCb( reg_addr, reg_data, length, this ) != 0 )
    {
        return ReturnCode::kSensorReadRegisterFail;
    }

    return ReturnCode::kOk;
}

} // namespace bme688
} // namespace sensors
} // namespace teapotlabs


