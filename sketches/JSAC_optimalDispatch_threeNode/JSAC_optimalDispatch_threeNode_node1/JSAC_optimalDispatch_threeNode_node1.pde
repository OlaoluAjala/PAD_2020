#include <Streaming.h>
#include <Dyno.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

Dyno d = Dyno();
XBee xbee = XBee();   
ZBRxResponse rx = ZBRxResponse();
// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x404980AE,10*base,100*base,-2.1667*base,0.1667*base,1,base);
OGraph g = OGraph(&s);
OAgent a = OAgent(&xbee,&rx,&g,true,true);

uint8_t errorPin = 6;  // error led pin
uint8_t sPin = 7;      // synced led
uint8_t cPin = 48;     // coordination enabled led pin
uint8_t oPin = 53;     // optimal dispatch enabled led pin

boolean de = false;
boolean ce = false;
boolean oe = false;


void setup()  {
  pinMode(cPin, OUTPUT);
  pinMode(sPin, OUTPUT);
  pinMode(oPin, OUTPUT);
  pinMode(errorPin,OUTPUT);
  pinMode(49,OUTPUT);
  pinMode(51,OUTPUT);
  digitalWrite(cPin,HIGH);
  digitalWrite(sPin,HIGH);
  digitalWrite(oPin,HIGH);
  digitalWrite(errorPin,HIGH);
  Serial.begin(57600);
  Serial3.begin(57600);
  d.setSerial(Serial);
  xbee.setSerial(Serial3);
  g.addInNeighbor(0x40498074);   // node 2
  g.addNonNeighbor(0x40498060);  // node 3
  digitalWrite(cPin,LOW);
  digitalWrite(sPin,LOW);
  digitalWrite(oPin,LOW);
  digitalWrite(errorPin,LOW);
}

void loop() {
  if(de == false) {
    if(Serial.available()) {
      uint8_t b = Serial.read();
      if (b == 'd') {
        digitalWrite(49,HIGH);
        de = true;
        if(a.sync()) {
          digitalWrite(49,LOW);
          digitalWrite(sPin,HIGH);
        }
      }
    }
  } else {
    if(ce == false && oe == false) {
      if(Serial.available()) {
        uint8_t b = Serial.read();
        if (b == 'c') {
          ce = true;
          // turn led cPin high
          digitalWrite(cPin,HIGH);
          digitalWrite(errorPin,LOW);
          delay(1000);
        } else if (b == 'o') {
          oe = true;
          // turn led oPin high
          digitalWrite(oPin,HIGH);
          digitalWrite(errorPin,LOW);
          delay(1000);
        }
      }
    }
    if(a.isSynced() && ce == true) {
      long tCmd;
      // fair splitting
      tCmd = a.leaderFairSplitRatioConsensusWithDyno(d,50,50);
      //Serial << "fair split: " << _DEC(tCmd) << endl;
      // set torque from fair splitting solution
      d.setTorqueInt(uint16_t(float(tCmd)/float(base)));
      ce = false;
      digitalWrite(cPin,LOW);
    } else if(a.isSynced() && oe == true) {
      long tCmd;
      tCmd = a.leaderOptimalDispatchWithDyno(d,50,250);
      //Serial << "optimal: " << _DEC(tCmd) << endl;
      // set torque from optimal dispatch
      d.setTorqueInt(uint16_t(float(tCmd)/float(base)));
      // reset optimal dispatch enable directive
      oe = false;
      digitalWrite(oPin,LOW);
    }
  }
}
