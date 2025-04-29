#include "TeapotlabsLoRaWAN.h"

namespace teapotlabs {
namespace connection {

  Lorawan::Lorawan( const uint8_t* device_eui, uint8_t device_eui_size,
                    const uint8_t* app_eui, uint8_t app_eui_size,
                    const uint8_t* app_key, uint8_t app_key_size,
                    RAK_LORA_BAND band,
                    uint8_t rejoin_retry,
                    uint8_t send_request_retry,
                    void (*send_cb)(int32_t) )
  {
    if( device_eui == nullptr || device_eui_size != sizeof(this->device_eui) ||
        app_eui    == nullptr || app_eui_size    != sizeof(this->app_eui)    ||
        app_key    == nullptr || app_key_size    != sizeof(this->app_key) ){
      this->initialized = false;
      return;
    }

    memcpy(this->device_eui, device_eui, sizeof(this->device_eui));
    memcpy(this->app_eui, app_eui, sizeof(this->app_eui));
    memcpy(this->app_key, app_key, sizeof(this->app_key));

    this->rejoin_retry = rejoin_retry;
    this->send_request_retry = send_request_retry;
    this->band = band;
    this->send_cb = send_cb;
    this->initialized = true;
  }
  
  bool Lorawan::IsInitialized()
  {
    return this->initialized;
  }
  
  ReturnCode Lorawan::Connect( LorawanCallback& notify_network_join_wait )
  {
    if (!api.lorawan.appeui.set(this->app_eui, 8)) {
      return ReturnCode::kInvalidAppEui;
    }
    if (!api.lorawan.appkey.set(this->app_key, 16)) {
      return ReturnCode::kInvalidAppKey;
    }
    if (!api.lorawan.deui.set(this->device_eui, 8)) {
      return ReturnCode::kInvalidDeui;
    }
  
    if (!api.lorawan.band.set(this->band)) {
      return ReturnCode::kInvalidBand;
    }

    api.lorawan.deviceClass.set(RAK_LORA_CLASS_A);
    api.lorawan.njm.set(RAK_LORA_OTAA);
    
    if (!api.lorawan.join())  // Join to Gateway
    {
      return ReturnCode::kFailJoinNetwork;
    }

    // wait for join success
    while(api.lorawan.njs.get() == false) 
    {
      api.lorawan.join();
      delay(10000);
      notify_network_join_wait.NotifyNetworkJoin( api.lorawan.njs.get() );
    }

    // enable ADR
    api.lorawan.adr.set(true);
    
    // enable send retry
    api.lorawan.rety.set(3);

    // enable send confirmation
    api.lorawan.cfm.set(1);

    // register callbacks
    if( this->send_cb != nullptr )
    {
      api.lorawan.registerSendCallback(this->send_cb); 
    }
    return ReturnCode::kOk;
  }

  ReturnCode Lorawan::Send(uint8_t* payload, size_t size)
  {
    int retry = this->rejoin_retry;
    
    // If not part of a network, do rejoin
    while (api.lorawan.njs.get() == 0) {
      api.lorawan.join();
//      Serial.println("waiting to join network");
      api.system.sleep.all(10000);
      if (retry-- <= 0) {
        break;
      }    
    }
  
    if(retry > 0 ){
      /** Send the data package */
      uint8_t try_send_request = this->send_request_retry;
      while( !api.lorawan.send(size, payload, 1) && try_send_request--) {
//       Serial.println("retry sending payload");
       delay(1000);
      }
      
      if (!try_send_request){  
        return ReturnCode::kFailSendPayload;
      } else { 
        return ReturnCode::kOk;
      }
    }
    
    return ReturnCode::kFailJoinNetwork;
  }
  
} // namespace connection
} // namespace teapotlabs
