#include "mbed.h"
#include "bme688/BME688.h"

#define ARRAY_LEN(array)    (sizeof(array)/sizeof(array[0]))

#define FEATHER_RAK3172
// #define BWLR1E

// define on-board led to use
#ifdef FEATHER_RAK3172
mbed::DigitalOut led0(PA_8);
#endif
#ifdef BWLR1E
mbed::DigitalOut led0(PA_15);
mbed::DigitalOut led1(PA_1);
#endif

static void data_available( const bme68x_data data, bsec_output_t* const outputs, const uint8_t n_outputs )
{
    if (!n_outputs)
    {
        return;
    }

    printf("BSEC outputs:\n\ttimestamp = %d\r\n", (int) (outputs[0].time_stamp / INT64_C(1000000)) );
    for (uint8_t i = 0; i < n_outputs; i++)
    {
        const bsec_output_t output  = outputs[i];
        switch (output.sensor_id)
        {
            case BSEC_OUTPUT_IAQ:
                printf("\tiaq = %f\r\n", output.signal );
                printf("\tiaq accuracy = %d\r\n",(int) output.accuracy );
                break;
            case BSEC_OUTPUT_RAW_TEMPERATURE:
                printf("\ttemperature = %f\r\n", output.signal );
                break;
            case BSEC_OUTPUT_RAW_PRESSURE:
                printf("\tpressure = %f\r\n", output.signal );
                break;
            case BSEC_OUTPUT_RAW_HUMIDITY:
                printf("\thumidity = %f\r\n", output.signal );
                break;
            case BSEC_OUTPUT_RAW_GAS:
                printf("\tgas resistance = %f\r\n", output.signal );
                break;
            case BSEC_OUTPUT_STABILIZATION_STATUS:
                printf("\tstabilization status = %f\r\n", output.signal );
                break;
            case BSEC_OUTPUT_RUN_IN_STATUS:
                printf("\trun in status = %f\r\n", output.signal );
                break;
            default:
                break;
        }
    }
}

int main()
{

// turn-off led for now
    led0 = 0;
#ifdef BWLR1E
    led01 = 0;
#endif

    // initialise bme688
    BME688 iaq_sensor( I2C_SDA, I2C_SCL, BME68X_I2C_ADDR_HIGH );

    /* Desired subscription list of BSEC2 outputs */
    bsec_virtual_sensor_t sensor_list[] = {
            BSEC_OUTPUT_IAQ,
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_STABILIZATION_STATUS,
            BSEC_OUTPUT_RUN_IN_STATUS
    };

    // for Low Power mode, refreshed every 3s
    // auto sample_rate = BSEC_SAMPLE_RATE_LP;

    // for Ultra-Low Power mode, refreshed every 300s
    auto sample_rate = BSEC_SAMPLE_RATE_ULP;

    BME688::ReturnCode result = iaq_sensor.Initialise( sensor_list, ARRAY_LEN(sensor_list), sample_rate );
    if( result != BME688::ReturnCode::kOk )
    {
        while( true )
        {
            /* blink led infinity to indicate bme688
               fail to initialise */
            ThisThread::sleep_for(500ms);
            led0 = !led0;
        }
    }

    iaq_sensor.SetCallback( data_available );

    // start measurement thread
    while ( true )
    {
        // get measurement
        result = iaq_sensor.Run();
        if( result != BME688::ReturnCode::kOk )
        {
            printf("fail to call bsec run(), err: %d \n\r", result);
        }

        // print current time in nanoseconds
        int64_t curr_time_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(Kernel::Clock::now()).time_since_epoch().count();
        printf("      current time? %lld ns\r\n", curr_time_ns);

        // print time to call next run
        printf("when to call run()? %lld ns\r\n", iaq_sensor.GetNextRunTimeNs());

        // target sleep time, for the system to sleep
        int64_t target_sleep_time_ms = (iaq_sensor.GetNextRunTimeNs()/INT64_C(1000000)) - 
                                       (curr_time_ns/INT64_C(1000000));
        printf("target sleep time? %lld ms\r\n", target_sleep_time_ms);

        // sleep until next scheduled Run();
        // NOTE: Full SRAM retention in deep-sleep (STOP2)
        ThisThread::sleep_for( std::chrono::milliseconds(target_sleep_time_ms) ); 
    }
}