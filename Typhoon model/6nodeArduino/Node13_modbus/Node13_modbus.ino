#include <Streaming.h>
#include <Dyno.h>
#include <XBee.h>
#include <OGraph_SFC.h>
#include <OAgent_SFC.h>
#include <MgsModbus.h>
#include <SPI.h>
#include <Ethernet.h>


//Node 13

long base = 10000;  // not using floating points so need a base number
long D_base = 100000000;

XBee xbee = XBee();                  // create an XBee object
ZBRxResponse rx = ZBRxResponse();

// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x4157847B,0,0.0479*D_base,-1.5*base,0.5*base,4,D_base,13);
//OLocalVertex s = OLocalVertex(0x4157847B,0,0,-1.5*base,0.5*base,5,D_base,13);
OGraph_SFC g = OGraph_SFC(&s);
OAgent_SFC a = OAgent_SFC(&xbee,&rx,&g,false,true);

uint8_t errorPin = 6;  // error led pin
uint8_t sPin = 7;      // synced led
uint8_t cPin = 48;     // coordination enabled led pin

//variables for node sync check
boolean de = false;

//AFE and controller variables
int state;         // variable to store the read value 
long n;
float error = 0;
float u =0;
float u_set=0.85;
int r = 0;

//Modbus Communication
MgsModbus Mb;
int val;
// Ethernet settings (depending on MAC and Local network)
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB4 };
IPAddress ip(192, 168, 2, 4);
IPAddress gateway(192, 168, 2, 20);
IPAddress subnet(255, 255,255, 0);

uint16_t state_high;
uint16_t state_low;
uint8_t Ref_high;
uint8_t Ref_low;
uint8_t Count_high;
uint8_t Count_low;
uint8_t Pos_high;
uint8_t Pos_low;
int fc;
int ref;
int count;
int pos;
long t;
int res_flag=0;
float state1 = 0 ;

float eps = 0.0628;

float D;

void setup()  {
  Serial.begin(38400);
  Serial3.begin(38400);
  pinMode(sPin, OUTPUT);
  pinMode(cPin, OUTPUT);
  digitalWrite(sPin,HIGH);
  digitalWrite(cPin,HIGH);
  
  xbee.setSerial(Serial3);
//Define the Neighboring nodes
  //g.addInNeighbor(0x4174F1AA); // node 1
  //g.addInNeighbor(0x4174F186); // node 2
  //g.addInNeighbor(0x4151C692); // node 3
  //g.addInNeighbor(0x4151C48B); // node 4
  //g.addInNeighbor(0x4151C688); // node 5
  //g.addInNeighbor(0x4151C6AB); // node 6
  //g.addInNeighbor(0x4151C6CB); // node 7
  //g.addInNeighbor(0x4151C6AC); // node 8
  g.addInNeighbor(0x415786E1); // node 9
  g.addInNeighbor(0x415786D3); // node 10
  g.addInNeighbor(0x415DB670); // node 11
  //g.addInNeighbor(0x415786A9); // node 12
  //g.addInNeighbor(0x4157847B); // node 13
  //g.addInNeighbor(0x415DB664); // node 14
  //g.addInNeighbor(0x415DB673); // node 15
  g.addInNeighbor(0x415DB684); // node 19
  //g.addInNeighbor(0x41516F0B); // node 20
  
  digitalWrite(sPin,LOW);
  digitalWrite(cPin,LOW);

  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);   // start etehrnet interface
  for (int i=0;i<12;i++) {
     Mb.MbData[i] = 0;
  }
}


void loop() {
  if(de == false)  
  {
    if(!(a.isLeader()))
    {
      Serial.println("Still trying to sync");
      if(a.sync()) 
      {
        Serial.println("Communication Link established");
        Serial.println("c");
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
    if(a.isSynced()) {
      receiveTyphoonData();
      state =  Mb.MbData[0];
      if(state == 0)
      {
      
        D=0;
      }
      else
      {
      
        D=0.0479;
      }
      //Serial.println("Data");
      //Serial.println(float(state),4);
      //a.nonleaderFairSplitRatioConsensus(-1*base*state);
      //a.nonleaderFairSplitRatioConsensus(1*D_base,0);
      //a.fairSplitRatioConsensus_RSL(D_base*1,0*D_base, 16,160);
      a.fairSplitRatioConsensus_RSL(-base*state,D*D_base,16,160);
      state1 = a.getbufferdata(0);

      //Serial.println("ratio consensus result");
      //Serial.println(state1,4);  
       /* Controller code
       r=r+1;
       if(r>2)
       { 
       error=error + -1*0.707*state1;
       u=u_set+0.707*error;
       Serial.println(u,4);
       }
       Mb.MbData[1]=10*base*u;
       sendConsensusResults();
       
      // Controller code over
      */
      a.resync();
      }
  }
}
















void sendConsensusResults()
{
  //VARIABLES NOT BEING USED////////////////////////////////////
  fc = 16;                                 //function code to write to multiple registers
  ref = 0;                                 //starting address of register at server end (Typhoon) which we are writing to in this case
  Ref_high = uint8_t(ref >> 8 && 0x00FF);
  Ref_low = uint8_t(ref & 0x0FF);
  count = 2;                              //in this case we want to write to 2 registers on the server(typhoon)
  Count_high = uint8_t(count >> 8 && 0x00FF);
  Count_low = uint8_t(count & 0x0FF);
  pos = 0;                              //position of the registers on the client(arduino) we want to read
  Pos_high = uint8_t(pos >> 8 && 0x00FF);
  Pos_low = uint8_t(pos & 0x0FF); 
  //Mb.Build(fc,Ref_high,Ref_low,Count_high,Count_low,Pos_high,Pos_low);
  //Serial.println("Sent Request Packet");
  ////////////////////////////////////////////////////////////////
  int node4_ip = 64; //part of ip address for node 1 on the HIL side 
  Mb.Req(MB_FC_WRITE_REGISTER,0,1,1,node4_ip); //(MB_FC FC, word Ref - typhoon, word Count, word Pos - arduino, int nodeip)
  Mb.MbmRun();
  //Serial.println("Received Response");
}

void receiveTyphoonData()
{
  int node4_ip = 64; //part of ip address for node 1 on the HIL side 
  Mb.Req(MB_FC_READ_INPUT_REGISTER,0,1,0,node4_ip); //(MB_FC FC, word Ref, word Count, word Pos, int nodeip)
  Mb.MbmRun();
}
