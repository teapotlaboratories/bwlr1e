#include "BME688.h"
#include "Defer.h"

static int8_t ReadRegister(uint8_t reg_addr, uint8_t *regdata, uint32_t length, void *intf_ptr);
static int8_t WriteRegister(uint8_t reg_addr,const uint8_t *reg_data, uint32_t length,void *intf_ptr);
static void delay(uint32_t period, void *intf_ptr);

BME688::BME688( PinName i2c_sda, PinName i2c_scl, uint32_t bme688_addr )
{
    this->new_data_available = 0;
    
    this->i2c_sda = i2c_sda;
    this->i2c_scl = i2c_scl;
    this->bme688_addr = bme688_addr;
    this->bme688_addr_8bit = ( bme688_addr << 1 );

    this->bsec_conf.requested_virtual_sensors_number = BSEC_REQUESTED_VIRTUAL_SENSORS_NUMBER;
    this->bsec_conf.temperature_offset = BSEC_TEMPERATURE_OFFSET;
    
    this->bsec_conf.temp_profile[0] = 320;
    this->bsec_conf.temp_profile[1] = 320;
    this->bsec_conf.dur_profile[0] = 150;
    this->bsec_conf.dur_profile[1] = 150;   
}

// static I2C i2c_debug( I2C_SDA, I2C_SCL );
BME688::ReturnCode BME688::Initialise()
{
    // this->i2c_local = &i2c_debug;    // set for debugging
    BME688::ReturnCode result;

    // enable I2C
    I2C i2c_temp( i2c_sda, i2c_scl );
    this->i2c_local = &i2c_temp;    // set i2c object for bsec to use
    Defer<I2C*> i2c_defer( this->i2c_local, nullptr );  // defer setting i2c_local to nullptr at Initialise() return

    // TODO: check chip ID    
    result = this->InitialiseSensorStructure();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    result = this->InitialiseSensorFilterSettings();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    result = this->InitialiseSensorHeaterSettings();
    if( result != ReturnCode::kOk )
    {
        return result;
    }
    
    result = this->SetSequentialMode();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    result = this->StartBsec();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    result = this->DoBsecSettings();
    if( result != ReturnCode::kOk )
    {
        return result;
    }

    return ReturnCode::kOk;
}


void BME688::DoMeasurements()
{
    // enable I2C
    I2C i2c_temp( i2c_sda, i2c_scl );
    this->i2c_local = &i2c_temp;    // set i2c object for bsec to use
    Defer<I2C*> i2c_defer( this->i2c_local, nullptr );  // defer setting i2c_local to nullptr at Initialise() return

    this->ProcessData();
    this->BsecProcessing();
}


BME688::ReturnCode BME688::InitialiseSensorStructure()
{
    bsec_conf.sensor_structure.intf_ptr =  this;  
    bsec_conf.sensor_structure.intf     =  BME68X_I2C_INTF;
    bsec_conf.sensor_structure.read     =  ReadRegister;
    bsec_conf.sensor_structure.write    =  WriteRegister;
    bsec_conf.sensor_structure.delay_us =  delay;
    
    if( bme68x_init(&bsec_conf.sensor_structure) != 0 )
    {
        return BME688::ReturnCode::kSensorStructureFail;
    }
    return BME688::ReturnCode::kOk;
}

BME688::ReturnCode BME688::InitialiseSensorFilterSettings()
{
    bme68x_get_conf(&bsec_conf.sensor_config, &bsec_conf.sensor_structure);
    bsec_conf.sensor_config.filter = BME68X_FILTER_OFF;
    bsec_conf.sensor_config.odr = BME68X_ODR_NONE;
    bsec_conf.sensor_config.os_hum = BME68X_OS_16X;
    bsec_conf.sensor_config.os_pres = BME68X_OS_1X;
    bsec_conf.sensor_config.os_temp = BME68X_OS_2X;

    if( bme68x_set_conf(&bsec_conf.sensor_config, &bsec_conf.sensor_structure) != 0 )
    {
        return BME688::ReturnCode::kSensorConfigFail;
    }
    return BME688::ReturnCode::kOk;
}

BME688::ReturnCode BME688::InitialiseSensorHeaterSettings()
{
    bsec_conf.sensor_heater_config.enable = BME68X_ENABLE;
    bsec_conf.sensor_heater_config.heatr_temp_prof = bsec_conf.temp_profile;
    bsec_conf.sensor_heater_config.heatr_dur_prof = bsec_conf.dur_profile;
    bsec_conf.sensor_heater_config.profile_len = 1;
    
    if( bme68x_set_heatr_conf(BME68X_SEQUENTIAL_MODE, &bsec_conf.sensor_heater_config, &bsec_conf.sensor_structure) != 0 )
    {
        return BME688::ReturnCode::kSensorHeaterFail;
    }
    return BME688::ReturnCode::kOk;
}


BME688::ReturnCode BME688::SetSequentialMode()
{    
    if( bme68x_set_op_mode(BME68X_SEQUENTIAL_MODE, &bsec_conf.sensor_structure) != 0 )
    {
        return BME688::ReturnCode::kSensorOperationSeqFail;
    }
    return BME688::ReturnCode::kOk;
}

BME688::ReturnCode BME688::StartBsec()
{
    if( bsec_init() != 0 )
    {
        return BME688::ReturnCode::kSensorBsecFail;
    }
    return BME688::ReturnCode::kOk;
}

