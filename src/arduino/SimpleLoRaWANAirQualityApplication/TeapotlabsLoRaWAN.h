#ifndef TEAPOTLABS_LORAWAN_H
#define TEAPOTLABS_LORAWAN_H

#include "Arduino.h"

namespace teapotlabs{
namespace connection {

  class LorawanCallback{
    public:
      virtual ~LorawanCallback() {}
      virtual void NotifyNetworkJoin( bool joined ) = 0;

    protected:
      LorawanCallback() {}
  };

  enum class ReturnCode {
    kOk = 0,
    kError,
    kInvalidAppEui,
    kInvalidAppKey,
    kInvalidDeui,
    kInvalidBand,
    kFailJoinNetwork,
    kFailSendPayload,
    kNullpointer,
    kTimeout,
  };
  
  class Lorawan {
    private:
      // OTAA Device EUI MSB first
      uint8_t device_eui[8];
      // OTAA Application EUI MSB first
      uint8_t app_eui[8];
      // OTAA Application Key MSB first
      uint8_t app_key[16];
      // LoRa Frequency
      RAK_LORA_BAND band;

      uint8_t rejoin_retry;
      uint8_t send_request_retry;
      bool initialized = false;
      
      static void ReceiveCb( SERVICE_LORA_RECEIVE_T * data );
      static void JoinCb( int32_t status );
      void (*send_cb)(int32_t);
    public:
      Lorawan( const uint8_t* device_eui, uint8_t device_eui_size,
               const uint8_t* app_eui, uint8_t app_eui_size,
               const uint8_t* app_key,uint8_t app_key_size,
               RAK_LORA_BAND band,
               uint8_t rejoin_retry = 3,
               uint8_t send_request_retry = 3,
               void (*send_cb)(int32_t) = nullptr );
      ReturnCode Connect( LorawanCallback& notify_network_join_wait );
      ReturnCode Send(uint8_t* payload, size_t size);
      bool IsInitialized();
  };
  
} // namespace connection 
} // namespace teapotlabs

#endif // TEAPOTLABS_LORAWAN_H
