#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number

uint8_t sPin = 7;   // synced led
 
  //Create objects needed for communication and control
  XBee xbee = XBee();
  ZBRxResponse rx = ZBRxResponse();
  // address, min, max, alpha, beta, out-degree, base
  OLocalVertex s = OLocalVertex(0x40498060,20*base,120*base,-1.875*base,0.125*base,1,base);
  OGraph g = OGraph(&s);
  OAgent a = OAgent(&xbee,&rx,&g,false,true);
  
void setup()  {  
  pinMode(13, OUTPUT);
  pinMode(sPin, OUTPUT);
  digitalWrite(sPin,HIGH);
  Serial.begin(9600);
  xbee.begin(Serial3);
  Serial3.begin(57600);
 
  g.addNonNeighbor(0x404980AE); // node 1
  g.addInNeighbor(0x40498074); // node 2
  
  digitalWrite(sPin,LOW);
  delay(1000);
  if(a.sync())  {
    digitalWrite(sPin,HIGH);
  Serial.println("Ard 3 has been synced");
  }  
}

void loop() {
  Serial.print("Leader time is ");
  unsigned long LeaderT = a.myMillis();
  Serial.println(LeaderT);
  delay(3000);
}
