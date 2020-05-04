#include <Streaming.h>
#include <XBee.h>
#include <OGraph.h>
#include <OAgent.h>
#include <MgsModbus.h>
#include <SPI.h>
#include <Ethernet.h>


//Nodes 9,19,11

long base = 10000;  // use base to increase precision of results

XBee xbee = XBee();
ZBRxResponse rx = ZBRxResponse();

// address, min, max, alpha, beta, out-degree, base
OLocalVertex s = OLocalVertex(0x415786E1,9,0,0,0,0,2,base); //address and ID
LinkedList l = LinkedList();  //#NODE
OGraph g = OGraph(&s,&l);
//OAgent_LinkedList al = OAgent_LinkedList();  //#NODE
OAgent a = OAgent(&xbee,&rx,&g,true,true); // argument rx?

uint8_t sPin = 7;      // synced led
uint8_t cPin = 48;     // coordination enabled led pin

//variables for node sync check
boolean de = false;

//node 9
//AFE and controller variables
float f_error0_1;         // variable to store the read value 
float v_error0_1;         // variable to store the read value
float flow_flag0_1;         // variable to store the read value 
float q_level0_1;         // variable to store the read value 
float f_error1_1;         // ratio consensus result for average frequency error

float deltaQ_1;           //delta q necessary for the voltage violation

float error_1 = 0;
float u_f_1 =0;
float u_v_1 =0;
float u_set_1=0.85;

//Node 10
//AFE and controller variables
float f_error0_2;         // variable to store the read value 
float v_error0_2;         // variable to store the read value
float flow_flag0_2;         // variable to store the read value 
float q_level0_2;         // variable to store the read value 
float f_error1_2;         // ratio consensus result for average frequency error

float deltaQ_2;           //delta q necessary for the voltage violation

float error_2 = 0;
float u_f_2 =0;
float u_v_2 =0;
float u_set_2=0.85;

//Node 11
//AFE and controller variables
float f_error0_3;         // variable to store the read value 
float v_error0_3;         // variable to store the read value
float flow_flag0_3;         // variable to store the read value 
float q_level0_3;         // variable to store the read value 
float f_error1_3;         // ratio consensus result for average frequency error

float deltaQ_3;           //delta q necessary for the voltage violation

float error_3 = 0;
float u_f_3 =0;
float u_v_3 =0;
float u_set_3=0.85;

//Modbus Communication
MgsModbus Mb; 
int val;
// Ethernet settings (depending on MAC and Local network)
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB9 };
IPAddress ip(192, 168, 2, 9); // What are these addresses
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

float eps_f = 0.001;
float eps_v = 0.001;
float D = 1;

void setup()  {
  Serial.begin(38400);
  Serial3.begin(38400);
  pinMode(cPin, OUTPUT);
  pinMode(sPin, OUTPUT);
  digitalWrite(cPin,HIGH);
  digitalWrite(sPin,HIGH);
  
  xbee.setSerial(Serial3); //Specify the serial port for xbee
//Define the Neighboring nodes
  //g.addInNeighbor(0x4174F1AA,1,0,0); // node 1
  //g.addInNeighbor(0x4174F186,2,0,0); // node 2
  //g.addInNeighbor(0x4151C692,3,0,0); // node 3
  //g.addInNeighbor(0x4151C48B,4,0,0); // node 4
  //g.addInNeighbor(0x4151C688,5,0,0); // node 5
  //g.addInNeighbor(0x4151C6AB,6,0,0); // node 6
  //g.addInNeighbor(0x4151C6CB,7,0,0); // node 7
  //g.addInNeighbor(0x4151C6AC,8,0,0); // node 8
  
  //g.addInNeighbor(0x415786E1,9,0,0); // node 9
  g.addInNeighbor(0x415786D3,10,0,0); // node 10
  g.addInNeighbor(0x415DB670,11,0,0); // node 11
  
  //g.addInNeighbor(0x415786A9,12,0,0); // node 12
  //g.addInNeighbor(0x4157847B,13,0,0); // node 13
  //g.addInNeighbor(0x415DB664,14,0,0); // node 14
  
  //g.addInNeighbor(0x415DB673,15,0,0); // node 15
  //g.addInNeighbor(0x415DB684,19,0,0); // node 19
  //g.addInNeighbor(0x41516F0B,20,0,0); // node 20

  g.configureLinkedList();
  
  digitalWrite(cPin,LOW);
  digitalWrite(sPin,LOW);
  
 // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);   // start etehrnet interface
  for (int i=0;i<12;i++) {
     Mb.MbData[i] = 0;
  }
}

