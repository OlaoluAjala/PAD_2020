#include <Streaming.h>
#include <Dyno.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

Dyno d = Dyno();
XBee xbee = XBee();                  // create an XBee object
ZBRxResponse rx = ZBRxResponse();
// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x405D9805,0*base,2.5*base,-1.5*base,0.5*base,1,base);
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
  Serial2.begin(57600);
  d.setSerial(Serial);
  //d.setTimeout(10000);
  xbee.setSerial(Serial2);
  g.addInNeighbor(0x405D9822); // node 6
  g.addNonNeighbor(0x40498065); // node 4
  digitalWrite(sPin,LOW);
  digitalWrite(cPin,LOW);
}

void loop() {
  if(de == false) {
    if(a.sync()) {
      de = true;
      digitalWrite(sPin,HIGH);
      d.flushSerial();
    }    
  } else {
    if(a.isSynced()) {
      a.nonleaderOptimalDispatchWithDyno(d,cPin);
//      float t = 1.15;
//      long initial = long(t*float(base));
//      Serial << "initial: " << _DEC(initial) << endl;
//      long final = a.nonleaderOptimalDispatch(initial,cPin);
//      Serial << "final: " << _DEC(final) << endl;
    }
  }
}

