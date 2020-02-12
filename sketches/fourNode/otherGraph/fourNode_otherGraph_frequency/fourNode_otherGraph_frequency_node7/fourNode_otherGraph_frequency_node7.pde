#include <Streaming.h>
#include <XBee.h>
#include <Dyno.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

Dyno d = Dyno();
XBee xbee = XBee();                  // create an XBee object
ZBRxResponse rx = ZBRxResponse();
// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x4068B69B,0.1*base,0.95*base,-4.25*base,0.25*base,1,base);
OGraph g = OGraph(&s);
OAgent a = OAgent(&xbee,&rx,&g,false,true);

uint8_t cPin = 48;  // drive enable led
uint8_t sPin = 7;   // synced led

boolean de = false;

void setup()  {
  pinMode(sPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  digitalWrite(sPin,HIGH);
  digitalWrite(cPin,HIGH);
  Serial.begin(57600);
  Serial3.begin(57600);
  d.setSerial(Serial);
  xbee.setSerial(Serial3);
  //d.setTimeout(10000);
  g.addInNeighbor(0x40498074);   // node 2
  g.addInNeighbor(0x40498060);   // node 3
  g.addNonNeighbor(0x404980AE);  // node 1
  digitalWrite(sPin,LOW);
  digitalWrite(cPin,LOW);
}

void loop() {
  if(de == false) {
    if(a.sync()) {
      de = true;
      d.flushSerial();
      digitalWrite(sPin,HIGH);
    }    
  } else {
    if(a.isSynced()) {
      a.reserveFairSplitRatioConsensusWithDyno(d,2*base,cPin);
    }
  }
}
