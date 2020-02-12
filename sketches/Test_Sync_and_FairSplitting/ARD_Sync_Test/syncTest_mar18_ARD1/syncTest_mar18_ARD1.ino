#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

uint8_t errorPin = 6;  // error led pin
uint8_t sPin = 7;      // synced led

  //Create objects needed for communication and control
  XBee xbee = XBee();
  ZBRxResponse rx = ZBRxResponse();
  // address, min, max, alpha, beta, out-degree, base
  OLocalVertex s = OLocalVertex(0x404980AE,10*base,100*base,-2.1667*base,0.1667*base,1,base);    //sets up parameters for local vertex
  OGraph g = OGraph(&s);
  OAgent a = OAgent(&xbee,&rx,&g,true,true);

void setup()  {
  pinMode(13, OUTPUT);
  pinMode(sPin, OUTPUT);
  pinMode(errorPin,OUTPUT);
  pinMode(49,OUTPUT);
  digitalWrite(sPin,HIGH);
  digitalWrite(errorPin,HIGH);
  Serial.begin(9600);
  xbee.begin(Serial3);
  Serial3.begin(57600);

  g.addInNeighbor(0x40498074);   // node 2
  g.addNonNeighbor(0x40498060);  // node 3
  digitalWrite(sPin,LOW);
  digitalWrite(errorPin,LOW);
  delay(1000);
  if(a.sync())  {
    digitalWrite(49,HIGH);
    digitalWrite(sPin,HIGH);
  Serial.println("The leader (Ard 1) has been synced");
  }  
}

void loop() {
  Serial.print("Leader time is ");
  unsigned long LeaderT = millis();
  Serial.println(LeaderT);
  delay(3000);
}
