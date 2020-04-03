//void(* resetFunc) (void) = 0; //declare reset function @ address 0
#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100;  // not using floating points so need a base number
long tCmd;
uint8_t i=2;
uint8_t errorPin = 6;
uint8_t sPin = 7;   // synced led

  //Create objects needed for communication and control
  XBee xbee = XBee();
  ZBRxResponse rx = ZBRxResponse();
  // address, min, max, alpha, beta, out-degree, base
  OLocalVertex s = OLocalVertex(0x4174F186,0*base,1*base,0,0,i,base);
  OGraph g = OGraph(&s);
  OAgent a = OAgent(&xbee,&rx,&g,false,true);

void setup()  {
  Serial.begin(38400);
  Serial3.begin(38400);
  pinMode(13, OUTPUT);
  pinMode(sPin, OUTPUT);
  pinMode(errorPin,OUTPUT);
  pinMode(49,OUTPUT);
  xbee.setSerial(Serial3);

  g.addInNeighbor(0x415786D3); // node 1
  g.addInNeighbor(0x4157847B); // node 5
  
//  synchronize all nodes
  if(a.sync())  {
    digitalWrite(sPin,HIGH);
    Serial.println("Ard 2 has been synced");
    Serial.print("Leader time is ");
    unsigned long LeaderT = a.myMillis();
    Serial.println(LeaderT);
//    if(a.resetSync()){
//      Serial.println("Reset packet received");
//      delay(100);
//      resetFunc();
//    }
    digitalWrite(49,HIGH);
  }
}

void loop() {
  tCmd = a.nonleaderFairSplitRatioConsensus(2*base);
  Serial.print("The new Torque is: ");
  Serial.println(tCmd);
}
