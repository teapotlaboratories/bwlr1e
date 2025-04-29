#ifndef TEAPOTLABS_PAYLOADS_ESTC_H
#define TEAPOTLABS_PAYLOADS_ESTC_H

#include "Arduino.h"

namespace teapotlabs{
namespace payload {
namespace v1 {
  /* Payload Component Definition */
  const uint8_t kVersion = 0x01;
  
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
    
    const uint8_t type = 0x10;
    public:
      // TODO: try this member initializer
      // Frame data = {{kVersion, type}};
      Frame data;
      EnvironmentalSensorTypeC( );
      size_t GetAsBytes( uint8_t* data, size_t size );
      size_t GetSize();
  };
} // namespace v1 
} // namespace payload 
} // namespace teapotlabs

#endif
