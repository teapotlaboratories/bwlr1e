#ifndef TEAPOTLABS_BWLR1E_H
#define TEAPOTLABS_BWLR1E_H

#include "Arduino.h"
#include "Zanshin_BME680.h"

// configuration for teapot bwlr3d application
#include "TeapotlabsBWLR1EConfig.h"

/* pin definition */
#define RED_LED               PA15
#define GREEN_LED             PA1
#define POWER_STATUS          PB12
#define I2C_SDA               PA11
#define I2C_SCL               PA12
#define BATT_MEASURE          PB3

#ifndef ADC_TO_BATTERY
  #error "ADC_TO_BATTERY not set"
#endif

/* application definition */
namespace teapotlabs{
namespace bwlr1e {
  enum class ReturnCode {
    kOk = 0,
    kError,
    kBme68xNotFound,
    kInvalidLed,
    kPeripheralOff,
    kBme68xReadFail,
    kTimeout,
  };
  
  enum class Led {
    kGreen = 0,
    kRed,
  };

  class Application {
    private:
      BME680_Class      bme68x;
    
      void  SetupBme68x();
    public:
      Application();
      void          Initialize();
      ReturnCode    ConfigureSensor();

      /**
       * Enable or disable LED
       *
       * @param led target led Target HDOP value
       * @param enable enable or disable led
       * @return `kOk` if successfully set LED
       */
      ReturnCode    EnableLed( Led led, bool enable );
      int           ReadBatteryAdc();
      float         ReadBatteryVoltage();
      bool          ReadPowerStatus();
      ReturnCode    GetEnvironmentalData( float& temperature, float& humidity, float& pressure );
      void          BlinkLedIndicator( uint32_t ms );
      void          BlinkLedIndicator(Led led, uint32_t ms);
      void          BlinkLedIndicator(Led led, int total_blink, uint32_t ms);
  };
  
} // namespace bwlr3d 
} // namespace teapotlabs

#endif
