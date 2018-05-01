#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial ss_xbee(2, 3); // RX, TX

XBee xbee = XBee();
//XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();

const int relayPin = 4;
bool relayOn = true;

void setup() {
  Serial.begin(9600);
  //Serial.println("Starting...");

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  
  
  // XBee
  ss_xbee.begin(9600);
  xbee.setSerial(ss_xbee);
  
  //Serial.println("Started!");
}

void loop() {
  bool stateNeedUpdate = false;

  // Why can I only read one message and then only errors?
  //xbee.getResponse().reset();
  //xbee.readPacketUntilAvailable();

  xbee.readPacket();
  
  if (xbee.getResponse().isAvailable()) {
    //Serial.println("got something");
    
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      Serial.println("got a zb rx packet");        
      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);
          
      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        String data = String((char*)rx.getData());
        Serial.println(data);
        if (data = "SWITCH") {
          relayOn = !relayOn;
          stateNeedUpdate = true;
        }
      }
    }   
  }

  if (stateNeedUpdate) {
    if (relayOn) {
      digitalWrite(relayPin, HIGH);
    } else {
      digitalWrite(relayPin, LOW);
    }   
    delay(500); 
  }
}


