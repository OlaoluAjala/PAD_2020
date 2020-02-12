void(* resetFunc) (void) = 0; //declare reset function @ address 0
#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>

long base = 100000;  // not using floating points so need a base number
long tCmd;
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
  Serial.begin(9600);
  xbee.begin(Serial3);
  Serial3.begin(57600);

  g.addInNeighbor(0x40498074);   // node 2
  g.addNonNeighbor(0x40498060);  // node 3

//  synchronize all nodes
  if(a.sync())  {
    digitalWrite(sPin,HIGH);
    Serial.println("The leader (Ard 1) has been synced");
    Serial.print("Leader time is ");
    unsigned long LeaderT = a.myMillis();
    Serial.println(LeaderT);
//  listen for reset packet in case some neighbors are not synced
    if(a.resetSync()){
      Serial.println("Reset packet received");
      delay(100);
      resetFunc();
    }
    digitalWrite(49,HIGH);
    Serial.println("Type 'f' to run FairSplitting algorithm");
  }
}

void loop() {
  if (Serial.available()){
    if ( Serial.read()=='f' ){
      //run fair splitting algorithm
      tCmd = a.leaderFairSplitRatioConsensus(70*base,200,50);
      Serial.print("The new Torque is: ");
      Serial.println(tCmd);
      }
    }
}
