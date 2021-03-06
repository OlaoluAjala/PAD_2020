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
OLocalVertex s = OLocalVertex(0x405D9822,0.1*base,1.75*base,-1.875*base,0.125*base,2,base);
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
  //d.setTimeout(10000);
  xbee.setSerial(Serial3);
  g.addInNeighbor(0x405D9805);   // node 5
  g.addInNeighbor(0x405D980A);   // node 8
  g.addNonNeighbor(0x40498065);  // node 4
  // manually save lambdas
  g.getVertexByUniqueID(1)->setLambdaMin(3.2*base);
  g.getVertexByUniqueID(1)->setLambdaMax(7.5*base);
  g.getVertexByUniqueID(2)->setLambdaMin(17.4*base);
  g.getVertexByUniqueID(2)->setLambdaMax(25*base);
  g.getVertexByUniqueID(3)->setLambdaMin(13.6*base);
  g.getVertexByUniqueID(3)->setLambdaMax(28*base);
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
      //long t = a.nonleaderOptimalDispatch(0.98*base,cPin);
      //Serial << _MEM(PSTR("tfinal: ")) << _DEC(t) << endl;
    }
  }
}
