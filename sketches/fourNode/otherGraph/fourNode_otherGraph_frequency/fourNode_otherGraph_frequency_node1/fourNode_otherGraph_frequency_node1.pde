#include <Streaming.h>
#include <XBee.h>
#include <Dyno.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

Dyno d = Dyno();
XBee xbee = XBee();   
ZBRxResponse rx = ZBRxResponse();
// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x404980AE,0.1*base,2.5*base,-2.1667*base,0.1667*base,2,base);
OGraph g = OGraph(&s);
OAgent a = OAgent(&xbee,&rx,&g,true,true);

uint8_t errorPin = 6;  // error led pin
uint8_t sPin = 7;      // synced led
uint8_t cPin = 48;     // coordination enabled led pin

boolean de = false;
boolean ce = false;

void setup()  {
  pinMode(cPin, OUTPUT);
  pinMode(sPin, OUTPUT);
  digitalWrite(cPin,HIGH);
  digitalWrite(sPin,HIGH);
  Serial.begin(57600);
  Serial3.begin(57600);
  d.setSerial(Serial);
  xbee.setSerial(Serial3);
  //d.setTimeout(10000);
  g.addInNeighbor(0x40498074);   // node 2
  g.addInNeighbor(0x40498060);   // node 3
  g.addNonNeighbor(0x4068B69B);  // node 7
  digitalWrite(cPin,LOW);
  digitalWrite(sPin,LOW);
}

void loop() {
  if(de == false) {
    if(Serial.available()) {
      uint8_t b = Serial.read();
      if (b == 'd') {
        de = true;
        if(a.sync()) {
          digitalWrite(sPin,HIGH);
          d.flushSerial();
        }
      }
    }
  } else {
    if(ce == false) {
      if(Serial.available()) {
        uint8_t b = Serial.read();
        if (b == 'c') {
          ce = true;
        }
      }
    }
    if(true && ce == true) {
      a.leaderFairSplitRatioConsensusWithDyno(d,75,50,cPin);
      ce = false;
    }
  }
}
