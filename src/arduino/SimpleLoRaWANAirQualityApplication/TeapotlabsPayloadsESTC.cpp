#include "TeapotlabsPayloadsESTC.h"

namespace teapotlabs{
namespace payload {
namespace v1 {
  /* environmental payload implementation*/
  EnvironmentalSensorTypeC::EnvironmentalSensorTypeC( )
  {
    // set data
    this->data.header.version = kVersion;
    this->data.header.type = this->type;
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
} // namespace teapotlabs
