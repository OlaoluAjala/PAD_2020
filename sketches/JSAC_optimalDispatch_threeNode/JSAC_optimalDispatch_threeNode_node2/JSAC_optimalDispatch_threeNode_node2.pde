#include <Streaming.h>
#include <Dyno.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

//Dyno d = Dyno();
XBee xbee = XBee();                  // create an XBee object
ZBRxResponse rx = ZBRxResponse();
// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x40498074,10*base,100*base,-1.5*base,0.5*base,2,base);
OGraph g = OGraph(&s);
OAgent a = OAgent(&xbee,&rx,&g,false,true);

uint8_t cPin = 48;  // drive enable led
uint8_t sPin = 7;   // synced led
uint8_t oPin = 53;     // optimal dispatch enabled led pin

boolean de = false;

void setup()  {
  pinMode(sPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  pinMode(oPin, OUTPUT);
  digitalWrite(sPin,HIGH);
  digitalWrite(cPin,HIGH);
  Serial.begin(57600);
  Serial3.begin(57600);
  //d.setSerial(Serial);
  xbee.setSerial(Serial3);
  g.addInNeighbor(0x404980AE); // node 1
  g.addInNeighbor(0x40498060); // node 3
  digitalWrite(sPin,LOW);
  digitalWrite(cPin,LOW);
}

void loop() {
  if(de == false) {
    if(a.sync()) {
      de = true;
      digitalWrite(sPin,HIGH);
    }    
  } else {
    if(a.isSynced()) {
      a.nonleaderDGC();
    }
  }
}

