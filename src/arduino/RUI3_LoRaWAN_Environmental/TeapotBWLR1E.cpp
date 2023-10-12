#include "TeapotBWLR1E.h"

/* application implementation */
namespace teapot {
namespace bwlr1e {
  
  Application::Application(){}
  void Application::Initialize()
  {    
    // setup power status
    pinMode(POWER_STATUS, INPUT);

    // setup battery read measure
    analogReadResolution(12);
    pinMode(BATT_MEASURE, INPUT);
    
    // setup led pin
    pinMode( RED_LED, OUTPUT );
    EnableLed( Led::kRed, false );
    pinMode( GREEN_LED, OUTPUT );
    EnableLed( Led::kGreen, false );
  }

  int Application::ReadBatteryAdc()
  {
    return analogRead( BATT_MEASURE );
  }

  float Application::ReadBatteryVoltage()
  {
    return ((float) ReadBatteryAdc()) * ADC_TO_BATTERY;
  }
 
  ReturnCode Application::EnableLed( Led led, bool enable )
  {
    switch( led ){
      case Led::kRed:
      {
        digitalWrite( RED_LED, (enable ? HIGH : LOW) );
      }
      case Led::kGreen:
      {
        digitalWrite( GREEN_LED, (enable ? HIGH : LOW) );
      }
      default:
      {
        return ReturnCode::kInvalidLed;
      }
    }
    return ReturnCode::kOk;
  }
  

  bool Application::ReadPowerStatus()
  {
    return digitalRead(POWER_STATUS);
  }
  
  void Application::SetupBme68x()
  {  
    // Set up oversampling and filter initialization
    this->bme68x.setOversampling(TemperatureSensor, Oversample16);  // Use enumerated type values
    this->bme68x.setOversampling(HumiditySensor, Oversample16);     // Use enumerated type values
    this->bme68x.setOversampling(PressureSensor, Oversample16);     // Use enumerated type values
    this->bme68x.setIIRFilter(IIR4);  // Use enumerated type values
    this->bme68x.setGas(320, 150);  // 320c for 150 milliseconds
  }
 
  ReturnCode Application::ConfigureSensor()
  {
    /* configure BME68x */
    if ( !(this->bme68x.begin()) ) {
      return ReturnCode::kBme68xNotFound;
    }
    SetupBme68x();
  
    return ReturnCode::kOk;
  }
  
  ReturnCode Application::GetEnvironmentalData( float& temperature, float& humidity, float& pressure )
  {    
   
    int32_t temp = 0, humid = 0, prsr = 0, gas = 0;
    
    // TODO: improve BME68x reading. Currently first read always fails.;
    for( int i = 0; i < 2; i++)
    {
      this->bme68x.getSensorData(temp, humid, prsr, gas, true);
    }

    temperature      = static_cast<float>(temp) / 100;
    pressure         = static_cast<float>(prsr) / 100;
    humidity         = static_cast<float>(humid) / 1000;
        
    return ReturnCode::kOk;
  }

  void Application::BlinkLedIndicator( uint32_t ms )
  {  
    EnableLed(Led::kGreen, true);
    EnableLed(Led::kRed, true);
    delay(ms);
    EnableLed(Led::kGreen, false);
    EnableLed(Led::kRed, false);
  } 
  
  void Application::BlinkLedIndicator(Led led, uint32_t ms)
  {  
    EnableLed( led, true );
    delay(ms);
    EnableLed( led, false );
  } 
  
  void Application::BlinkLedIndicator(Led led, int total_blink, uint32_t ms)
  {
    for( int i = 0; i < total_blink; i++ )
    {      
      EnableLed( led, true );
      delay(ms);
      EnableLed( led, false );
      delay(ms);
    }
  }
  
} // namespace bwlr1e 
} // namespace teapot

/* lora payload implementation */
namespace teapot{
namespace payload {
namespace v1 {
  /* environmental payload implementation*/
  EnvironmentalSensorTypeC::EnvironmentalSensorTypeC( )
  {
    // set data
    this->data.header.version = kVersion;
    this->data.header.type = static_cast<uint32_t>(this->type);
  }
  
  size_t EnvironmentalSensorTypeC::GetAsBytes(uint8_t* data, size_t size)
  {
    if( data == nullptr || size < sizeof(this->data) )
    {
      return 0;
    }
    
    memcpy( data, &(this->data), sizeof(this->data) );
    return( sizeof(this->data) );
  }

  size_t EnvironmentalSensorTypeC::GetSize()
  {
    return( sizeof(this->data) );
  }

  



} // namespace v1 
} // namespace payload 
} // namespace teapot
