#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>


long base = 100;  // not using floating points so need a base number
long tCmd;
uint8_t i=1;
uint8_t errorPin = 6;  // error led pin

uint8_t sPin = 7;      // synced led

  //Create objects needed for communication and control
  XBee xbee = XBee();
  ZBRxResponse rx = ZBRxResponse();
  // address, min, max, alpha, beta, out-degree, base
  OLocalVertex s = OLocalVertex(0x4174F1AA,0*base,1*base,0,0,i,base);    //sets up parameters for local vertex
 // OLocalVertex s = OLocalVertex(0x415786E1,0,0.225*D_base,-2.1667*base,0.1667*base,5,D_base,9);
  OGraph g = OGraph(&s);
  OAgent a = OAgent(&xbee,&rx,&g,true,true);

void setup()  {
  Serial.begin(38400);
  Serial3.begin(38400);
  
  pinMode(13, OUTPUT);
  pinMode(sPin, OUTPUT);
  pinMode(errorPin,OUTPUT);
  pinMode(49,OUTPUT);
  xbee.setSerial(Serial3);

  g.addInNeighbor(0x415786D3);   // node 2
  g.addNonNeighbor(0x4157847B);  // node 5

Serial.println("syncronize has begun");
//  synchronize all nodes

  if(a.sync())  {
    digitalWrite(sPin,HIGH);
    Serial.println("The leader (Ard 1) has been synced");
    Serial.print("Leader time is ");
    unsigned long LeaderT = a.myMillis();
    Serial.println(LeaderT);
//  listen for reset packet in case some neighbors are not synced
//    if(a.resetSync()){
//      Serial.println("Reset packet received");
//      delay(100);
//      resetFunc();
//    }
    digitalWrite(49,HIGH);
    Serial.println("Type 'f' to run FairSplitting algorithm");
  }
}

void loop() {
  if (Serial.available()){
    if ( Serial.read()=='f' ){
      //run fair splitting algorithm
      tCmd = a.leaderFairSplitRatioConsensus(1*base,10,50);
      Serial.print("The new Torque is: ");
      Serial.println(tCmd);
      }
    }
}
