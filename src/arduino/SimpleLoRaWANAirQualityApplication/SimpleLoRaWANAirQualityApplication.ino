#include "TeapotlabsBWLR1E.h"
#include "TeapotlabsLoRaWAN.h"

/* forward-declaration */
void SendCb( int32_t status );

#define PERIOD_MINUTE 60 * 1000UL

/* LoRaWAN configuration */
const uint8_t kOtaaDevEui[8]  = {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0x34, 0xB3};
const uint8_t kOtaaAppEui[8]  = {0x0E, 0x0D, 0x0D, 0x01, 0x0E, 0x01, 0x02, 0x0E};
const uint8_t kOtaaAppKey[16] = {0x3d, 0xbe, 0x8c, 0xc0, 0x87, 0x77, 0x5c, 0x9e, 0x83, 0xba, 0x7c, 0x5e, 0x9a, 0x88, 0x96, 0x98};
const RAK_LORA_BAND kOtaaBand = RAK_REGION_US915;
const uint32_t kOtaaPeriodMs  = (2 * PERIOD_MINUTE ); // 2 minute

/* main application objects */
teapotlabs::bwlr1e::Application app;
teapotlabs::connection::Lorawan lora( kOtaaDevEui, sizeof(kOtaaDevEui),
                                      kOtaaAppEui, sizeof(kOtaaAppEui),
                                      kOtaaAppKey, sizeof(kOtaaAppKey),
                                      kOtaaBand,3, 5, nullptr );

void setup(void) {
  api.system.restoreDefault();  
  Serial.begin(115200);

  // initialize hardware
  app.Initialize();
  app.ConfigureSensor();

  // connect to LoRaWAN network
  if( !lora.IsInitialized() ){
    while(true) {
      Serial.println("fail to initialize connection");
      delay( 5000 );
    }
  }
  
  class NotifyLorawanNetworkJoin : public teapotlabs::connection::LorawanCallback {
    public:
      NotifyLorawanNetworkJoin() = default;
      void NotifyNetworkJoin(bool joined){
        Serial.printf("Trying to join. Is joined? %d\r\n", joined);

      }
  };
  NotifyLorawanNetworkJoin notify_join;
  teapotlabs::connection::ReturnCode ret = lora.Connect( notify_join );
  if( ret != teapotlabs::connection::ReturnCode::kOk ){
    while(true) {
      Serial.printf("fail to connect to gateway. ret: %d\r\n", ret);
      delay( 5000 );
    }
  }

}

void loop() 
{  
  /* prepare packet */
  uint8_t payload[128];
  size_t  out_payload_size = 0;
  out_payload_size = sizeof(payload);

  /* send payload and wait for process to complete */
  lora.Send(payload, out_payload_size);

  /* sleep device */
  Serial.printf("Try sleep %ums..\r\n", kOtaaPeriodMs);
  api.system.sleep.all(kOtaaPeriodMs);
  Serial.println("Wakeup..");

}
