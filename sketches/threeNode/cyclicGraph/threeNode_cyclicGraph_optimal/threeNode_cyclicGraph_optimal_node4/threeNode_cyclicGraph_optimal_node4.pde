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
OLocalVertex s = OLocalVertex(0x40498065,0*base,4*base,-2.1667*base,0.1667*base,1,base);
OGraph g = OGraph(&s);
OAgent a = OAgent(&xbee,&rx,&g,true,true);

uint8_t sPin = 7;      // synced led
uint8_t cPin = 48;     // coordination enabled led pin

boolean de = false;
boolean ce = false;

void setup()  {
  pinMode(sPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  digitalWrite(sPin,HIGH);
  digitalWrite(cPin,HIGH);
  Serial.begin(57600);
  Serial3.begin(57600);
  d.setSerial(Serial);
  //d.setTimeout(10000);
  xbee.setSerial(Serial3);
  g.addInNeighbor(0x405D9805);   // node 5
  g.addNonNeighbor(0x405D9822);  // node 6
  digitalWrite(sPin,LOW);
  digitalWrite(cPin,LOW);
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
        if (b == 'o') {
          ce = true;
        }
      }
    }
    if(a.isSynced() && ce == true) {
      a.leaderOptimalDispatchWithDyno(d,125,300,cPin);
//      float t = 1.37;
//      long initial = long(t*float(base));
//      Serial << "initial: " << _DEC(initial) << endl;
//      long final = a.leaderOptimalDispatch(initial,75,250,cPin);
//      Serial << "final: " << _DEC(final) << endl;
      ce = false;
    }
  }
}
