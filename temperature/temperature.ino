/*
 *  The circuit:   
 *    
 *    Power XBee:
 *      GND of Arduino to GND of XBee: 10
 *      3.3v of Arduino to 3.3v of XBee: 1
 *  
 *    Arduino to XBee:
 *      RX is digital pin 2 of Arduino (connect to TX of XBee: 2)
 *      TX is digital pin 3 of Arduino (connect to RX of XBee: 3)
 *      SLEEP_PIN is digital pin 4 of arduino (connect to SLEEP_RQ of XBee: 9)
 *        
 *    Atmega328P to XBee:
 *      RX is digital pin 2 = pin 4 of Atmega328P (connect to TX of XBee: 2)
 *      TX is digital pin 3 = pin 5 of Atmega328P (connect to RX of XBee: 3)
 *      SLEEP_PIN is digital pin 4 = pin 6 of Atmega328P (connect to SLEEP_RQ of XBee: 9)
 *      
 *    Atmega328P to LED:
 *      LED_BUILTIN is digital pin 13 = pin 19 of Atmega328P (connect to + LED)
 */
 
#include <Adafruit_SHT31.h>
#include <SoftwareSerial.h>
#include <XBee.h>

const int SLEEP_PIN = 4;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

SoftwareSerial ss_xbee(2, 3); // RX, TX
XBee xbee = XBee();

void setup() {
  //Serial.begin(9600);
  
  // SHT31
  sht31.begin(0x44);

  // XBee
  ss_xbee.begin(9600);
  xbee.setSerial(ss_xbee);

  // Sleep pin
  pinMode(SLEEP_PIN, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Wake up XBee module
  digitalWrite(SLEEP_PIN, LOW);

  // Show visually that we are reading/emitting
  digitalWrite(LED_BUILTIN, HIGH);
    
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    String s = String(t) + ";" + String(h);
    
    //Serial.println(s);
    //Serial.println(s.length());

    uint8_t data[s.length()+1];
    s.getBytes(data, sizeof(data));
    ZBTxRequest f = ZBTxRequest(XBeeAddress64(), data, sizeof(data));
    xbee.send(f);
  } else {
    //Serial.println("Failed to read temperature or humidity");
  }

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(SLEEP_PIN, HIGH);
  delay(5000);
}