void loop(){

Serial.println("Send letter s to start");
while (Serial.available() == 0) 
    { 
      //simply makes the arduino wait until commputer sends signal        
    }
if(Serial.available()) 
  {
    Serial.println("got some letter");
    uint8_t b = Serial.read(); //enter the character 's'
    Serial.println(b);
    
 if ((b == 's'))//start program
    {
      receiveTyphoonData();
      //node 9
      int f_error_1 =  Mb.MbData[0]*((-2*Mb.MbData[1])+1);
      int v_error_1  = Mb.MbData[2]*((-2*Mb.MbData[3])+1);
      int flow_flag_1  = Mb.MbData[4]*((-2*Mb.MbData[5])+1);
      int q_level_1  = Mb.MbData[6]*((-2*Mb.MbData[7])+1);
      
      f_error0_1 =  float(f_error_1);
      v_error0_1 =  float(v_error_1);
      flow_flag0_1 =  float(flow_flag_1);
      q_level0_1 =  float(q_level_1);
      
      f_error0_1 =  f_error0_1/base;
      v_error0_1 =  v_error0_1/base;
      flow_flag0_1 =  flow_flag0_1/base;
      q_level0_1 =  q_level0_1/base;

      Serial.print("f error in 9: ");
      Serial.println(float(f_error0_1),4);
      Serial.print("v error in 9: ");
      Serial.println(float(v_error0_1),4);
      Serial.print("flow flag in 9: ");
      Serial.println(float(flow_flag0_1),4);
      Serial.print("Q level in 9: ");
      Serial.println(float(q_level0_1),4);
      delay(100);
      
      //Node 10
      int f_error_2 =  Mb.MbData[8]*((-2*Mb.MbData[9])+1);
      int v_error_2  = Mb.MbData[10]*((-2*Mb.MbData[11])+1);
      int flow_flag_2  = Mb.MbData[12]*((-2*Mb.MbData[13])+1);
      int q_level_2  = Mb.MbData[14]*((-2*Mb.MbData[14])+1);
      
      f_error0_2 =  float(f_error_2);
      v_error0_2 =  float(v_error_2);
      flow_flag0_2 =  float(flow_flag_2);
      q_level0_2 =  float(q_level_2);
      
      f_error0_2 =  f_error0_2/base;
      v_error0_2 =  v_error0_2/base;
      flow_flag0_2 =  flow_flag0_2/base;
      q_level0_2 =  q_level0_2/base;

      Serial.print("f error in 10: ");
      Serial.println(float(f_error0_2),4);
      Serial.print("v error in 10: ");
      Serial.println(float(v_error0_2),4);
      Serial.print("flow flag in 10: ");
      Serial.println(float(flow_flag0_2),4);
      Serial.print("Q level in 10: ");
      Serial.println(float(q_level0_2),4);
      delay(100);

      //Node 11
      int f_error_3 =  Mb.MbData[16]*((-2*Mb.MbData[17])+1);
      int v_error_3  = Mb.MbData[18]*((-2*Mb.MbData[19])+1);
      int flow_flag_3  = Mb.MbData[20]*((-2*Mb.MbData[21])+1);
      int q_level_3  = Mb.MbData[22]*((-2*Mb.MbData[23])+1);
      
      f_error0_3 =  float(f_error_3);
      v_error0_3 =  float(v_error_3);
      flow_flag0_3 =  float(flow_flag_3);
      q_level0_3 =  float(q_level_3);
      
      f_error0_3 =  f_error0_3/base;
      v_error0_3 =  v_error0_3/base;
      flow_flag0_3 =  flow_flag0_3/base;
      q_level0_3 =  q_level0_3/base;


      Serial.print("f error in 11: ");
      Serial.println(float(f_error0_3),4);
      Serial.print("v error in 11: ");
      Serial.println(float(v_error0_3),4);
      Serial.print("flow flag in 11: ");
      Serial.println(float(flow_flag0_3),4);
      Serial.print("Q level in 11: ");
      Serial.println(float(q_level0_3),4);
      delay(100);

      a.voltageControl_cent(v_error0_1,v_error0_2,v_error0_3,1,5,q_level0_1,
      q_level0_2,q_level0_3,0.707,-0.707,-0.225736,-0.224693,-0.258852,1/3);
      
      //void voltageControl_cent( float diffV1,float diffV2,float diffV3, float Vref, float secPercentage, float q1,
      //float q2,float q3, float qtop,float qbottom, float S1,float S2,float S3, float alphaVC){
     
      u_v_1=s.getQ1();
      u_f_1=0.1;
      u_v_2=getQ2();
      u_f_2=0.2;
      u_v_3=getQ3();
      u_f_3=0.3;

      //SENDING RESULTS
      if (u_f_1<0)
      {
         Mb.MbData[0]=1;
      }else{
         Mb.MbData[0]=0;
      }
      
      if (u_v_1<0)
      {
         Mb.MbData[2]=1;
      }else{
         Mb.MbData[2]=0;
      }
      
      if (u_f_2<0)
      {
         Mb.MbData[4]=1;
      }else{
         Mb.MbData[4]=0;
      }
      
      if (u_v_2<0)
      {
         Mb.MbData[6]=1;
      }else{
         Mb.MbData[6]=0;
      }
      
      if (u_f_3<0)
      {
         Mb.MbData[8]=1;
      }else{
         Mb.MbData[8]=0;
      }
      
       if (u_v_3<0)
      {
         Mb.MbData[10]=1;
      }else{
         Mb.MbData[10]=0;
      }

      Mb.MbData[1]=base*abs(u_f_1);            
      Mb.MbData[3]=base*abs(u_v_1);
      Mb.MbData[5]=base*abs(u_f_2);
      Mb.MbData[7]=base*abs(u_v_2);
      Mb.MbData[9]=base*abs(u_f_3);
      Mb.MbData[11]=base*abs(u_v_3);

      sendConsensusResults();     
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
  int node9_ip = 69; //part of ip address for node 9 on the HIL side 
  Mb.Req(MB_FC_WRITE_MULTIPLE_REGISTERS,0,12,0,node9_ip); //(MB_FC FC, word Ref - typhoon, word Count, word Pos - arduino, int nodeip)
  Mb.MbmRun();
  //SerialUSB.println("Sent Stuff to typhoon");
}

void receiveTyphoonData()
{
  int node9_ip = 69; //part of ip address for node 9 on the HIL side
  Mb.Req(MB_FC_READ_INPUT_REGISTER,0,24,0,node9_ip); //(MB_FC FC, word Ref - typhoon, word Count, word Pos -arduino, int nodeip)
  Mb.MbmRun();
}
