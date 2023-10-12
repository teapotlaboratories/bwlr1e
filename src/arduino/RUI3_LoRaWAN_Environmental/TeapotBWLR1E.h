#ifndef TEAPOT_BWLR1E_H
#define TEAPOT_BWLR1E_H

#include "Arduino.h"
#include "Zanshin_BME680.h"

// configuration for teapot bwlr3d application
#include "TeapotBWLR1EConfig.h"

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
namespace teapot{
namespace bwlr1e {
  enum class ReturnCode {
    kOk = 0,
    kError,
    kLis3mdlNotFound,
    kLsm6dsoxNotFound,
    kBme68xNotFound,
    kVeml7700NotFound,
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
} // namespace teapot


/* lora payload definition */
namespace teapot{
namespace payload {
namespace v1 {
  /* Payload Component Definition */
  const uint8_t kVersion = 0x01;
  enum class Type {
    kEstc = 0x10
  };
  
  struct Header {
    uint8_t version;
    uint8_t type;
  } __attribute__((packed));

  /* Payload Definition */  
  /* environmental payload */
  class EnvironmentalSensorTypeC {
    struct Frame {
      Header header;
      uint8_t iaq_accuracy;
      float iaq;
      float static_iaq;
      float co2;
      int16_t temperature;
      uint16_t humidity;
      uint16_t pressure;
      uint16_t voltage;
    } __attribute__((packed));
    
    const Type type = teapot::payload::v1::Type::kEstc;
    public:
      Frame data;
      EnvironmentalSensorTypeC( );
      size_t GetAsBytes( uint8_t* data, size_t size );
      size_t GetSize();
  };
} // namespace v1 
} // namespace payload 
} // namespace teapot

#endif
