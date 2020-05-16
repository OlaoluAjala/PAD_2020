#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>


long base = 1e4;  // not using floating points so need a base number
float tCmd;
uint8_t i=1;//number of inneighbors

//Create objects needed for communication and control
XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();
// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x415786E1,9,0,0,0,0,i,base);    //sets up parameters for local vertex
// OLocalVertex s = OLocalVertex(0x415786E1,0,0.225*D_base,-2.1667*base,0.1667*base,5,D_base,9);
LinkedList l = LinkedList();
OGraph g = OGraph(&s,&l);
OAgent a = OAgent(&xbee,&rx,&g,true,true);//(xbee,rx,leader,...)

uint8_t errorPin = 6;  // error led pin
uint8_t sPin = 7;      // synced led
uint8_t cPin = 48;     // coordination enabled led pin

//variables for node sync check
boolean de = false;

void setup()  
{
  Serial.println("start sync");
  Serial.begin(38400);
  Serial3.begin(38400);
  pinMode(13, OUTPUT);
  pinMode(sPin, OUTPUT);
  pinMode(errorPin,OUTPUT);
  pinMode(49,OUTPUT);
  xbee.setSerial(Serial3);

  g.addInNeighbor(0x415786D3,10,0,0);   // node 10
//  g.addNonNeighbor(0x415DB670);  // node 11
g.configureLinkedList();
}
void loop()
{
  if(de == false)
  {
    if(!(a.isLeader()))
    {
      Serial.println("Still trying to sync");
      if(a.sync()) 
      {
        Serial.println("Communication Link established");
        Serial.println("c");
        delay(10);
        digitalWrite(sPin,HIGH);
        de = true;
      }
      else
      {
        de  = false; //means could not sync 
      }
    }
    if (a.isLeader())
    {
      Serial.println("Send letter s(r) to sync(resync)"); //let computer know you want to sync
      while (Serial.available() == 0) 
      { 
        //simply makes the arduino wait until commputer sends signal        
      }
      if(Serial.available()) 
      {
        Serial.println("got some letter");
        uint8_t b = Serial.read(); //enter the character 's'
        Serial.println(b);
        if (b == 'r')
          {
            a.setLeader(0);
          }
        if ((b == 's')||(b == 'r'))
        {
          Serial.println("got the s and about to sync");
          de = true;
          if(a.sync()) {
            Serial.println("Communication Link established");
            Serial.println("c"); 
            digitalWrite(sPin,HIGH);
            //ce = true;
          }
          else
          {
            de  = false; //means could not sync 
          } 
        }
      }
    }
  }

  
  else 
  {
    if(a.isSynced())
    {
     //run fair splitting algorithm
      tCmd = a.fairSplitRatioConsensus_RSL(0.00678,1,50,200,3,0.001);
      //(y,z,iterations,period,diameter,epsilon)
      Serial.print("The value of the RC algorithm is: ");
      Serial.println(tCmd);

      int bbbb = Serial.read();

      while(Serial.available()==0)
      {
        
      }      
       a.resync();
    }
  }
}