BME688::ReturnCode BME688::DoBsecSettings()
{
    bsec_conf.requested_virtual_sensors[0].sensor_id = BSEC_OUTPUT_IAQ;
    bsec_conf.requested_virtual_sensors[0].sample_rate = BSEC_SAMPLE_RATE_LP;
    bsec_conf.requested_virtual_sensors[1].sensor_id = BSEC_OUTPUT_CO2_EQUIVALENT;
    bsec_conf.requested_virtual_sensors[1].sample_rate = BSEC_SAMPLE_RATE_LP;
    bsec_conf.requested_virtual_sensors[2].sensor_id = BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE;
    bsec_conf.requested_virtual_sensors[2].sample_rate = BSEC_SAMPLE_RATE_LP;
    bsec_conf.requested_virtual_sensors[3].sensor_id = BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY;
    bsec_conf.requested_virtual_sensors[3].sample_rate = BSEC_SAMPLE_RATE_LP;
    bsec_sensor_configuration_t requiredSensorSettings[BSEC_MAX_PHYSICAL_SENSOR];
    bsec_conf.number_required_sensor_settings = BSEC_MAX_PHYSICAL_SENSOR;

    if( bsec_update_subscription( bsec_conf.requested_virtual_sensors, bsec_conf.requested_virtual_sensors_number,
                                  requiredSensorSettings, &bsec_conf.number_required_sensor_settings ) != 0 )
    {
        return BME688::ReturnCode::kSensorBsecSubscriptionFail;
    }
    return BME688::ReturnCode::kOk;
}

BME688::ReturnCode BME688::ProcessData()
{
    uint32_t delayInUs = bme68x_get_meas_dur(BME68X_SEQUENTIAL_MODE, &bsec_conf.sensor_config, &bsec_conf.sensor_structure) + (bsec_conf.sensor_heater_config.heatr_dur_prof[0] * 1000);
    delay(delayInUs,NULL);
    if( bme68x_get_data(BME68X_SEQUENTIAL_MODE, bsec_conf.sensor_data, &bsec_conf.data_fields, &bsec_conf.sensor_structure) != 0 )
    {
        return BME688::ReturnCode::kSensorGetDataFail;
    }
    return BME688::ReturnCode::kOk;
}


void BME688::BsecProcessing()
{
    if (!(bsec_conf.sensor_data[bsec_conf.data_fields - 1].status & BME68X_NEW_DATA_MSK))
        return;
    bsec_input_t inputs[BSEC_MAX_PHYSICAL_SENSOR];
    uint8_t nInputs = 0;;
    bsec_conf.current_time_ns = Kernel::get_ms_count() * INT64_C(1000000);
    inputs[nInputs].sensor_id = BSEC_INPUT_TEMPERATURE;
    inputs[nInputs].signal = bsec_conf.sensor_data[bsec_conf.data_fields - 1].temperature;
    inputs[nInputs].time_stamp = bsec_conf.current_time_ns;
    nInputs++;

    inputs[nInputs].sensor_id = BSEC_INPUT_HUMIDITY;
    inputs[nInputs].signal = bsec_conf.sensor_data[bsec_conf.data_fields - 1].humidity;
    inputs[nInputs].time_stamp = bsec_conf.current_time_ns;
    nInputs++;

    inputs[nInputs].sensor_id = BSEC_INPUT_PRESSURE;
    inputs[nInputs].signal = bsec_conf.sensor_data[bsec_conf.data_fields - 1].pressure;
    inputs[nInputs].time_stamp = bsec_conf.current_time_ns;
    nInputs++;

    inputs[nInputs].sensor_id = BSEC_INPUT_GASRESISTOR;
    inputs[nInputs].signal = bsec_conf.sensor_data[bsec_conf.data_fields - 1].gas_resistance;
    inputs[nInputs].time_stamp = bsec_conf.current_time_ns;
    nInputs++;

    inputs[nInputs].sensor_id = BSEC_INPUT_HEATSOURCE;
    inputs[nInputs].signal = bsec_conf.temperature_offset;
    inputs[nInputs].time_stamp = bsec_conf.current_time_ns;

    
    uint8_t nOutputs = 0;
    nOutputs = BSEC_NUMBER_OUTPUTS;

    
    bsec_output_t outputStorage[BSEC_NUMBER_OUTPUTS];

    auto status = bsec_do_steps(inputs, nInputs, outputStorage, &nOutputs);
    if (status != BSEC_OK)
    {
        return;
    }
    if (nOutputs > 0)
    {
        new_data_available = true;
        for (uint8_t i = 0; i < nOutputs; i++)
        {
            switch (outputStorage[i].sensor_id)
            {
            case BSEC_OUTPUT_IAQ:
                sensor_data.iaq = outputStorage[i].signal;
                sensor_data.iaq_accuracy = outputStorage[i].accuracy;
                break;
            case BSEC_OUTPUT_CO2_EQUIVALENT:
                sensor_data.co2 = outputStorage[i].signal;
                sensor_data.co2_accuracy = outputStorage[i].accuracy;
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
                sensor_data.temperature = outputStorage[i].signal;
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
                sensor_data.humidity = outputStorage[i].signal;
                break;
            default:
                break;
            }
        }
    }
    else
    {
        new_data_available = false;
    }
}

void BME688::DumpData()
{
    printf("Temperature:%2.1f *C\n\r",sensor_data.temperature);
    printf("Humidity:%2.1f\n\r",sensor_data.humidity);
    printf("CO2:%2.1f\n\r",sensor_data.co2);
    printf("CO2 Accuracy:%d\n\r",sensor_data.co2_accuracy);
    printf("IAQ:%2.1f\n\r",sensor_data.iaq);
    printf("IAQ Accuracy:%d",sensor_data.iaq_accuracy);
}

bool BME688::IsNewDataAvailable()
{
    return new_data_available;    
}


BME688::SensorData BME688::GetLatest()
{
    return sensor_data;
}

// Static function

int8_t ReadRegister(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
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


static int8_t WriteRegister(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
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


static void delay(uint32_t period, void *intf_ptr)
{
    wait_us ( period );
}


