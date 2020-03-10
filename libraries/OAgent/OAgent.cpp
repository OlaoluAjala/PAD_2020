/*
 *  OAgent.cpp
 *  
 *
 *  Created by Stanton T. Cady on 06/15/12
 *  Edited by Laura Haller on 01/11/13
 *  Last edited by Stanton T. Cady on 01/16/13
 *
 */



#include "OAgent.h"
#include "Streaming.h"
#include <cstdlib> 
//#define VERBOSE

//// Public methods
/// Constructors
OAgent::OAgent() {

    XBee temp1 = XBee();
    ZBRxResponse temp2 = ZBRxResponse();
    OGraph temp3 = OGraph();
    _prepareOAgent(&temp1,&temp2,&temp3);
    //setRS(0);
}

OAgent::OAgent(XBee * xbee, OGraph * G, bool leader, bool quiet) {
     ZBRxResponse temp = ZBRxResponse();
    _prepareOAgent(xbee,&temp,G,leader,quiet);
    //setRS(0);
}

OAgent::OAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader, bool quiet) {
    _prepareOAgent(xbee,rx,G,leader,quiet);
    //setRS(0);
}

// OAgent::OAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader, bool quiet, int RS) {
//     _prepareOAgent(xbee,rx,G,leader,quiet);  
// }

/// End Constructors

/// Methods to get private elements
//OGraph * OAgent::getGraph() {
//    return _G;
//}
//
//void OAgent::setLeader(bool leader) {
//    _leader = leader;
//}
//
//bool OAgent::isLeader() {
//    return _leader;
//}
//
//void OAgent::setQuiet(bool quiet) {
//    _quiet = quiet;
//}
//
//bool OAgent::isQuiet() {
//    return _quiet;
//}
/// End methods to get private elements

/// Ratio-consensus
// Fair splitting
float OAgent::fairSplitRatioConsensus(long y, long z, uint8_t iterations, uint16_t period) {  //,uint8_t round
    OLocalVertex * s = _G->getLocalVertex();      // store pointer to local vertex 
    float Dout = float(s->getOutDegree() + 1);    // store out degree, the +1 is to account for the self loops
    _initializeFairSplitting(s,y,z);      // initialize state variables                           
    unsigned long start;                // create variable to store iteration start time
    bool txDone;                        // create variable to keep track of broadcasts
    bool mucheck = 0;
    bool sigmacheck = 0;
    //srand(analogRead(0)); //put this instruction in both the leader and nonleader consensus functions
    uint16_t txTime;       //_genTxTime(period,10,analogRead(0));   // get transmit time; 
    long inY;                           // incoming state variable
    long inZ;
    int count = 3;
    //uint8_t no_of_nodes = _G->getN() - 1;  //number of in-neighbors (in this case)
    int node_check[NUM_REMOTE_VERTICES]; //checker for each neighbor whether data is received or not per iteration
    //int step_counter = 0;        //used when adjusting vertex array to account for offline neigbors
    uint32_t aLsb;

    for(int i=0; i < NUM_REMOTE_VERTICES; i++)
    {
        node_check[i] = 0;
        //Serial <<"\n"<<"For Node "<<s->getID()<<", from Node"<<i<<", Y[0]= "<< getneighborY0(i) << "\n ";
        //Serial <<"\n"<<"For Node "<<s->getID()<<", from Node"<<i<<", Z[0]= "<< getneighborZ0(i) << "\n ";
    }
 
    int frame = 35;
    


   

    //if(txTime <= 0 || txTime > period)
     //   txTime = 25;                      //25 milliseconds

    for(uint8_t k = 0; k < iterations; k++) {
        srand(analogRead(0));
        txTime =  (rand() % (period - 2*frame)) + frame;  //determines the time window in which a payload is transmitted
        txDone = false;     // initialize toggle to keep track of broadcasts
        start = millis();   // initialize timer
        // clear in y and in z
        inY = 0;
        inZ = 0;
            
        //Serial << "iteration: ";
        //Serial << k;
        //Serial << "\n";
        uint8_t i;
        while(uint16_t(millis()-start) < period) {
            if(_fairSplitPacketAvailable()) {                                   // robust, coordinate value packet available
                aLsb = _rx->getRemoteAddress64().getLsb();
                if(_G->isInNeighbor(aLsb,i)) {    // check if remote device is in in-neighborhood

                    float inMu = _getMuFromPacket();                             // store incoming value of mu
                    float inSigma = _getSigmaFromPacket();                       // store incoming value of sigma
                    uint8_t neighborID = _getNeighborIDFromPacket();
                    //int inheritor_id = _getinheritorIDFromPacket();

                    uint8_t nodeID = s->getID();


                    if(s->getStatus(neighborID) == 1)
                    {
                        s->setStatus(neighborID, 2);
                        uint8_t dout = s->getOutDegree();
                        s->setOutDegree(dout + 1);
                    }
                    else if(s->getStatus(neighborID) == 0)
                        s->setStatus(neighborID, 2);


                    //Serial << "Address of neighbor: "; 
                    //Serial  << _HEX(_rx->getRemoteAddress64().getLsb());
                    //Serial << "\n";   
                    
                    long Mudiff = inMu - s->getNuMin(i); 
                    long sigdiff =  inSigma - s->getTau(i);
                        Serial<<"incoming µ from node "<<i<<" is: "<<Mudiff<<endl;
                        Serial<<"incoming σ from node "<<i<<" is: "<<sigdiff<<endl;
                        // if(k==0)
                        //     {
                        //      setneighborY0( (neighborID-1), Mudiff+( Mudiff*(inheritor_id==nodeID) ) );
                        //         //Serial <<"\n"<<"Y0 element "<<i<<" is: "<< getneighborY0(i) << "\n ";
                        //      setneighborZ0( (neighborID-1), sigdiff+( sigdiff*(inheritor_id==nodeID) ) );
                        //         //Serial <<"\n"<<"Z0 element "<<i<<" is: "<< getneighborZ0(i) << "\n " ;
                        //     }
                    inY += Mudiff;                               // add mu from incoming device and subtract last received value
                    s->setNuMin(i,inMu);                                        // save received mu as new nu (nuMin)
                    inZ += sigdiff;                              // add sigma from incoming device and subtract last received value
                    s->setTau(i,inSigma);                                       // save received sigma as new tau
                    
                    node_check[neighborID -1] = 1;                      //data was received from a neighbor at this iteration
                   //Serial<<"inY: "<<inY<<endl; 
                }
            }

            if((int((millis() - start)) >= txTime) && !txDone) {
                txDone = true; // toggle txDone
                //Serial<<"Sending "<<s->getMuMin()<<" to neighbors"<<endl;
                _broadcastFairSplitPacket(s);
                
            }
        }

        if(!_quiet) {
           
            delay(10);
        } else {
            delay(25);
        }

        s->setYMin(long(float(s->getYMin())/Dout) + inY);             //problem is here (if inY is zero then this is set to zero)
        s->setMuMin(s->getMuMin() + long(float(s->getYMin())/Dout));
        s->setZ(long(float(s->getZ())/Dout) + inZ);
        s->addToSigma(long(float(s->getZ())/Dout));

        _buffer[count] = float(s->getYMin())/float(s->getZ()); //add kth iterate to buffer
        _bufferY[count] = float(s->getYMin()); //add kth iterate to buffer
        _bufferZ[count] = float(s->getZ()); //add kth iterate to buffer
        //Serial << "Every Iteration Y";
        //Serial << _buffer[count];
        //Serial << "\n";
        count++; 

        //CODE TO IMPROVE RESILIENCY

                
        for(int j=0;j < NUM_REMOTE_VERTICES; j++)
        {
            if(node_check[j] == 0 && node_counter[j] >= 0)
                node_counter[j] += 1;
            else if(node_check[j] == 1 ) 
                node_counter[j] = 0;
            

            
            //Serial << "node_counter ";
            //Serial << j;
            //Serial << " : ";
            //Serial << node_counter[j];
            //Serial << "\n";
            

            if(node_counter[j] >= int(iterations/2) )
            {
                s->setStatus(j, 1);
                s->decrementInDegree();
                uint8_t dout = s->getOutDegree();              //since we assume it is a bidirectional graph, InDegree is equivalent to OutDegree
                s->setOutDegree(dout - 1); 
                node_counter[j] = -1;                          //set counter to -1 when limit reached to indicate offline link status    
            }
            node_check[j] = 0;                                 //reset node_check after each iteration
        }

        /*  old approach
        step_counter = 0;

        for(int j=0;j < no_of_nodes; j++)
        {
            if(online[j])
            {
                if(step_counter > 0)
                {
                     node_counter[j - step_counter] = node_counter[j];
                     _G->AdjustVertexArray(j,step_counter);
                } 
            }
            else
            {
                _G->removeInNeighbor(j);
                step_counter++;
            }
        }
        no_of_nodes = no_of_nodes - step_counter;
        
        */
    }

    if(s->getZ() != 0)
        _buffer[0] = float(s->getYMin())/float(s->getZ()); 

    _buffer[1] = s->getOutDegree();
    _buffer[2] = _G->getN() - 1;

    
    //Serial << "RC from Library";
    //Serial<<"Y and Z: " << (s->getYMin()) << " , " <<(s->getZ())<<endl;
    //Serial <<"\n";  
   
    return float(s->getYMin())/float(s->getZ());
}


// Resilient Fair splitting RC (added in by Olaolu)
float OAgent::ratiomaxminConsensus(float y, float z, uint8_t iterations, uint16_t period) //,uint8_t round
{  
    OLocalVertex * s = _G->getLocalVertex();        // store pointer to local vertex 
    float Dout = float(s->getOutDegree() + 1);      // store out degree, the +1 is to account for the self loops
    _initializeFairSplitting_RSL(s,y,z);            // initialize state variables                           
    unsigned long start;                            // create variable to store iteration start time
    bool txDone;                                    // create variable to keep track of broadcasts
    bool mucheck = 0;
    bool sigmacheck = 0;
    //srand(analogRead(0)); //put this instruction in both the leader and nonleader consensus functions
    uint16_t txTime;        //_genTxTime(period,10,analogRead(0));   // get transmit time; 
    float inY;              // incoming state variable
    float inZ;
    float eps=0.00001;       //variable for setting the end point of the iterations
    float endY;
    float endZ;
    int count = 3;    
    int iter;               //variable for the iteration count

    //uint8_t no_of_nodes = _G->getN() - 1;  //number of in-neighbors (in this case)
    int node_check[NUM_REMOTE_VERTICES]; //checker for each neighbor whether data is received or not per iteration
    //int step_counter = 0;        //used when adjusting vertex array to account for offline neigbors
    uint32_t aLsb;

    for(int i=0; i < NUM_REMOTE_VERTICES; i++)
    {
        node_check[i] = 0;
        //Serial <<"\n"<<"For Node "<<s->getID()<<", from Node"<<i<<", Y[0]= "<< getneighborY0(i) << "\n ";
        //Serial <<"\n"<<"For Node "<<s->getID()<<", from Node"<<i<<", Z[0]= "<< getneighborZ0(i) << "\n ";
    }
    int frame = 30;
   

    //if(txTime <= 0 || txTime > period)
     //   txTime = 25;                      //25 milliseconds
    iter=0;
    do
    {
        srand(analogRead(0));
        txTime =  (rand() % (period - 2*frame)) + frame;  //determines the time window in which a payload is transmitted
        txDone = false;     // initialize toggle to keep track of broadcasts
        start = millis();   // initialize timer
        // clear in y and in z
        inY = 0;
        inZ = 0;
            
        Serial<<"Iteration "<<iter+1<<". Ratio is: "<<_FLOAT(s->getYMin()/s->getZ(),4)<<"; Y is: "<<_FLOAT(s->getYMin(),4)<<"; Z is: "<<_FLOAT(s->getZ(),4)<<endl;
        delay(5);
        endY=s->getYMin();
        endZ=s->getZ();

        uint8_t i;
        while(uint16_t(millis()-start) < period)
        {
            if(_fairSplitPacketAvailable())
            {                                   // robust, coordinate value packet available
                aLsb = _rx->getRemoteAddress64().getLsb();
                if(_G->isInNeighbor(aLsb,i))
                {    // check if remote device is in in-neighborhood
                    float inMu = _getMuFromPacket();                             // store incoming value of mu
                    float inSigma = _getSigmaFromPacket();                       // store incoming value of sigma
                    uint8_t neighborID = _getNeighborIDFromPacket();

                    uint8_t nodeID = s->getID();

                    //find out id this node is the inheritor for node i
                    // if(s->getStatus(neighborID) == 1)
                    // {
                    //     s->setStatus(neighborID, 2);
                    //     uint8_t dout = s->getOutDegree();
                    //     s->setOutDegree(dout + 1);
                    // }
                    // else if(s->getStatus(neighborID) == 0)
                    //     s->setStatus(neighborID, 2);


                    //Serial << "Address of neighbor: "; 
                    //Serial  << _HEX(_rx->getRemoteAddress64().getLsb());
                    //Serial << "\n";   
                    
                    float Mudiff = inMu - s->getNuMin(i); 
                    float sigdiff =  inSigma - s->getTau(i);
                    //Serial<<"received: "<<inMu<<" and "<<inSigma<<" from node "<<i+1<<endl;
                        // if(k==0)
                        // {
                        //     setneighborY0( (neighborID-1), Mudiff+( Mudiff*(inheritor_id==nodeID) ) );
                        //     //Serial <<"\n"<<"Y0 element "<<i<<" is: "<< getneighborY0(i) << "\n ";
                        //     setneighborZ0( (neighborID-1), sigdiff+( sigdiff*(inheritor_id==nodeID) ) );
                        //     //Serial <<"\n"<<"Z0 element "<<i<<" is: "<< getneighborZ0(i) << "\n " ;
                        // }
                    inY += Mudiff;                               // add mu from incoming device and subtract last received value
                    s->setNuMin(i,inMu);                                        // save received mu as new nu (nuMin)
                    inZ += sigdiff;                              // add sigma from incoming device and subtract last received value
                    s->setTau(i,inSigma);                                       // save received sigma as new tau
                    

                    //Using min and max consensus, choose a leader and deputy for begining fair split ratio consensus
                    //min consensus
                    // if (leader_id < s->getleaderID())
                    // {
                    //     s->setleaderID(leader_id);
                    // }
                    // //max consensus
                    // if (deputy_id > s->getdeputyID())
                    // {
                    //     s->setdeputyID(deputy_id);
                    // }


                    node_check[neighborID -1] = 1;                      //data was received from a neighbor at this iteration
                    
                }
            }
            if((int((millis() - start)) >= txTime) && !txDone) {
                txDone = true; // toggle txDone
                _broadcastFairSplitPacket_RSL(s);
                    //Serial<<"Sent: "<<s->getMuMin()<<" and "<<s->getSigma()<<" to neighbors"<<endl;
            }
        }
        // Serial<<"Ratio is: "<<_FLOAT(float(s->getYMin())/float(s->getZ()),4)<<"; Y is: "<<_FLOAT(float(s->getYMin()),4)<<"; Z is: "<<_FLOAT(float(s->getZ()),4)<<endl;

        if(!_quiet) {
           
            delay(10);
        } else {
            delay(25);
        }
        // Serial<<"Total inY from neighbors is: "<<inY<<endl;
        // Serial<<"Total inZ from neighbors is: "<<inZ<<endl;
        s->setYMin((s->getYMin()/Dout) + inY);             //problem is here (if inY is zero then this is set to zero)
        s->setMuMin(s->getMuMin() + (s->getYMin()/Dout));
        s->setZ((s->getZ()/Dout) + inZ);
        s->addToSigma(s->getZ()/Dout);

        _buffer[count] = (s->getYMin())/(s->getZ()); //add kth iterate to buffer
        _bufferY[count] = s->getYMin(); //add kth iterate to buffer
        _bufferZ[count] = s->getZ(); //add kth iterate to buffer
        //Serial << "Every Iteration Y";
        //Serial << _buffer[count];
        //Serial << "\n";
        count++; 

        //CODE TO IMPROVE RESILIENCY

                
        for(int j=0;j < NUM_REMOTE_VERTICES; j++)
        {
            if(node_check[j] == 0 && node_counter[j] >= 0)
                node_counter[j] += 1;
            else if(node_check[j] == 1 ) 
                node_counter[j] = 0;
            

            
            //Serial << "node_counter ";
            //Serial << j;
            //Serial << " : ";
            //Serial << node_counter[j];
            //Serial << "\n";
            

            if(node_counter[j] >= int(iterations/2) )
            {
                s->setStatus(j+1, 1);
                s->decrementInDegree();
                uint8_t dout = s->getOutDegree();              //since we assume it is a bidirectional graph, InDegree is equivalent to OutDegree
                s->setOutDegree(dout - 1); 
                node_counter[j] = -1;                          //set counter to -1 when limit reached to indicate offline link status    
            }
            node_check[j] = 0;                                 //reset node_check after each iteration
        }

        /*  old approach
        step_counter = 0;

        for(int j=0;j < no_of_nodes; j++)
        {
            if(online[j])
            {
                if(step_counter > 0)
                {
                     node_counter[j - step_counter] = node_counter[j];
                     _G->AdjustVertexArray(j,step_counter);
                } 
            }
            else
            {
                _G->removeInNeighbor(j);
                step_counter++;
            }
        }
        no_of_nodes = no_of_nodes - step_counter;
        
        */
        iter++;// increase the iteration count

        //Serial<<"value of Y: "<<endY<<", value of Z: "<<endZ<<endl;
    }while(iter < iterations); //we need to implement here the max consensus
//}while(iter < iterations && -(endY) > eps && (endZ) > eps);
    if(s->getZ() != 0)
        _buffer[0] = (s->getYMin()/s->getZ()); 


    _buffer[1] = s->getOutDegree();
    _buffer[2] = _G->getN() - 1;

    
    //Serial << "RC from Library";
    //Serial << float(s->getYMin())/float(s->getZ());
    //Serial <<"\n";  
    return (s->getYMin()/s->getZ());
}


// long OAgent::computeFairSplitFinalValue(float gamma) {
//     OLocalVertex * s = _G->getLocalVertex();
//  if(gamma <= 0)
//      return s->getMin();
//     else if(gamma < 1)
//         // if under maximum system capacity
//          return s->getMin() + long(gamma*float(s->getMax()-s->getMin()));
//     else if(gamma >= 1)
//         return s->getMax();    
// }



float OAgent::fairSplitRatioConsensus_RSL(float y, float z, uint8_t iterations, uint16_t period) {
    srand(analogRead(7));    
    float gamma = 0;
    
    if(isLeader())
    {
        // Serial<<"going into leader RC  "<<endl;
        // delay(5);     
        gamma = leaderFairSplitRatioConsensus_RSL(y, z, iterations,period);
    }
    else
    {

        // Serial<<"going into nonleader RC "<<endl;
        // delay(5);
        gamma = nonleaderFairSplitRatioConsensus_RSL(y, z, iterations,period);
    }
        //Serial<<"Sup bro?! "<<getbufferdata(0)<<"\n";

    return gamma;
}




float OAgent::leaderFairSplitRatioConsensus_RSL(float y, float z, uint8_t iterations, uint16_t period) {
    unsigned long t0 = myMillis();
    unsigned long startTime = t0 + RC_DELAY;
    OLocalVertex * s = _G->getLocalVertex();
    float gamma = 0;
    bool scheduled =_waitForChildSchedulePacketRC(SCHEDULE_FAIR_SPLIT_HEADER,SCHEDULE_TIMEOUT, startTime, iterations, period);
    //Serial<<"Schedule done at "<<myMillis()<<"\n";
    //bool stat = startTime>myMillis();

    //Serial<<"Leader: Startime= "<<startTime<<", Time= "<<myMillis()<<"\n";

    if (!scheduled) 
    {
        Serial<<"RC scheduling was a FAIL!"<<endl;
        delay(5);
        gamma = -1;
    }
    else
    {
        Serial<<"RC scheduling was a SUCCESS!"<<endl;
        delay(5);
        Serial << "Correct Startime is " <<startTime<<", and current time is "<< myMillis()<<endl;
        delay(5);
        if(_waitToStart(startTime,true,10000))
        {
            Serial <<"My startime is "<< myMillis() <<endl;
            delay(5);
            gamma = ratiomaxminConsensus(y, z, iterations,period);
        }
    }        
    return gamma;
}

float OAgent::nonleaderFairSplitRatioConsensus_RSL(float y, float z, uint8_t iterations, uint16_t period) {
    unsigned long startTime = 0;
    //delay(50);
    float gamma = 0;
    bool scheduled = _waitForParentSchedulePacketRC(startTime,iterations,period,-1);
    //Serial<<"Schedule done at "<<myMillis()<<"\n";
    
    //bool stat = startTime>myMillis();
    //Serial<<"Startime > Time? "<<stat<<"\n";

    //Serial<<"NonLeader: Startime= "<<startTime<<", Time= "<<myMillis()<<"\n";

    if(scheduled)
    {
        Serial<<"RC scheduling was a SUCCESS!"<<endl;
        delay(5);
        Serial << "Correct Startime is " <<startTime<<", and current time is "<< myMillis()<<endl;
        delay(5);
        if(_waitToStart(startTime,true,10000)) {
            Serial <<"My startime is "<< myMillis() <<endl;
            delay(5);
            gamma = ratiomaxminConsensus(y, z, iterations,period);
        }
        
        //digitalWrite(48,LOW);
    }
    else
    {
        Serial<<"RC scheduling was a FAIL!"<<endl;
        delay(5);
        //Serial<<"No schedule received from Node "<<s->getleaderID()<<"\n";
        //s->setleaderID(s->getdeputyID());
        //Serial<<"Node "<<s->getleaderID()<<" is the new leader\n";
        gamma = -1;
    }
    return gamma;
}



long OAgent::leaderFairSplitRatioConsensus(long y, long z, uint8_t iterations, uint16_t period) {
    srand(analogRead(7));                    //moved this instruction here from fairSplitRatioConsensus() - Sammy
    unsigned long t0 = millis(); 
    unsigned long startTime = t0 + 1200;                         //was 1200 initially
    _broadcastScheduleFairSplitPacket(startTime,iterations,period);
    _waitForACKPacket(ACK_START_HEADER, t0, startTime, iterations, period);
    float gamma = 0;
    _buffer[2] = 0;
    if(_waitToStart(startTime,false,1800)) {
        gamma = fairSplitRatioConsensus(y, z, iterations,period);
    }
    return gamma;
}

long OAgent::nonleaderFairSplitRatioConsensus(long y, long z) {
    srand(analogRead(7));
    unsigned long startTime = 0;
    uint8_t iterations = 0;
    uint16_t period = 0;
    OLocalVertex * s = _G->getLocalVertex();
    uint8_t id = s->getID();
    _waitForScheduleFairSplitPacket(startTime,iterations,period,id);
    digitalWrite(48,HIGH);
    // long final = initial;
    
    float gamma = 0;
    if(_waitToStart(startTime,true,1800)) {
        gamma = fairSplitRatioConsensus(y, z, iterations,period);
    }
    digitalWrite(48,LOW);
    return gamma;
}


// End fair splitting
/// End ratio-consensus





//  Begin Max Consensus
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long OAgent::maxminConsensusAlgorithm(bool isMax, long max, long min, uint8_t iterations, uint16_t period) {
    srand(analogRead(7));
    long gamma = 0;

    if(isLeader())
        gamma = leaderMaxMinConsensus(isMax,max,min,iterations,period);
    else
        gamma = nonleaderMaxMinConsensus(isMax,max,min,iterations,period);
    return gamma;
}

long OAgent::leaderMaxMinConsensus(bool isMax, long max, long min, uint8_t iterations, uint16_t period) {
    unsigned long t0 = myMillis();
    unsigned long startTime = t0 + MC_DELAY;
    long gamma = 0;
    bool scheduled =_waitForChildSchedulePacketRC(SCHEDULE_MAXMIN_HEADER,SCHEDULE_TIMEOUT,startTime,iterations,period);

    if (!scheduled)
    {
        Serial << "maxmin scheduling was a FAIL"<<endl;
        gamma = -1;
    }
    else
    {
        Serial << "maxmin scheduling was a SUCCESS"<<endl;
        if(_waitToStart(startTime,true,10000))
        {
            //Serial << "Correct Startime is " <<startTime<< ". My startime is "<< myMillis() <<endl;
            gamma = maxminConsensus(isMax,max,min,iterations,period);
        }
    }        
    return gamma;
}

long OAgent::nonleaderMaxMinConsensus(bool isMax, long max, long min, uint8_t iterations, uint16_t period) {
    unsigned long startTime = 0;
    long gamma = 0;

    bool scheduled = _waitForScheduleMaxMinPacket_RSL(startTime,iterations,period,-1);

    if(scheduled)
    {
        Serial << "maxmin scheduling was a SUCCESS"<<endl;
        if(_waitToStart(startTime,true,10000))
        {
            //Serial << "Correct Startime is " <<startTime<< ". My startime is "<< myMillis() <<endl;
            gamma = maxminConsensus(isMax,max,min,iterations,period);
        }
    }
    else
    {
        Serial << "maxmin scheduling was a FAIL"<<endl;
        gamma = -1;
    }
    return gamma;
}

// MaxMin Consensus Algorithm (added in by Olaolu)
long OAgent::maxminConsensus(bool isMax, long max, long min, uint8_t iterations, uint16_t period) {
    unsigned long start;                // create variable to store iteration start time
    bool txDone;                        // create variable to keep track of broadcasts
    uint16_t txTime;       //_genTxTime(period,10,analogRead(0));   // get transmit time; 

    long inMax;
    long inMin;
    int count = 3;
    uint32_t aLsb;
    
    int frame = 20;
    
    for(uint8_t k = 0; k < iterations; k++) {
        srand(analogRead(0));
        txTime =  (rand() % (period - 2*frame)) + frame;  //determines the time window in which a payload is transmitted
        txDone = false;     // initialize toggle to keep track of broadcasts
        start = millis();   // initialize timer
        
        while(uint16_t(millis()-start) < period) {
            if(_maxminPacketAvailable()) {
                long inMax = _getMaxFromPacket();                               // store incoming value of Max
                long inMin = _getMinFromPacket();                               // store incoming value of Min
                if(inMax > max)
                    max = inMax;
                if(inMin < min)
                    min = inMin;
            }
            if((int((millis() - start)) >= txTime) && !txDone) {
                txDone = true; // toggle txDone
                _broadcastMaxMinPacket(max,min);
            }
        }
        // Serial << "At iteration "<< k <<" we have "<< max <<" windows"<<endl;
        // delay(5);
    }
    if(isMax)
        return max;
    else
        return min;
}



long OAgent::_getMaxFromPacket() {
    uint8_t ptr = 2;
    return _getUint32_tFromPacket(ptr);
}

long OAgent::_getMinFromPacket() {
    uint8_t ptr = 6;
    return _getUint32_tFromPacket(ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  End Max Consensus






// Primal Dual methods

bool OAgent::primalDualAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    srand(analogRead(7));
    bool gamma = false;

    if(isLeader())
    { 
        gamma = leaderPrimalDualAlgorithm(genBus,alpha,iterations);
    }
    else
    {
        gamma = nonleaderPrimalDualAlgorithm(genBus,alpha,iterations);
    }
        //Serial<<"Sup bro?! "<<getbufferdata(0)<<"\n";

    return gamma;

}

bool OAgent::leaderPrimalDualAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    unsigned long t0 = myMillis();
    unsigned long startTime = t0 + PD_DELAY;
    bool gamma = false;
    bool scheduled = _waitForChildSchedulePacketPD(SCHEDULE_TIMEOUT,startTime,iterations);
    //Serial<<"Schedule done at "<<myMillis()<<"\n";
    //bool stat = startTime>myMillis();

    //Serial<<"Leader: Startime= "<<startTime<<", Time= "<<myMillis()<<"\n";

    if (!scheduled) 
    {
        Serial<<"PD scheduling was a FAIL"<<endl;
        gamma = false;
    }
    else
    {
        Serial<<"PD scheduling was a SUCCESS"<<endl;
        if(_waitToStart(startTime,true,10000))
        {
            Serial << "Correct Startime is " <<startTime<< ". My startime is "<< myMillis() <<endl;
            // gamma = standardPrimalDualAlgorithm0(genBus,alpha,iterations);
            // gamma = acceleratedPrimalDualAlgorithm0(genBus,alpha,iterations);
            // gamma = standardPrimalDualAlgorithm1(genBus,alpha,iterations);
            gamma = acceleratedPrimalDualAlgorithm1(genBus,alpha,iterations);
            // gamma = standardPrimalDualAlgorithm2(genBus,alpha,iterations);
            // gamma = acceleratedPrimalDualAlgorithm2(genBus,alpha,iterations);
        }
    }        
    return gamma;
}

bool OAgent::nonleaderPrimalDualAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    unsigned long startTime = 0;
    bool gamma = false;
    //delay(50);
    bool scheduled = _waitForParentSchedulePacketPD(startTime,iterations,-1);
    //Serial<<"Schedule done at "<<myMillis()<<"\n";
    
    //bool stat = startTime>myMillis();
    //Serial<<"Startime > Time? "<<stat<<"\n";

    //Serial<<"NonLeader: Startime= "<<startTime<<", Time= "<<myMillis()<<"\n";

    if(scheduled)
    {
        Serial<<"PD scheduling was a SUCCESS"<<endl;
        if(_waitToStart(startTime,true,10000))
        {
            Serial << "Correct Startime is " <<startTime<< ". My startime is "<< myMillis() <<endl;
            // gamma = standardPrimalDualAlgorithm0(genBus,alpha,iterations);
            // gamma = acceleratedPrimalDualAlgorithm0(genBus,alpha,iterations);
            // gamma = standardPrimalDualAlgorithm1(genBus,alpha,iterations);
            gamma = acceleratedPrimalDualAlgorithm1(genBus,alpha,iterations);
            // gamma = standardPrimalDualAlgorithm2(genBus,alpha,iterations);
            // gamma = acceleratedPrimalDualAlgorithm2(genBus,alpha,iterations);
        }
        //digitalWrite(48,LOW);
    }
    else
    {
        Serial<<"PD scheduling was a FAIL"<<endl;
        gamma = false;
    }
    return gamma;
}


bool OAgent::standardPrimalDualAlgorithm0(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex
    iterations = 400;
    alpha = 0.1;
    uint16_t nodeID = s->getID();
    uint8_t neighborID;

    float fp;                                                                                   // node state variable active flow
    float fq;                                                                                   // node state variable reactive flow
    float g_fp;
    float g_fq;

    float delta_fp;                                                                             // change in node state variable active flow
    float delta_fq;                                                                             // change in node state variable reactive flow
    float delta_gfp;                                                                             // change in node state variable active flow
    float delta_gfq;                                                                             // change in node state variable reactive flow

    float node_fp;                                                                          // neighbor state variable active flow
    float node_fq;                                                                          // neighbor state variable reactive flow
    float node_gfp;                                                                          // neighbor state variable active flow
    float node_gfq;                                                                          // neighbor state variable reactive flow

    float neighbor_fp;                                                                          // neighbor state variable active flow
    float neighbor_fq;                                                                          // neighbor state variable reactive flow
    float neighbor_gfp;                                                                          // neighbor state variable active flow
    float neighbor_gfq;                                                                          // neighbor state variable reactive flow
    
    float neighbornode_fp;
    float neighbornode_fq;
    
    float neighbornode_gfp;
    float neighbornode_gfq;

    bool node_flag;
    bool neighbor_flag;
    bool neighbor_status;
    
    float P = (float)genBus*(s->getActiveSetpoint());                                                  // active injection
    float Q = (float)genBus*(s->getReactiveSetpoint());                                                // reactive injection
    float Pd = s->getActiveDemand();                                                            // active demand
    float Qd = s->getReactiveDemand();                                                          // reactive demand
    float Mu = s->getMu();                                                                      // lagrange multiplier
    float Nu = s->getNu();                                                                      // lagrange multiplier
    float bP = P - Pd - l->addActiveFlows(nodeID,n);                                                  // active balance
    float bQ = Q - Qd - l->addReactiveFlows(nodeID,n);                                                 // reactive balance
    
    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts
    bool rxDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 100;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 20;
    
    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer
        // Serial<<"Iteration "<<k+1<<endl;
        P = P - (float)genBus*alpha*( Mu+((s->getWp())*bP) );
        Q = Q - (float)genBus*alpha*( Nu+((s->getWq())*bQ) );
        Mu = Mu + alpha*bP;
        Nu = Nu + alpha*bQ;
        
        txDone = false;
        rxDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForUnicastPacket(neighborID,nodeID,PD_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get values for fp, fq, and lambda that are received from this neighbor
                neighbor_fp = _getActiveFlowFromPacket_neighbor();                                      // store incoming value of fp
                neighbor_fq = _getReactiveFlowFromPacket_neighbor();                                    // store incoming value of fq
                neighbor_gfp = _getActiveFlowGradientFromPacket_neighbor();                                     // store incoming value of fp
                neighbor_gfq = _getReactiveFlowGradientFromPacket_neighbor();                                   // store incoming value of fq

                neighbor_status = _getFlagFromPacket_ACC();                                                 // store incoming value of lambda

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                node_flag = neighborP->getNodeFlag();
                neighbor_flag = neighborP->getNeighborFlag();
                
                if(neighborID < nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                    node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                    node_gfp = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1)) ;
                    node_gfq =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1)) ;

                    // Serial<<"Received from Parent Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
              //    delay(5);

                    if (neighbor_flag != neighbor_status)
                    {  
                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                    //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);
                                    
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(node_fp + neighbor_fp);
                        fq = 0.5*(node_fq + neighbor_fq);

                        g_fp = 0.5*(node_gfp + neighbor_gfp);
                        g_fq = 0.5*(node_gfq + neighbor_gfq);
                                            
                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        neighborP->setNeighborActiveFlow(neighbor_fp);
                        neighborP->setNeighborReactiveFlow(neighbor_fq);
                                            
                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNeighborActiveFlowGradient(neighbor_gfp);
                        neighborP->setNeighborReactiveFlowGradient(neighbor_gfq);
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);

                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
           //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq<<endl;
                    // delay(5);
                }
                else if(neighborID > nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getNodeActiveFlow());
                    node_fq = (neighborP->getNodeReactiveFlow());
                    node_gfp = (neighborP->getNodeActiveFlowGradient());
                    node_gfq = (neighborP->getNodeReactiveFlowGradient());

                    // Serial<<"Received from Child Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
                    // delay(5);

                    if (neighbor_flag != neighbor_status)
                    {
                        neighbornode_fp = _getActiveFlowFromPacket_nodeACC();                                       // store incoming value of fp from child
                        neighbornode_fq = _getReactiveFlowFromPacket_nodeACC();                                     // store incoming value of fq from child
                        neighbornode_gfp = _getActiveFlowGradientFromPacket_node();                                     // store incoming value of fp from child
                        neighbornode_gfq = _getReactiveFlowGradientFromPacket_node();                                   // store incoming value of fq from child

                        // Serial<<"Received self data from Child Node "<<neighborID<<":"<<endl<<node_fp<<" , "<<node_fq<<" , "<<node_lambda<<" , "<<node_gfp<<" , "<<node_gfq<<" , "<<node_glambda<<" , "<<neighbor_status<<endl;
                        // delay(5);

                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                                                  //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);                                        //update neighbor's status since neighbor's averaging step was successful
                        
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(neighbornode_fp+neighbor_fp);
                        fq = 0.5*(neighbornode_fq+neighbor_fq);

                        g_fp = 0.5*(neighbornode_gfp + neighbor_gfp);
                        g_fq = 0.5*(neighbornode_gfq + neighbor_gfq);

                        delta_fp = node_fp - neighbornode_fp;
                        delta_fq = node_fq - neighbornode_fq;

                        delta_gfp = node_gfp - neighbornode_gfp;
                        delta_gfq = node_gfq - neighbornode_gfq;
                        
                        fp = fp + delta_fp;
                        fq = fq + delta_fq;

                        g_fp = g_fp + delta_gfp;
                        g_fq = g_fq + delta_gfq;
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);

                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
           //          Serial << "fp ("<<nodeID<<","<<neighborID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<nodeID<<","<<neighborID<<") = "<<fq<<endl;
                    // delay(5);
                }
            }

            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                neighborID = l->unlinkLinkedListNodes();
                //Serial<<"Neighbor id is "<<neighborID<<endl;
                while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
                {
                    neighborP = (n+(neighborID-1));
                    if(neighborID < nodeID)
                    {
                        _sendToParent(neighborID,neighborP->getNodeActiveFlow(),neighborP->getNodeReactiveFlow(),0,neighborP->getNodeActiveFlowGradient(),neighborP->getNodeReactiveFlowGradient(),0,neighborP->getNodeFlag(),neighborP->getNeighborActiveFlow(),neighborP->getNeighborReactiveFlow(),0,neighborP->getNeighborActiveFlowGradient(),neighborP->getNeighborReactiveFlowGradient(),0);
                        // Serial<<"Sent to Parent Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<" , "<<neighborP->getNeighborActiveFlow()<<" , "<<neighborP->getNeighborReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNeighborActiveFlowGradient()<<" , "<<neighborP->getNeighborReactiveFlowGradient()<<" , "<<0<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<";";
                    }
                    else if(neighborID > nodeID)
                    {
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                        node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                        node_gfp = 2*alpha*(Mu + ((s->getWp())*bP)) ;
                        node_gfq =  2*alpha*(Nu + ((s->getWq())*bQ)) ;

                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        _sendToChild(neighborID,node_fp,node_fq,0,node_gfp,node_gfq,0,neighborP->getNodeFlag());
                        // Serial<<"Sent to Child Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<";";
                    }
                    neighborID = l->unlinkLinkedListNodes();
                    neighborP = (n+(neighborID-1));
                }
                //l->updateLinkedList(s->getStatusP());
            }
        }
        l->updateLinkedList(s->getStatusP());
        l->updateActiveLinks(n);
        l->resetLinkedListStatus(s->getStatusP());

        neighborID = l->findInActiveLink(n);                                                                                            //find neighbors that no packet was received from
        neighborP = (n+(neighborID-1));
        
        while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
        {
            // Serial<<neighborID<<" inactive"<<endl;
            // delay(5);

            packetDropCount++;

            if(neighborID < nodeID)
            {
                node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                node_gfp = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1)) ;
                node_gfq =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1)) ;

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
            }
            else if(neighborID > nodeID)
            {
                node_gfp = neighborP->getNodeActiveFlowGradient();
                node_gfq = neighborP->getNodeReactiveFlowGradient();

                //get values for fp, fq, and lambda that are currently associated with this neighbor
                node_fp = (neighborP->getNodeActiveFlow());
                node_fq = (neighborP->getNodeReactiveFlow());

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
            }
   //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp + g_node_fp<<endl;
            // delay(5);
   //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq + g_node_fq<<endl;
            // delay(5);

            neighborID = l->findInActiveLink(n);
            neighborP = (n+(neighborID-1));
        }
        l->resetActiveLinks(n);
        l->updateLinkedList(s->getStatusP());
        bP = P - Pd - l->addActiveFlows(nodeID,n);
        bQ = Q - Qd - l->addReactiveFlows(nodeID,n);
      //   if (packetDropCount+packetReceiveCount > (_G->getN()-1))
      //   {
      //    Serial<<"ERROR! "<<packetDropCount<<" packet drops encountered"<<endl;
            // delay(5);
            // Serial<<"ERROR! "<<packetReceiveCount<<" packets received"<<endl;
            // delay(5);
      //   }
        packetsLost += packetDropCount;
        packetReceived += packetReceiveCount;

        packetDropCount = 0;
        packetReceiveCount = 0;

        Serial<<_FLOAT(P,6)<<","<<_FLOAT(Q,6)<<","<<_FLOAT(bP,6)<<","<<_FLOAT(bQ,6)<<","<<_FLOAT(Mu,6)<<","<<_FLOAT(Nu,6)<<endl;//<<"Active injection: "<<P<<endl<<"Reactive injection: "<<Q<<endl;
        delay(5);
    }

    s->setActiveSetpoint(P);
    s->setReactiveSetpoint(Q);
    s->setActiveBalance(bP);
    s->setReactiveBalance(bQ);
    s->setMu(Mu);
    s->setNu(Nu);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}

bool OAgent::acceleratedPrimalDualAlgorithm0(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex
    iterations = 400;
    alpha = 0.1;
    uint16_t nodeID = s->getID();
    uint8_t neighborID;

    float fp;                                                                                   // node state variable active flow
    float fq;                                                                                   // node state variable reactive flow
    float g_fp;
    float g_fq;

    float delta_fp;                                                                             // change in node state variable active flow
    float delta_fq;                                                                             // change in node state variable reactive flow
    float delta_gfp;                                                                             // change in node state variable active flow
    float delta_gfq;                                                                             // change in node state variable reactive flow

    float g_fpTMP;
    float g_fqTMP;

    float node_fp;                                                                          // neighbor state variable active flow
    float node_fq;                                                                          // neighbor state variable reactive flow
    float node_gfp;                                                                          // neighbor state variable active flow
    float node_gfq;                                                                          // neighbor state variable reactive flow

    float neighbor_fp;                                                                          // neighbor state variable active flow
    float neighbor_fq;                                                                          // neighbor state variable reactive flow
    float neighbor_gfp;                                                                          // neighbor state variable active flow
    float neighbor_gfq;                                                                          // neighbor state variable reactive flow
    
    float neighbornode_fp;
    float neighbornode_fq;
    
    float neighbornode_gfp;
    float neighbornode_gfq;

    bool node_flag;
    bool neighbor_flag;
    bool neighbor_status;
    
    float P = (float)genBus*(s->getActiveSetpoint());                                                  // active injection
    float Q = (float)genBus*(s->getReactiveSetpoint());                                                // reactive injection
    float Pd = s->getActiveDemand();                                                            // active demand
    float Qd = s->getReactiveDemand();                                                          // reactive demand
    float Mu = s->getMu();                                                                      // lagrange multiplier
    float Nu = s->getNu();                                                                      // lagrange multiplier
    float bP = P - Pd - l->addActiveFlows(nodeID,n);                                                  // active balance
    float bQ = Q - Qd - l->addReactiveFlows(nodeID,n);                                                 // reactive balance
    
    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts
    bool rxDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 100;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 20;
    
    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {        
        start = millis();   // initialize timer
        // Serial<<"Iteration "<<k+1<<endl;
        P = P - (float)genBus*alpha*( Mu+((s->getWp())*bP) );
        Q = Q - (float)genBus*alpha*( Nu+((s->getWq())*bQ) );
        Mu = Mu + alpha*bP;
        Nu = Nu + alpha*bQ;
        
        txDone = false;
        rxDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForUnicastPacket(neighborID,nodeID,PD_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get values for fp, fq that are received from this neighbor
                neighbor_fp = _getActiveFlowFromPacket_neighbor();                                      // store incoming value of fp
                neighbor_fq = _getReactiveFlowFromPacket_neighbor();                                    // store incoming value of fq
                neighbor_gfp = _getActiveFlowGradientFromPacket_neighbor();                                     // store incoming value of fp
                neighbor_gfq = _getReactiveFlowGradientFromPacket_neighbor();                                   // store incoming value of fq

                neighbor_status = _getFlagFromPacket_ACC();                                                 // store incoming value of lambda

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                node_flag = neighborP->getNodeFlag();
                neighbor_flag = neighborP->getNeighborFlag();
                
                if(neighborID < nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                    node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                    g_fpTMP = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1)) ;
                    g_fqTMP =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1)) ;

                    node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                    node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );

                    neighborP->setActiveFlowGradientTMP(g_fpTMP);
                    neighborP->setReactiveFlowGradientTMP(g_fqTMP);

                    // Serial<<"Received from Parent Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<0<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<0<<" , "<<neighbor_status<<endl;
              //    delay(5);

                    if (neighbor_flag != neighbor_status)
                    {  
                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                    //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);
                                    
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(node_fp + neighbor_fp);
                        fq = 0.5*(node_fq + neighbor_fq);

                        g_fp = 0.5*(node_gfp + neighbor_gfp);
                        g_fq = 0.5*(node_gfq + neighbor_gfq);
                                            
                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        neighborP->setNeighborActiveFlow(neighbor_fp);
                        neighborP->setNeighborReactiveFlow(neighbor_fq);
                                            
                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNeighborActiveFlowGradient(neighbor_gfp);
                        neighborP->setNeighborReactiveFlowGradient(neighbor_gfq);
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);

                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
           //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq<<endl;
                    // delay(5);
                }
                else if(neighborID > nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getNodeActiveFlow());
                    node_fq = (neighborP->getNodeReactiveFlow());
                    node_gfp = (neighborP->getNodeActiveFlowGradient());
                    node_gfq = (neighborP->getNodeReactiveFlowGradient());

                    // Serial<<"Received from Child Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<0<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<0<<" , "<<neighbor_status<<endl;
                    // delay(5);

                    if (neighbor_flag != neighbor_status)
                    {
                        neighbornode_fp = _getActiveFlowFromPacket_nodeACC();                                       // store incoming value of fp from child
                        neighbornode_fq = _getReactiveFlowFromPacket_nodeACC();                                     // store incoming value of fq from child
                        neighbornode_gfp = _getActiveFlowGradientFromPacket_node();                                     // store incoming value of fp from child
                        neighbornode_gfq = _getReactiveFlowGradientFromPacket_node();                                   // store incoming value of fq from child

                        // Serial<<"Received self data from Child Node "<<neighborID<<":"<<endl<<node_fp<<" , "<<node_fq<<" , "<<0<<" , "<<node_gfp<<" , "<<node_gfq<<" , "<<0<<" , "<<neighbor_status<<endl;
                        // delay(5);

                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                                                  //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);                                        //update neighbor's status since neighbor's averaging step was successful
                        
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(neighbornode_fp+neighbor_fp);
                        fq = 0.5*(neighbornode_fq+neighbor_fq);

                        g_fp = 0.5*(neighbornode_gfp + neighbor_gfp);
                        g_fq = 0.5*(neighbornode_gfq + neighbor_gfq);

                        delta_fp = node_fp - neighbornode_fp;
                        delta_fq = node_fq - neighbornode_fq;

                        delta_gfp = node_gfp - neighbornode_gfp;
                        delta_gfq = node_gfq - neighbornode_gfq;
                        
                        fp = fp + delta_fp;
                        fq = fq + delta_fq;

                        g_fp = g_fp + delta_gfp;
                        g_fq = g_fq + delta_gfq;
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);

                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
           //          Serial << "fp ("<<nodeID<<","<<neighborID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<nodeID<<","<<neighborID<<") = "<<fq<<endl;
                    // delay(5);
                }
            }

            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                neighborID = l->unlinkLinkedListNodes();
                //Serial<<"Neighbor id is "<<neighborID<<endl;
                while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
                {
                    neighborP = (n+(neighborID-1));
                    if(neighborID < nodeID)
                    {
                        _sendToParent(neighborID,neighborP->getNodeActiveFlow(),neighborP->getNodeReactiveFlow(),0,neighborP->getNodeActiveFlowGradient(),neighborP->getNodeReactiveFlowGradient(),0,neighborP->getNodeFlag(),neighborP->getNeighborActiveFlow(),neighborP->getNeighborReactiveFlow(),0,neighborP->getNeighborActiveFlowGradient(),neighborP->getNeighborReactiveFlowGradient(),0);
                        // Serial<<"Sent to Parent Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<" , "<<neighborP->getNeighborActiveFlow()<<" , "<<neighborP->getNeighborReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNeighborActiveFlowGradient()<<" , "<<neighborP->getNeighborReactiveFlowGradient()<<" , "<<0<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<";";
                    }
                    else if(neighborID > nodeID)
                    {
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                        node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                        g_fpTMP = 2*alpha*(Mu + ((s->getWp())*bP));
                        g_fqTMP =  2*alpha*(Nu + ((s->getWq())*bQ));

                        node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                        node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );

                        neighborP->setActiveFlowGradientTMP(g_fpTMP);
                        neighborP->setReactiveFlowGradientTMP(g_fqTMP);

                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        _sendToChild(neighborID,node_fp,node_fq,0,node_gfp,node_gfq,0,neighborP->getNodeFlag());
                        // Serial<<"Sent to Child Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<";";
                    }
                    neighborID = l->unlinkLinkedListNodes();
                    neighborP = (n+(neighborID-1));
                }
                //l->updateLinkedList(s->getStatusP());
            }
        }
        l->updateLinkedList(s->getStatusP());
        l->updateActiveLinks(n);
        l->resetLinkedListStatus(s->getStatusP());

        neighborID = l->findInActiveLink(n);                                                                                            //find neighbors that no packet was received from
        neighborP = (n+(neighborID-1));
        
        while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
        {
            // Serial<<neighborID<<" inactive"<<endl;
            // delay(5);

            packetDropCount++;

            if(neighborID < nodeID)
            {
                g_fpTMP = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1));
                g_fqTMP =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1));

                node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );

                neighborP->setActiveFlowGradientTMP(g_fpTMP);
                neighborP->setReactiveFlowGradientTMP(g_fqTMP);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
            }
            else if(neighborID > nodeID)
            {
                node_gfp = neighborP->getNodeActiveFlowGradient();
                node_gfq = neighborP->getNodeReactiveFlowGradient();

                //get values for fp, fq that are currently associated with this neighbor
                node_fp = (neighborP->getNodeActiveFlow());
                node_fq = (neighborP->getNodeReactiveFlow());

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
            }
   //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp + g_node_fp<<endl;
            // delay(5);
   //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq + g_node_fq<<endl;
            // delay(5);

            neighborID = l->findInActiveLink(n);
            neighborP = (n+(neighborID-1));
        }
        l->resetActiveLinks(n);
        l->updateLinkedList(s->getStatusP());
        bP = P - Pd - l->addActiveFlows(nodeID,n);
        bQ = Q - Qd - l->addReactiveFlows(nodeID,n);
      //   if (packetDropCount+packetReceiveCount > (_G->getN()-1))
      //   {
      //    Serial<<"ERROR! "<<packetDropCount<<" packet drops encountered"<<endl;
            // delay(5);
            // Serial<<"ERROR! "<<packetReceiveCount<<" packets received"<<endl;
            // delay(5);
      //   }
        packetsLost += packetDropCount;
        packetReceived += packetReceiveCount;

        packetDropCount = 0;
        packetReceiveCount = 0;

        Serial<<_FLOAT(P,6)<<","<<_FLOAT(Q,6)<<","<<_FLOAT(bP,6)<<","<<_FLOAT(bQ,6)<<","<<_FLOAT(Mu,6)<<","<<_FLOAT(Nu,6)<<endl;//<<"Active injection: "<<P<<endl<<"Reactive injection: "<<Q<<endl;
        delay(5);
    }

    s->setActiveSetpoint(P);
    s->setReactiveSetpoint(Q);
    s->setActiveBalance(bP);
    s->setReactiveBalance(bQ);
    s->setMu(Mu);
    s->setNu(Nu);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}


bool OAgent::standardPrimalDualAlgorithm1(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex
    iterations = 750;
    alpha = 0.1;
    uint16_t nodeID = s->getID();
    uint8_t neighborID;

    float fp;                                                                                   // node state variable active flow
    float fq;                                                                                   // node state variable reactive flow
    float lambda;                                                                               // node state variable lagrange multiplier
    float g_fp;
    float g_fq;
    float g_lambda;

    float delta_fp;                                                                             // change in node state variable active flow
    float delta_fq;                                                                             // change in node state variable reactive flow
    float delta_lambda;                                                                         // change in node state variable lagrange multiplier
    float delta_gfp;                                                                             // change in node state variable active flow
    float delta_gfq;                                                                             // change in node state variable reactive flow
    float delta_glambda;                                                                         // change in node state variable lagrange multiplier

    float node_fp;                                                                          // neighbor state variable active flow
    float node_fq;                                                                          // neighbor state variable reactive flow
    float node_lambda;  
    float node_gfp;                                                                          // neighbor state variable active flow
    float node_gfq;                                                                          // neighbor state variable reactive flow
    float node_glambda;

    float neighbor_fp;                                                                          // neighbor state variable active flow
    float neighbor_fq;                                                                          // neighbor state variable reactive flow
    float neighbor_lambda;  
    float neighbor_gfp;                                                                          // neighbor state variable active flow
    float neighbor_gfq;                                                                          // neighbor state variable reactive flow
    float neighbor_glambda;  
    
    float neighbornode_fp;
    float neighbornode_fq;
    float neighbornode_lambda;
    
    float neighbornode_gfp;
    float neighbornode_gfq;
    float neighbornode_glambda;

    bool node_flag;
    bool neighbor_flag;
    bool neighbor_status;
    
    float P = (float)genBus*(s->getActiveSetpoint());                                                  // active injection
    float Q = (float)genBus*(s->getReactiveSetpoint());                                                // reactive injection
    float Pd = s->getActiveDemand();                                                            // active demand
    float Qd = s->getReactiveDemand();                                                          // reactive demand
    float sqV = s->getSquareVoltage();                                                          // square voltage magnitude
    float Mu = s->getMu();                                                                      // lagrange multiplier
    float Nu = s->getNu();                                                                      // lagrange multiplier
    float bP = P - Pd - l->addActiveFlows(nodeID,n);                                                  // active balance
    float bQ = Q - Qd - l->addReactiveFlows(nodeID,n);                                                 // reactive balance
    
    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts
    bool rxDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 200;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 25;
    
    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer
        // Serial<<"Iteration "<<k+1<<endl;
        P = P - (float)genBus*alpha*( Mu+((s->getDp())*P)+((s->getWp())*bP) );
        Q = Q - (float)genBus*alpha*( Nu+((s->getDq())*Q)+((s->getWq())*bQ) );
        sqV = sqV - alpha*( l->addLambdas(nodeID,n)+((s->getWv())*(sqV-1)) );
        
        if (sqV < 0.8)
            sqV = 0.8;
        else if (sqV > 1.2)
            sqV = 1.2;

        Mu = Mu + alpha*bP;
        Nu = Nu + alpha*bQ;
        
        txDone = false;
        rxDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForUnicastPacket(neighborID,nodeID,PD_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get values for fp, fq, and lambda that are received from this neighbor
                neighbor_fp = _getActiveFlowFromPacket_neighbor();                                      // store incoming value of fp
                neighbor_fq = _getReactiveFlowFromPacket_neighbor();                                    // store incoming value of fq
                neighbor_lambda = _getLambdaFromPacket_neighbor();                                      // store incoming value of lambda
                neighbor_gfp = _getActiveFlowGradientFromPacket_neighbor();                                     // store incoming value of fp
                neighbor_gfq = _getReactiveFlowGradientFromPacket_neighbor();                                   // store incoming value of fq
                neighbor_glambda = _getLambdaGradientFromPacket_neighbor();                                     // store incoming value of lambda

                neighbor_status = _getFlagFromPacket_ACC();                                                 // store incoming value of lambda

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                node_flag = neighborP->getNodeFlag();
                neighbor_flag = neighborP->getNeighborFlag();
                
                if(neighborID < nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                    node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());
                    node_lambda = (neighborP->getLambda()) + (neighborP->getLambdaGradient());

                    node_gfp = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) + ((neighborP->getResistance())*(neighborP->getLambda()))) ;
                    node_gfq =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) + ((neighborP->getReactance())*(neighborP->getLambda()))) ;
                    node_glambda= 2*alpha*(sqV*(-1) - ((neighborP->getActiveFlow())*(neighborP->getResistance())) - ((neighborP->getReactiveFlow())*(neighborP->getReactance())) );

                    // Serial<<"Received from Parent Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
              //    delay(5);

                    if (neighbor_flag != neighbor_status)
                    {  
                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                    //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);
                                    
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(node_fp + neighbor_fp);
                        fq = 0.5*(node_fq + neighbor_fq);
                        lambda = 0.5*(node_lambda + neighbor_lambda);

                        g_fp = 0.5*(node_gfp + neighbor_gfp);
                        g_fq = 0.5*(node_gfq + neighbor_gfq);
                        g_lambda = 0.5*(node_glambda + neighbor_glambda);
                                            
                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);
                        neighborP->setNodeLambda(node_lambda);

                        neighborP->setNeighborActiveFlow(neighbor_fp);
                        neighborP->setNeighborReactiveFlow(neighbor_fq);
                        neighborP->setNeighborLambda(neighbor_lambda);
                                            
                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);
                        neighborP->setNodeLambdaGradient(node_glambda);

                        neighborP->setNeighborActiveFlowGradient(neighbor_gfp);
                        neighborP->setNeighborReactiveFlowGradient(neighbor_gfq);
                        neighborP->setNeighborLambdaGradient(neighbor_glambda);
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;
                        g_lambda = node_glambda;

                        fp = node_fp;
                        fq = node_fq;
                        lambda = node_lambda;
                    }
                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
                    neighborP->setLambdaGradient(g_lambda);

                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
                    neighborP->setLambda(lambda);
           //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq<<endl;
                    // delay(5);
           //          Serial << "lambda ("<<neighborID<<","<<nodeID<<") = "<<lambda<<endl;
                    // delay(5);
                }
                else if(neighborID > nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getNodeActiveFlow());
                    node_fq = (neighborP->getNodeReactiveFlow());
                    node_lambda = (neighborP->getNodeLambda());
                    node_gfp = (neighborP->getNodeActiveFlowGradient());
                    node_gfq = (neighborP->getNodeReactiveFlowGradient());
                    node_glambda = (neighborP->getNodeLambdaGradient());

                    // Serial<<"Received from Child Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
                    // delay(5);

                    if (neighbor_flag != neighbor_status)
                    {
                        neighbornode_fp = _getActiveFlowFromPacket_nodeACC();                                       // store incoming value of fp from child
                        neighbornode_fq = _getReactiveFlowFromPacket_nodeACC();                                     // store incoming value of fq from child
                        neighbornode_lambda = _getLambdaFromPacket_nodeACC();                                       // store incoming value of lambda from child
                        neighbornode_gfp = _getActiveFlowGradientFromPacket_node();                                     // store incoming value of fp from child
                        neighbornode_gfq = _getReactiveFlowGradientFromPacket_node();                                   // store incoming value of fq from child
                        neighbornode_glambda = _getLambdaGradientFromPacket_node();                                     // store incoming value of lambda from child

                        // Serial<<"Received self data from Child Node "<<neighborID<<":"<<endl<<node_fp<<" , "<<node_fq<<" , "<<node_lambda<<" , "<<node_gfp<<" , "<<node_gfq<<" , "<<node_glambda<<" , "<<neighbor_status<<endl;
                        // delay(5);

                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                                                  //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);                                        //update neighbor's status since neighbor's averaging step was successful
                        
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(neighbornode_fp+neighbor_fp);
                        fq = 0.5*(neighbornode_fq+neighbor_fq);
                        lambda = 0.5*(neighbornode_lambda +neighbor_lambda);

                        g_fp = 0.5*(neighbornode_gfp + neighbor_gfp);
                        g_fq = 0.5*(neighbornode_gfq + neighbor_gfq);
                        g_lambda = 0.5*(neighbornode_glambda + neighbor_glambda);

                        delta_fp = node_fp - neighbornode_fp;
                        delta_fq = node_fq - neighbornode_fq;
                        delta_lambda = node_lambda - neighbornode_lambda;

                        delta_gfp = node_gfp - neighbornode_gfp;
                        delta_gfq = node_gfq - neighbornode_gfq;
                        delta_glambda = node_glambda - neighbornode_glambda;
                        
                        fp = fp + delta_fp;
                        fq = fq + delta_fq;
                        lambda = lambda + delta_lambda;

                        g_fp = g_fp + delta_gfp;
                        g_fq = g_fq + delta_gfq;
                        g_lambda = g_lambda + delta_glambda;
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;
                        g_lambda = node_glambda;

                        fp = node_fp;
                        fq = node_fq;
                        lambda = node_lambda;
                    }
                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
                    neighborP->setLambda(lambda);

                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
                    neighborP->setLambdaGradient(g_lambda);
           //          Serial << "fp ("<<nodeID<<","<<neighborID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<nodeID<<","<<neighborID<<") = "<<fq<<endl;
                    // delay(5);
           //          Serial << "lambda ("<<nodeID<<","<<neighborID<<") = "<<lambda<<endl;
                    // delay(5);
                }
            }

            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                neighborID = l->unlinkLinkedListNodes();
                //Serial<<"Neighbor id is "<<neighborID<<endl;
                while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
                {
                    neighborP = (n+(neighborID-1));
                    if(neighborID < nodeID)
                    {
                        _sendToParent(neighborID,neighborP->getNodeActiveFlow(),neighborP->getNodeReactiveFlow(),neighborP->getNodeLambda(),neighborP->getNodeActiveFlowGradient(),neighborP->getNodeReactiveFlowGradient(),neighborP->getNodeLambdaGradient(),neighborP->getNodeFlag(),neighborP->getNeighborActiveFlow(),neighborP->getNeighborReactiveFlow(),neighborP->getNeighborLambda(),neighborP->getNeighborActiveFlowGradient(),neighborP->getNeighborReactiveFlowGradient(),neighborP->getNeighborLambdaGradient());
                        // Serial<<"Sent to Parent Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<neighborP->getNodeLambda()<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<neighborP->getNodeLambdaGradient()<<" , "<<neighborP->getNodeFlag()<<" , "<<neighborP->getNeighborActiveFlow()<<" , "<<neighborP->getNeighborReactiveFlow()<<" , "<<neighborP->getNeighborLambda()<<" , "<<neighborP->getNeighborActiveFlowGradient()<<" , "<<neighborP->getNeighborReactiveFlowGradient()<<" , "<<neighborP->getNeighborLambdaGradient()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    else if(neighborID > nodeID)
                    {
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                        node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());
                        node_lambda = (neighborP->getLambda()) + (neighborP->getLambdaGradient());

                        node_gfp = 2*alpha*(Mu + ((s->getWp())*bP) + ((neighborP->getResistance())*(neighborP->getLambda()))) ;
                        node_gfq =  2*alpha*(Nu + ((s->getWq())*bQ) + ((neighborP->getReactance())*(neighborP->getLambda()))) ;
                        node_glambda = 2*alpha*(sqV - ((neighborP->getActiveFlow())*(neighborP->getResistance())) - ((neighborP->getReactiveFlow())*(neighborP->getReactance())) );

                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);
                        neighborP->setNodeLambdaGradient(node_glambda);

                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);
                        neighborP->setNodeLambda(node_lambda);

                        _sendToChild(neighborID,node_fp,node_fq,node_lambda,node_gfp,node_gfq,node_glambda,neighborP->getNodeFlag());
                        // Serial<<"Sent to Child Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<neighborP->getNodeLambda()<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<neighborP->getNodeLambdaGradient()<<" , "<<neighborP->getNodeFlag()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    neighborID = l->unlinkLinkedListNodes();
                    neighborP = (n+(neighborID-1));
                }
                //l->updateLinkedList(s->getStatusP());
            }
        }
        l->updateLinkedList(s->getStatusP());
        l->updateActiveLinks(n);
        l->resetLinkedListStatus(s->getStatusP());

        neighborID = l->findInActiveLink(n);                                                                                            //find neighbors that no packet was received from
        neighborP = (n+(neighborID-1));
        
        while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
        {
            // Serial<<neighborID<<" inactive"<<endl;
            // delay(5);

            packetDropCount++;

            if(neighborID < nodeID)
            {
                node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());
                node_lambda = (neighborP->getLambda()) + (neighborP->getLambdaGradient());

                node_gfp = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) + ((neighborP->getResistance())*(neighborP->getLambda()))) ;
                node_gfq =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) + ((neighborP->getReactance())*(neighborP->getLambda()))) ;
                node_glambda= 2*alpha*(sqV*(-1) - ((neighborP->getActiveFlow())*(neighborP->getResistance())) - ((neighborP->getReactiveFlow())*(neighborP->getReactance())) );

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
                neighborP->setLambda(node_lambda);

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
                neighborP->setLambdaGradient(node_gfp);
            }
            else if(neighborID > nodeID)
            {
                node_gfp = neighborP->getNodeActiveFlowGradient();
                node_gfq = neighborP->getNodeReactiveFlowGradient();
                node_glambda = neighborP->getNodeLambdaGradient();

                //get values for fp, fq, and lambda that are currently associated with this neighbor
                node_fp = (neighborP->getNodeActiveFlow());
                node_fq = (neighborP->getNodeReactiveFlow());
                node_lambda = (neighborP->getNodeLambda());

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
                neighborP->setLambdaGradient(node_gfp);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
                neighborP->setLambda(node_lambda);
            }
   //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp + g_node_fp<<endl;
            // delay(5);
   //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq + g_node_fq<<endl;
            // delay(5);
   //          Serial << "lambda ("<<neighborID<<","<<nodeID<<") = "<<lambda + g_node_lambda<<endl;
            // delay(5);

            neighborID = l->findInActiveLink(n);
            neighborP = (n+(neighborID-1));
        }
        l->resetActiveLinks(n);
        l->updateLinkedList(s->getStatusP());
        bP = P - Pd - l->addActiveFlows(nodeID,n);
        bQ = Q - Qd - l->addReactiveFlows(nodeID,n);
      //   if (packetDropCount+packetReceiveCount > (_G->getN()-1))
      //   {
      //    Serial<<"ERROR! "<<packetDropCount<<" packet drops encountered"<<endl;
            // delay(5);
            // Serial<<"ERROR! "<<packetReceiveCount<<" packets received"<<endl;
            // delay(5);
      //   }
        packetsLost += packetDropCount;
        packetReceived += packetReceiveCount;

        packetDropCount = 0;
        packetReceiveCount = 0;

        Serial<<_FLOAT(P,6)<<","<<_FLOAT(Q,6)<<","<<_FLOAT(bP,6)<<","<<_FLOAT(bQ,6)<<","<<_FLOAT(sqV,6)<<","<<_FLOAT(Mu,6)<<","<<_FLOAT(Nu,6)<<endl;//<<"Active injection: "<<P<<endl<<"Reactive injection: "<<Q<<endl;
        delay(5);
    }

    s->setActiveSetpoint(P);
    s->setReactiveSetpoint(Q);
    s->setActiveBalance(bP);
    s->setReactiveBalance(bQ);
    s->setSquareVoltage(sqV);
    s->setMu(Mu);
    s->setNu(Nu);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}


bool OAgent::acceleratedPrimalDualAlgorithm1(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex
    iterations = 500;
    alpha = 0.1;
    uint16_t nodeID = s->getID();
    uint8_t neighborID;

    float fp;                                                                                   // node state variable active flow
    float fq;                                                                                   // node state variable reactive flow
    float lambda;                                                                               // node state variable lagrange multiplier
    float g_fp;
    float g_fq;
    float g_lambda;

    float delta_fp;                                                                             // change in node state variable active flow
    float delta_fq;                                                                             // change in node state variable reactive flow
    float delta_lambda;                                                                         // change in node state variable lagrange multiplier
    float delta_gfp;                                                                             // change in node state variable active flow
    float delta_gfq;                                                                             // change in node state variable reactive flow
    float delta_glambda;                                                                         // change in node state variable lagrange multiplier

    float g_fpTMP;
    float g_fqTMP;
    float g_lambdaTMP;

    float node_fp;                                                                          // neighbor state variable active flow
    float node_fq;                                                                          // neighbor state variable reactive flow
    float node_lambda;  
    float node_gfp;                                                                          // neighbor state variable active flow
    float node_gfq;                                                                          // neighbor state variable reactive flow
    float node_glambda;

    float neighbor_fp;                                                                          // neighbor state variable active flow
    float neighbor_fq;                                                                          // neighbor state variable reactive flow
    float neighbor_lambda;  
    float neighbor_gfp;                                                                          // neighbor state variable active flow
    float neighbor_gfq;                                                                          // neighbor state variable reactive flow
    float neighbor_glambda;  
    
    float neighbornode_fp;
    float neighbornode_fq;
    float neighbornode_lambda;
    
    float neighbornode_gfp;
    float neighbornode_gfq;
    float neighbornode_glambda;

    bool node_flag;
    bool neighbor_flag;
    bool neighbor_status;
    
    float P = (float)genBus*(s->getActiveSetpoint());                                                  // active injection
    float Q = (float)genBus*(s->getReactiveSetpoint());                                                // reactive injection
    float Pd = s->getActiveDemand();                                                            // active demand
    float Qd = s->getReactiveDemand();                                                          // reactive demand
    float sqV = s->getSquareVoltage();                                                          // square voltage magnitude
    float Mu = s->getMu();                                                                      // lagrange multiplier
    float Nu = s->getNu();                                                                      // lagrange multiplier
    float bP = P - Pd - l->addActiveFlows(nodeID,n);                                                  // active balance
    float bQ = Q - Qd - l->addReactiveFlows(nodeID,n);                                                 // reactive balance
    
    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts
    bool rxDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 200;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 20;
    
    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer
        // Serial<<"Iteration "<<k+1<<endl;
        P = P - (float)genBus*alpha*( Mu+((s->getDp())*P)+((s->getWp())*bP) );
        Q = Q - (float)genBus*alpha*( Nu+((s->getDq())*Q)+((s->getWq())*bQ) );
        sqV = sqV - alpha*( l->addLambdas(nodeID,n)+((s->getWv())*(sqV-1)) );

        if (sqV < 0.8)
            sqV = 0.8;
        else if (sqV > 1.2)
            sqV = 1.2;

        Mu = Mu + alpha*bP;
        Nu = Nu + alpha*bQ;
        
        txDone = false;
        rxDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForUnicastPacket(neighborID,nodeID,PD_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get values for fp, fq, and lambda that are received from this neighbor
                neighbor_fp = _getActiveFlowFromPacket_neighbor();                                      // store incoming value of fp
                neighbor_fq = _getReactiveFlowFromPacket_neighbor();                                    // store incoming value of fq
                neighbor_lambda = _getLambdaFromPacket_neighbor();                                      // store incoming value of lambda
                neighbor_gfp = _getActiveFlowGradientFromPacket_neighbor();                                     // store incoming value of fp
                neighbor_gfq = _getReactiveFlowGradientFromPacket_neighbor();                                   // store incoming value of fq
                neighbor_glambda = _getLambdaGradientFromPacket_neighbor();                                     // store incoming value of lambda

                neighbor_status = _getFlagFromPacket_ACC();                                                 // store incoming value of lambda

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                node_flag = neighborP->getNodeFlag();
                neighbor_flag = neighborP->getNeighborFlag();
                
                if(neighborID < nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                    node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());
                    node_lambda = (neighborP->getLambda()) + (neighborP->getLambdaGradient());

                    g_fpTMP = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) + ((neighborP->getResistance())*(neighborP->getLambda()))) ;
                    g_fqTMP =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) + ((neighborP->getReactance())*(neighborP->getLambda()))) ;
                    g_lambdaTMP= 2*alpha*(sqV*(-1) - ((neighborP->getActiveFlow())*(neighborP->getResistance())) - ((neighborP->getReactiveFlow())*(neighborP->getReactance())) );

                    node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                    node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );
                    node_glambda = neighborP->getLambdaGradient() + ( g_lambdaTMP - neighborP->getLambdaGradientTMP() );

                    neighborP->setActiveFlowGradientTMP(g_fpTMP);
                    neighborP->setReactiveFlowGradientTMP(g_fqTMP);
                    neighborP->setLambdaGradientTMP(g_lambdaTMP);

                    // Serial<<"Received from Parent Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
              //    delay(5);

                    if (neighbor_flag != neighbor_status)
                    {  
                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                    //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);
                                    
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(node_fp + neighbor_fp);
                        fq = 0.5*(node_fq + neighbor_fq);
                        lambda = 0.5*(node_lambda + neighbor_lambda);

                        g_fp = 0.5*(node_gfp + neighbor_gfp);
                        g_fq = 0.5*(node_gfq + neighbor_gfq);
                        g_lambda = 0.5*(node_glambda + neighbor_glambda);
                                            
                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);
                        neighborP->setNodeLambda(node_lambda);

                        neighborP->setNeighborActiveFlow(neighbor_fp);
                        neighborP->setNeighborReactiveFlow(neighbor_fq);
                        neighborP->setNeighborLambda(neighbor_lambda);
                                            
                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);
                        neighborP->setNodeLambdaGradient(node_glambda);

                        neighborP->setNeighborActiveFlowGradient(neighbor_gfp);
                        neighborP->setNeighborReactiveFlowGradient(neighbor_gfq);
                        neighborP->setNeighborLambdaGradient(neighbor_glambda);
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;
                        g_lambda = node_glambda;

                        fp = node_fp;
                        fq = node_fq;
                        lambda = node_lambda;
                    }
                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
                    neighborP->setLambdaGradient(g_lambda);

                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
                    neighborP->setLambda(lambda);
           //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq<<endl;
                    // delay(5);
           //          Serial << "lambda ("<<neighborID<<","<<nodeID<<") = "<<lambda<<endl;
                    // delay(5);
                }
                else if(neighborID > nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getNodeActiveFlow());
                    node_fq = (neighborP->getNodeReactiveFlow());
                    node_lambda = (neighborP->getNodeLambda());
                    node_gfp = (neighborP->getNodeActiveFlowGradient());
                    node_gfq = (neighborP->getNodeReactiveFlowGradient());
                    node_glambda = (neighborP->getNodeLambdaGradient());

                    // Serial<<"Received from Child Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
                    // delay(5);

                    if (neighbor_flag != neighbor_status)
                    {
                        neighbornode_fp = _getActiveFlowFromPacket_nodeACC();                                       // store incoming value of fp from child
                        neighbornode_fq = _getReactiveFlowFromPacket_nodeACC();                                     // store incoming value of fq from child
                        neighbornode_lambda = _getLambdaFromPacket_nodeACC();                                       // store incoming value of lambda from child
                        neighbornode_gfp = _getActiveFlowGradientFromPacket_node();                                     // store incoming value of fp from child
                        neighbornode_gfq = _getReactiveFlowGradientFromPacket_node();                                   // store incoming value of fq from child
                        neighbornode_glambda = _getLambdaGradientFromPacket_node();                                     // store incoming value of lambda from child

                        // Serial<<"Received self data from Child Node "<<neighborID<<":"<<endl<<node_fp<<" , "<<node_fq<<" , "<<node_lambda<<" , "<<node_gfp<<" , "<<node_gfq<<" , "<<node_glambda<<" , "<<neighbor_status<<endl;
                        // delay(5);

                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                                                  //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);                                        //update neighbor's status since neighbor's averaging step was successful
                        
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(neighbornode_fp+neighbor_fp);
                        fq = 0.5*(neighbornode_fq+neighbor_fq);
                        lambda = 0.5*(neighbornode_lambda +neighbor_lambda);

                        g_fp = 0.5*(neighbornode_gfp + neighbor_gfp);
                        g_fq = 0.5*(neighbornode_gfq + neighbor_gfq);
                        g_lambda = 0.5*(neighbornode_glambda + neighbor_glambda);

                        delta_fp = node_fp - neighbornode_fp;
                        delta_fq = node_fq - neighbornode_fq;
                        delta_lambda = node_lambda - neighbornode_lambda;

                        delta_gfp = node_gfp - neighbornode_gfp;
                        delta_gfq = node_gfq - neighbornode_gfq;
                        delta_glambda = node_glambda - neighbornode_glambda;
                        
                        fp = fp + delta_fp;
                        fq = fq + delta_fq;
                        lambda = lambda + delta_lambda;

                        g_fp = g_fp + delta_gfp;
                        g_fq = g_fq + delta_gfq;
                        g_lambda = g_lambda + delta_glambda;
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;
                        g_lambda = node_glambda;

                        fp = node_fp;
                        fq = node_fq;
                        lambda = node_lambda;
                    }
                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
                    neighborP->setLambda(lambda);

                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
                    neighborP->setLambdaGradient(g_lambda);
           //          Serial << "fp ("<<nodeID<<","<<neighborID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<nodeID<<","<<neighborID<<") = "<<fq<<endl;
                    // delay(5);
           //          Serial << "lambda ("<<nodeID<<","<<neighborID<<") = "<<lambda<<endl;
                    // delay(5);
                }
            }

            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                neighborID = l->unlinkLinkedListNodes();
                //Serial<<"Neighbor id is "<<neighborID<<endl;
                while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
                {
                    neighborP = (n+(neighborID-1));
                    if(neighborID < nodeID)
                    {
                        _sendToParent(neighborID,neighborP->getNodeActiveFlow(),neighborP->getNodeReactiveFlow(),neighborP->getNodeLambda(),neighborP->getNodeActiveFlowGradient(),neighborP->getNodeReactiveFlowGradient(),neighborP->getNodeLambdaGradient(),neighborP->getNodeFlag(),neighborP->getNeighborActiveFlow(),neighborP->getNeighborReactiveFlow(),neighborP->getNeighborLambda(),neighborP->getNeighborActiveFlowGradient(),neighborP->getNeighborReactiveFlowGradient(),neighborP->getNeighborLambdaGradient());
                        // Serial<<"Sent to Parent Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<neighborP->getNodeLambda()<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<neighborP->getNodeLambdaGradient()<<" , "<<neighborP->getNodeFlag()<<" , "<<neighborP->getNeighborActiveFlow()<<" , "<<neighborP->getNeighborReactiveFlow()<<" , "<<neighborP->getNeighborLambda()<<" , "<<neighborP->getNeighborActiveFlowGradient()<<" , "<<neighborP->getNeighborReactiveFlowGradient()<<" , "<<neighborP->getNeighborLambdaGradient()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    else if(neighborID > nodeID)
                    {
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                        node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());
                        node_lambda = (neighborP->getLambda()) + (neighborP->getLambdaGradient());

                        g_fpTMP = 2*alpha*(Mu + ((s->getWp())*bP) + ((neighborP->getResistance())*(neighborP->getLambda()))) ;
                        g_fqTMP =  2*alpha*(Nu + ((s->getWq())*bQ) + ((neighborP->getReactance())*(neighborP->getLambda()))) ;
                        g_lambdaTMP = 2*alpha*(sqV - ((neighborP->getActiveFlow())*(neighborP->getResistance())) - ((neighborP->getReactiveFlow())*(neighborP->getReactance())) );

                        node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                        node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );
                        node_glambda = neighborP->getLambdaGradient() + ( g_lambdaTMP - neighborP->getLambdaGradientTMP() );

                        neighborP->setActiveFlowGradientTMP(g_fpTMP);
                        neighborP->setReactiveFlowGradientTMP(g_fqTMP);
                        neighborP->setLambdaGradientTMP(g_lambdaTMP);

                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);
                        neighborP->setNodeLambdaGradient(node_glambda);

                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);
                        neighborP->setNodeLambda(node_lambda);

                        _sendToChild(neighborID,node_fp,node_fq,node_lambda,node_gfp,node_gfq,node_glambda,neighborP->getNodeFlag());
                        // Serial<<"Sent to Child Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<neighborP->getNodeLambda()<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<neighborP->getNodeLambdaGradient()<<" , "<<neighborP->getNodeFlag()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    neighborID = l->unlinkLinkedListNodes();
                    neighborP = (n+(neighborID-1));
                }
                //l->updateLinkedList(s->getStatusP());
            }
        }
        l->updateLinkedList(s->getStatusP());
        l->updateActiveLinks(n);
        l->resetLinkedListStatus(s->getStatusP());

        neighborID = l->findInActiveLink(n);                                                                                            //find neighbors that no packet was received from
        neighborP = (n+(neighborID-1));
        
        while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
        {
            // Serial<<neighborID<<" inactive"<<endl;
            // delay(5);

            packetDropCount++;

            if(neighborID < nodeID)
            {
                g_fpTMP = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) + ((neighborP->getResistance())*(neighborP->getLambda()))) ;
                g_fqTMP =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) + ((neighborP->getReactance())*(neighborP->getLambda()))) ;
                g_lambdaTMP= 2*alpha*(sqV*(-1) - ((neighborP->getActiveFlow())*(neighborP->getResistance())) - ((neighborP->getReactiveFlow())*(neighborP->getReactance())) );

                node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());
                node_lambda = (neighborP->getLambda()) + (neighborP->getLambdaGradient());

                node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );
                node_glambda = neighborP->getLambdaGradient() + ( g_lambdaTMP - neighborP->getLambdaGradientTMP() );

                neighborP->setActiveFlowGradientTMP(g_fpTMP);
                neighborP->setReactiveFlowGradientTMP(g_fqTMP);
                neighborP->setLambdaGradientTMP(g_lambdaTMP);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
                neighborP->setLambda(node_lambda);

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
                neighborP->setLambdaGradient(node_glambda);
            }
            else if(neighborID > nodeID)
            {
                node_gfp = neighborP->getNodeActiveFlowGradient();
                node_gfq = neighborP->getNodeReactiveFlowGradient();
                node_glambda = neighborP->getNodeLambdaGradient();

                //get values for fp, fq, and lambda that are currently associated with this neighbor
                node_fp = (neighborP->getNodeActiveFlow());
                node_fq = (neighborP->getNodeReactiveFlow());
                node_lambda = (neighborP->getNodeLambda());

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
                neighborP->setLambdaGradient(node_glambda);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
                neighborP->setLambda(node_lambda);
            }
   //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp + g_node_fp<<endl;
            // delay(5);
   //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq + g_node_fq<<endl;
            // delay(5);
   //          Serial << "lambda ("<<neighborID<<","<<nodeID<<") = "<<lambda + g_node_lambda<<endl;
            // delay(5);

            neighborID = l->findInActiveLink(n);
            neighborP = (n+(neighborID-1));
        }
        l->resetActiveLinks(n);
        l->updateLinkedList(s->getStatusP());
        bP = P - Pd - l->addActiveFlows(nodeID,n);
        bQ = Q - Qd - l->addReactiveFlows(nodeID,n);
      //   if (packetDropCount+packetReceiveCount > (_G->getN()-1))
      //   {
      //    Serial<<"ERROR! "<<packetDropCount<<" packet drops encountered"<<endl;
            // delay(5);
            // Serial<<"ERROR! "<<packetReceiveCount<<" packets received"<<endl;
            // delay(5);
      //   }
        packetsLost += packetDropCount;
        packetReceived += packetReceiveCount;

        packetDropCount = 0;
        packetReceiveCount = 0;

        Serial<<_FLOAT(P,6)<<","<<_FLOAT(Q,6)<<","<<_FLOAT(bP,6)<<","<<_FLOAT(bQ,6)<<","<<_FLOAT(sqV,6)<<","<<_FLOAT(Mu,6)<<","<<_FLOAT(Nu,6)<<endl;//<<"Active injection: "<<P<<endl<<"Reactive injection: "<<Q<<endl;
        delay(5);
    }

    s->setActiveSetpoint(P);
    s->setReactiveSetpoint(Q);
    s->setActiveBalance(bP);
    s->setReactiveBalance(bQ);
    s->setSquareVoltage(sqV);
    s->setMu(Mu);
    s->setNu(Nu);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}

bool OAgent::standardPrimalDualAlgorithm2(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex
    iterations = 750;
    alpha = 0.1;
    uint16_t nodeID = s->getID();
    uint8_t neighborID;

    float fp;                                                                                   // node state variable active flow
    float fq;                                                                                   // node state variable reactive flow
    float g_fp;
    float g_fq;

    float delta_fp;                                                                             // change in node state variable active flow
    float delta_fq;                                                                             // change in node state variable reactive flow
    float delta_gfp;                                                                             // change in node state variable active flow
    float delta_gfq;                                                                             // change in node state variable reactive flow

    float node_fp;                                                                          // neighbor state variable active flow
    float node_fq;                                                                          // neighbor state variable reactive flow
    float node_gfp;                                                                          // neighbor state variable active flow
    float node_gfq;                                                                          // neighbor state variable reactive flow

    float neighbor_fp;                                                                          // neighbor state variable active flow
    float neighbor_fq;                                                                          // neighbor state variable reactive flow
    float neighbor_gfp;                                                                          // neighbor state variable active flow
    float neighbor_gfq;                                                                          // neighbor state variable reactive flow
    
    float neighbornode_fp;
    float neighbornode_fq;
    
    float neighbornode_gfp;
    float neighbornode_gfq;

    bool node_flag;
    bool neighbor_flag;
    bool neighbor_status;
    
    float P = (float)genBus*(s->getActiveSetpoint());                                                  // active injection
    float Q = (float)genBus*(s->getReactiveSetpoint());                                                // reactive injection
    float Pd = s->getActiveDemand();                                                            // active demand
    float Qd = s->getReactiveDemand();                                                          // reactive demand
    float Mu = s->getMu();                                                                      // lagrange multiplier
    float Nu = s->getNu();                                                                      // lagrange multiplier
    float bP = P - Pd - l->addActiveFlows(nodeID,n);                                                  // active balance
    float bQ = Q - Qd - l->addReactiveFlows(nodeID,n);                                                 // reactive balance
    
    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts
    bool rxDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 200;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 25;
    
    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer
        // Serial<<"Iteration "<<k+1<<endl;
        P = P - (float)genBus*alpha*( Mu+((s->getWp())*bP) );
        Q = Q - (float)genBus*alpha*( Nu+((s->getWq())*bQ) );
        Mu = Mu + alpha*bP;
        Nu = Nu + alpha*bQ;
        
        txDone = false;
        rxDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForUnicastPacket(neighborID,nodeID,PD_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get values for fp, fq, and lambda that are received from this neighbor
                neighbor_fp = _getActiveFlowFromPacket_neighbor();                                      // store incoming value of fp
                neighbor_fq = _getReactiveFlowFromPacket_neighbor();                                    // store incoming value of fq
                neighbor_gfp = _getActiveFlowGradientFromPacket_neighbor();                                     // store incoming value of fp
                neighbor_gfq = _getReactiveFlowGradientFromPacket_neighbor();                                   // store incoming value of fq

                neighbor_status = _getFlagFromPacket_ACC();                                                 // store incoming value of lambda

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                node_flag = neighborP->getNodeFlag();
                neighbor_flag = neighborP->getNeighborFlag();
                
                if(neighborID < nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                    node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                    node_gfp = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) - ((neighborP->getResistance())*node_fp)) ;
                    node_gfq =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) - ((neighborP->getReactance())*node_fq)) ;

                    // Serial<<"Received from Parent Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
              //    delay(5);

                    if (neighbor_flag != neighbor_status)
                    {  
                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                    //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);
                                    
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(node_fp + neighbor_fp);
                        fq = 0.5*(node_fq + neighbor_fq);

                        g_fp = 0.5*(node_gfp + neighbor_gfp);
                        g_fq = 0.5*(node_gfq + neighbor_gfq);
                                            
                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        neighborP->setNeighborActiveFlow(neighbor_fp);
                        neighborP->setNeighborReactiveFlow(neighbor_fq);
                                            
                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNeighborActiveFlowGradient(neighbor_gfp);
                        neighborP->setNeighborReactiveFlowGradient(neighbor_gfq);
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);

                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
           //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq<<endl;
                    // delay(5);
                }
                else if(neighborID > nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getNodeActiveFlow());
                    node_fq = (neighborP->getNodeReactiveFlow());
                    node_gfp = (neighborP->getNodeActiveFlowGradient());
                    node_gfq = (neighborP->getNodeReactiveFlowGradient());

                    // Serial<<"Received from Child Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<neighbor_lambda<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<neighbor_glambda<<" , "<<neighbor_status<<endl;
                    // delay(5);

                    if (neighbor_flag != neighbor_status)
                    {
                        neighbornode_fp = _getActiveFlowFromPacket_nodeACC();                                       // store incoming value of fp from child
                        neighbornode_fq = _getReactiveFlowFromPacket_nodeACC();                                     // store incoming value of fq from child
                        neighbornode_gfp = _getActiveFlowGradientFromPacket_node();                                     // store incoming value of fp from child
                        neighbornode_gfq = _getReactiveFlowGradientFromPacket_node();                                   // store incoming value of fq from child

                        // Serial<<"Received self data from Child Node "<<neighborID<<":"<<endl<<node_fp<<" , "<<node_fq<<" , "<<node_lambda<<" , "<<node_gfp<<" , "<<node_gfq<<" , "<<node_glambda<<" , "<<neighbor_status<<endl;
                        // delay(5);

                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                                                  //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);                                        //update neighbor's status since neighbor's averaging step was successful
                        
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(neighbornode_fp+neighbor_fp);
                        fq = 0.5*(neighbornode_fq+neighbor_fq);

                        g_fp = 0.5*(neighbornode_gfp + neighbor_gfp);
                        g_fq = 0.5*(neighbornode_gfq + neighbor_gfq);

                        delta_fp = node_fp - neighbornode_fp;
                        delta_fq = node_fq - neighbornode_fq;

                        delta_gfp = node_gfp - neighbornode_gfp;
                        delta_gfq = node_gfq - neighbornode_gfq;
                        
                        fp = fp + delta_fp;
                        fq = fq + delta_fq;

                        g_fp = g_fp + delta_gfp;
                        g_fq = g_fq + delta_gfq;
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);

                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
           //          Serial << "fp ("<<nodeID<<","<<neighborID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<nodeID<<","<<neighborID<<") = "<<fq<<endl;
                    // delay(5);
                }
            }

            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                neighborID = l->unlinkLinkedListNodes();
                //Serial<<"Neighbor id is "<<neighborID<<endl;
                while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
                {
                    neighborP = (n+(neighborID-1));
                    if(neighborID < nodeID)
                    {
                        _sendToParent(neighborID,neighborP->getNodeActiveFlow(),neighborP->getNodeReactiveFlow(),0,neighborP->getNodeActiveFlowGradient(),neighborP->getNodeReactiveFlowGradient(),0,neighborP->getNodeFlag(),neighborP->getNeighborActiveFlow(),neighborP->getNeighborReactiveFlow(),0,neighborP->getNeighborActiveFlowGradient(),neighborP->getNeighborReactiveFlowGradient(),0);
                        // Serial<<"Sent to Parent Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<" , "<<neighborP->getNeighborActiveFlow()<<" , "<<neighborP->getNeighborReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNeighborActiveFlowGradient()<<" , "<<neighborP->getNeighborReactiveFlowGradient()<<" , "<<0<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    else if(neighborID > nodeID)
                    {
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                        node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                        node_gfp = 2*alpha*(Mu + ((s->getWp())*bP) - ((neighborP->getResistance())*node_fp)) ;
                        node_gfq =  2*alpha*(Nu + ((s->getWq())*bQ) - ((neighborP->getReactance())*node_fq)) ;

                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        _sendToChild(neighborID,node_fp,node_fq,0,node_gfp,node_gfq,0,neighborP->getNodeFlag());
                        // Serial<<"Sent to Child Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    neighborID = l->unlinkLinkedListNodes();
                    neighborP = (n+(neighborID-1));
                }
                //l->updateLinkedList(s->getStatusP());
            }
        }
        l->updateLinkedList(s->getStatusP());
        l->updateActiveLinks(n);
        l->resetLinkedListStatus(s->getStatusP());

        neighborID = l->findInActiveLink(n);                                                                                            //find neighbors that no packet was received from
        neighborP = (n+(neighborID-1));
        
        while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
        {
            // Serial<<neighborID<<" inactive"<<endl;
            // delay(5);

            packetDropCount++;

            if(neighborID < nodeID)
            {
                node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                node_gfp = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) - ((neighborP->getResistance())*node_fp)) ;
                node_gfq =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) - ((neighborP->getReactance())*node_fq)) ;

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
            }
            else if(neighborID > nodeID)
            {
                node_gfp = neighborP->getNodeActiveFlowGradient();
                node_gfq = neighborP->getNodeReactiveFlowGradient();

                //get values for fp, fq, and lambda that are currently associated with this neighbor
                node_fp = (neighborP->getNodeActiveFlow());
                node_fq = (neighborP->getNodeReactiveFlow());

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
            }
   //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp + g_node_fp<<endl;
            // delay(5);
   //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq + g_node_fq<<endl;
            // delay(5);

            neighborID = l->findInActiveLink(n);
            neighborP = (n+(neighborID-1));
        }
        l->resetActiveLinks(n);
        l->updateLinkedList(s->getStatusP());
        bP = P - Pd - l->addActiveFlows(nodeID,n);
        bQ = Q - Qd - l->addReactiveFlows(nodeID,n);
      //   if (packetDropCount+packetReceiveCount > (_G->getN()-1))
      //   {
      //    Serial<<"ERROR! "<<packetDropCount<<" packet drops encountered"<<endl;
            // delay(5);
            // Serial<<"ERROR! "<<packetReceiveCount<<" packets received"<<endl;
            // delay(5);
      //   }
        packetsLost += packetDropCount;
        packetReceived += packetReceiveCount;

        packetDropCount = 0;
        packetReceiveCount = 0;

        Serial<<_FLOAT(P,6)<<","<<_FLOAT(Q,6)<<","<<_FLOAT(bP,6)<<","<<_FLOAT(bQ,6)<<","<<_FLOAT(Mu,6)<<","<<_FLOAT(Nu,6)<<endl;//<<"Active injection: "<<P<<endl<<"Reactive injection: "<<Q<<endl;
        delay(5);
    }

    s->setActiveSetpoint(P);
    s->setReactiveSetpoint(Q);
    s->setActiveBalance(bP);
    s->setReactiveBalance(bQ);
    s->setMu(Mu);
    s->setNu(Nu);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}

bool OAgent::acceleratedPrimalDualAlgorithm2(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex
    iterations = 500;
    alpha = 0.1;
    uint16_t nodeID = s->getID();
    uint8_t neighborID;

    float fp;                                                                                   // node state variable active flow
    float fq;                                                                                   // node state variable reactive flow
    float g_fp;
    float g_fq;

    float delta_fp;                                                                             // change in node state variable active flow
    float delta_fq;                                                                             // change in node state variable reactive flow
    float delta_gfp;                                                                             // change in node state variable active flow
    float delta_gfq;                                                                             // change in node state variable reactive flow

    float g_fpTMP;
    float g_fqTMP;

    float node_fp;                                                                          // neighbor state variable active flow
    float node_fq;                                                                          // neighbor state variable reactive flow
    float node_gfp;                                                                          // neighbor state variable active flow
    float node_gfq;                                                                          // neighbor state variable reactive flow

    float neighbor_fp;                                                                          // neighbor state variable active flow
    float neighbor_fq;                                                                          // neighbor state variable reactive flow
    float neighbor_gfp;                                                                          // neighbor state variable active flow
    float neighbor_gfq;                                                                          // neighbor state variable reactive flow
    
    float neighbornode_fp;
    float neighbornode_fq;
    
    float neighbornode_gfp;
    float neighbornode_gfq;

    bool node_flag;
    bool neighbor_flag;
    bool neighbor_status;
    
    float P = (float)genBus*(s->getActiveSetpoint());                                                  // active injection
    float Q = (float)genBus*(s->getReactiveSetpoint());                                                // reactive injection
    float Pd = s->getActiveDemand();                                                            // active demand
    float Qd = s->getReactiveDemand();                                                          // reactive demand
    float Mu = s->getMu();                                                                      // lagrange multiplier
    float Nu = s->getNu();                                                                      // lagrange multiplier
    float bP = P - Pd - l->addActiveFlows(nodeID,n);                                                  // active balance
    float bQ = Q - Qd - l->addReactiveFlows(nodeID,n);                                                 // reactive balance
    
    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts
    bool rxDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 200;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 20;
    
    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer
        // Serial<<"Iteration "<<k+1<<endl;
        P = P - (float)genBus*alpha*( Mu+((s->getDp())*P)+((s->getWp())*bP) );
        Q = Q - (float)genBus*alpha*( Nu+((s->getDq())*Q)+((s->getWq())*bQ) );
        Mu = Mu + alpha*bP;
        Nu = Nu + alpha*bQ;
        
        txDone = false;
        rxDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForUnicastPacket(neighborID,nodeID,PD_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get values for fp, fq that are received from this neighbor
                neighbor_fp = _getActiveFlowFromPacket_neighbor();                                      // store incoming value of fp
                neighbor_fq = _getReactiveFlowFromPacket_neighbor();                                    // store incoming value of fq
                neighbor_gfp = _getActiveFlowGradientFromPacket_neighbor();                                     // store incoming value of fp
                neighbor_gfq = _getReactiveFlowGradientFromPacket_neighbor();                                   // store incoming value of fq

                neighbor_status = _getFlagFromPacket_ACC();                                                 // store incoming value of lambda

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                node_flag = neighborP->getNodeFlag();
                neighbor_flag = neighborP->getNeighborFlag();
                
                if(neighborID < nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                    node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                    g_fpTMP = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) - ((neighborP->getResistance())*node_fp)) ;
                    g_fqTMP =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) - ((neighborP->getReactance())*node_fq)) ;

                    node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                    node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );

                    neighborP->setActiveFlowGradientTMP(g_fpTMP);
                    neighborP->setReactiveFlowGradientTMP(g_fqTMP);

                    // Serial<<"Received from Parent Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<0<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<0<<" , "<<neighbor_status<<endl;
              //    delay(5);

                    if (neighbor_flag != neighbor_status)
                    {  
                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                    //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);
                                    
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(node_fp + neighbor_fp);
                        fq = 0.5*(node_fq + neighbor_fq);

                        g_fp = 0.5*(node_gfp + neighbor_gfp);
                        g_fq = 0.5*(node_gfq + neighbor_gfq);
                                            
                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        neighborP->setNeighborActiveFlow(neighbor_fp);
                        neighborP->setNeighborReactiveFlow(neighbor_fq);
                                            
                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNeighborActiveFlowGradient(neighbor_gfp);
                        neighborP->setNeighborReactiveFlowGradient(neighbor_gfq);
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);

                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);
           //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq<<endl;
                    // delay(5);
                }
                else if(neighborID > nodeID)
                {
                    //get values for fp, fq, and lambda that are currently associated with this neighbor
                    node_fp = (neighborP->getNodeActiveFlow());
                    node_fq = (neighborP->getNodeReactiveFlow());
                    node_gfp = (neighborP->getNodeActiveFlowGradient());
                    node_gfq = (neighborP->getNodeReactiveFlowGradient());

                    // Serial<<"Received from Child Node "<<neighborID<<":"<<endl<<neighbor_fp<<" , "<<neighbor_fq<<" , "<<0<<" , "<<neighbor_gfp<<" , "<<neighbor_gfq<<" , "<<0<<" , "<<neighbor_status<<endl;
                    // delay(5);

                    if (neighbor_flag != neighbor_status)
                    {
                        neighbornode_fp = _getActiveFlowFromPacket_nodeACC();                                       // store incoming value of fp from child
                        neighbornode_fq = _getReactiveFlowFromPacket_nodeACC();                                     // store incoming value of fq from child
                        neighbornode_gfp = _getActiveFlowGradientFromPacket_node();                                     // store incoming value of fp from child
                        neighbornode_gfq = _getReactiveFlowGradientFromPacket_node();                                   // store incoming value of fq from child

                        // Serial<<"Received self data from Child Node "<<neighborID<<":"<<endl<<node_fp<<" , "<<node_fq<<" , "<<0<<" , "<<node_gfp<<" , "<<node_gfq<<" , "<<0<<" , "<<neighbor_status<<endl;
                        // delay(5);

                        node_flag = !node_flag;
                        neighborP->setNodeFlag(node_flag);                                                  //invert node's flag value since averaging has been successfully performed
                        neighborP->setNeighborFlag(neighbor_status);                                        //update neighbor's status since neighbor's averaging step was successful
                        
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        fp = 0.5*(neighbornode_fp+neighbor_fp);
                        fq = 0.5*(neighbornode_fq+neighbor_fq);

                        g_fp = 0.5*(neighbornode_gfp + neighbor_gfp);
                        g_fq = 0.5*(neighbornode_gfq + neighbor_gfq);

                        delta_fp = node_fp - neighbornode_fp;
                        delta_fq = node_fq - neighbornode_fq;

                        delta_gfp = node_gfp - neighbornode_gfp;
                        delta_gfq = node_gfq - neighbornode_gfq;
                        
                        fp = fp + delta_fp;
                        fq = fq + delta_fq;

                        g_fp = g_fp + delta_gfp;
                        g_fq = g_fq + delta_gfq;
                    }
                    else
                    {
                        g_fp = node_gfp;
                        g_fq = node_gfq;

                        fp = node_fp;
                        fq = node_fq;
                    }
                    neighborP->setActiveFlow(fp);
                    neighborP->setReactiveFlow(fq);

                    neighborP->setActiveFlowGradient(g_fp);
                    neighborP->setReactiveFlowGradient(g_fq);
           //          Serial << "fp ("<<nodeID<<","<<neighborID<<") = "<<fp<<endl;
                    // delay(5);
           //          Serial << "fq ("<<nodeID<<","<<neighborID<<") = "<<fq<<endl;
                    // delay(5);
                }
            }

            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                neighborID = l->unlinkLinkedListNodes();
                //Serial<<"Neighbor id is "<<neighborID<<endl;
                while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
                {
                    neighborP = (n+(neighborID-1));
                    if(neighborID < nodeID)
                    {
                        _sendToParent(neighborID,neighborP->getNodeActiveFlow(),neighborP->getNodeReactiveFlow(),0,neighborP->getNodeActiveFlowGradient(),neighborP->getNodeReactiveFlowGradient(),0,neighborP->getNodeFlag(),neighborP->getNeighborActiveFlow(),neighborP->getNeighborReactiveFlow(),0,neighborP->getNeighborActiveFlowGradient(),neighborP->getNeighborReactiveFlowGradient(),0);
                        // Serial<<"Sent to Parent Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<" , "<<neighborP->getNeighborActiveFlow()<<" , "<<neighborP->getNeighborReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNeighborActiveFlowGradient()<<" , "<<neighborP->getNeighborReactiveFlowGradient()<<" , "<<0<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    else if(neighborID > nodeID)
                    {
                        //get values for fp, fq, and lambda that are currently associated with this neighbor
                        node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                        node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                        g_fpTMP = 2*alpha*(Mu + ((s->getWp())*bP) - ((neighborP->getResistance())*node_fp)) ;
                        g_fqTMP =  2*alpha*(Nu + ((s->getWq())*bQ) - ((neighborP->getReactance())*node_fq)) ;

                        node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                        node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );

                        neighborP->setActiveFlowGradientTMP(g_fpTMP);
                        neighborP->setReactiveFlowGradientTMP(g_fqTMP);

                        neighborP->setNodeActiveFlowGradient(node_gfp);
                        neighborP->setNodeReactiveFlowGradient(node_gfq);

                        neighborP->setNodeActiveFlow(node_fp);
                        neighborP->setNodeReactiveFlow(node_fq);

                        _sendToChild(neighborID,node_fp,node_fq,0,node_gfp,node_gfq,0,neighborP->getNodeFlag());
                        // Serial<<"Sent to Child Node "<<neighborID<<": "<<neighborP->getNodeActiveFlow()<<" , "<<neighborP->getNodeReactiveFlow()<<" , "<<0<<" , "<<neighborP->getNodeActiveFlowGradient()<<" , "<<neighborP->getNodeReactiveFlowGradient()<<" , "<<0<<" , "<<neighborP->getNodeFlag()<<endl;
      //                delay(5);
                        Serial<<_FLOAT(neighborP->getActiveFlow(),6)<<","<<_FLOAT(neighborP->getReactiveFlow(),6)<<", ,";
                    }
                    neighborID = l->unlinkLinkedListNodes();
                    neighborP = (n+(neighborID-1));
                }
                //l->updateLinkedList(s->getStatusP());
            }
        }
        l->updateLinkedList(s->getStatusP());
        l->updateActiveLinks(n);
        l->resetLinkedListStatus(s->getStatusP());

        neighborID = l->findInActiveLink(n);                                                                                            //find neighbors that no packet was received from
        neighborP = (n+(neighborID-1));
        
        while(neighborID != 0)                                                                                                          //while neighbors have not been sent a packet
        {
            // Serial<<neighborID<<" inactive"<<endl;
            // delay(5);

            packetDropCount++;

            if(neighborID < nodeID)
            {
                node_fp = (neighborP->getActiveFlow()) + (neighborP->getActiveFlowGradient());
                node_fq = (neighborP->getReactiveFlow()) + (neighborP->getReactiveFlowGradient());

                g_fpTMP = 2*alpha*(Mu*(-1) + ((s->getWp())*bP)*(-1) - ((neighborP->getResistance())*node_fp)) ;
                g_fqTMP =  2*alpha*(Nu*(-1) + ((s->getWq())*bQ)*(-1) - ((neighborP->getReactance())*node_fq)) ;

                node_gfp = neighborP->getActiveFlowGradient() + ( g_fpTMP - neighborP->getActiveFlowGradientTMP() );
                node_gfq = neighborP->getReactiveFlowGradient() + ( g_fqTMP - neighborP->getReactiveFlowGradientTMP() );

                neighborP->setActiveFlowGradientTMP(g_fpTMP);
                neighborP->setReactiveFlowGradientTMP(g_fqTMP);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);
            }
            else if(neighborID > nodeID)
            {
                node_gfp = neighborP->getNodeActiveFlowGradient();
                node_gfq = neighborP->getNodeReactiveFlowGradient();

                //get values for fp, fq, and lambda that are currently associated with this neighbor
                node_fp = (neighborP->getNodeActiveFlow());
                node_fq = (neighborP->getNodeReactiveFlow());

                neighborP->setActiveFlowGradient(node_gfp);
                neighborP->setReactiveFlowGradient(node_gfq);

                neighborP->setActiveFlow(node_fp);
                neighborP->setReactiveFlow(node_fq);
            }
   //          Serial << "fp ("<<neighborID<<","<<nodeID<<") = "<<fp + g_node_fp<<endl;
            // delay(5);
   //          Serial << "fq ("<<neighborID<<","<<nodeID<<") = "<<fq + g_node_fq<<endl;
            // delay(5);

            neighborID = l->findInActiveLink(n);
            neighborP = (n+(neighborID-1));
        }
        l->resetActiveLinks(n);
        l->updateLinkedList(s->getStatusP());
        bP = P - Pd - l->addActiveFlows(nodeID,n);
        bQ = Q - Qd - l->addReactiveFlows(nodeID,n);
      //   if (packetDropCount+packetReceiveCount > (_G->getN()-1))
      //   {
      //    Serial<<"ERROR! "<<packetDropCount<<" packet drops encountered"<<endl;
            // delay(5);
            // Serial<<"ERROR! "<<packetReceiveCount<<" packets received"<<endl;
            // delay(5);
      //   }
        packetsLost += packetDropCount;
        packetReceived += packetReceiveCount;

        packetDropCount = 0;
        packetReceiveCount = 0;

        Serial<<_FLOAT(P,6)<<","<<_FLOAT(Q,6)<<","<<_FLOAT(bP,6)<<","<<_FLOAT(bQ,6)<<","<<_FLOAT(Mu,6)<<","<<_FLOAT(Nu,6)<<endl;//<<"Active injection: "<<P<<endl<<"Reactive injection: "<<Q<<endl;
        delay(5);
    }

    s->setActiveSetpoint(P);
    s->setReactiveSetpoint(Q);
    s->setActiveBalance(bP);
    s->setReactiveBalance(bQ);
    s->setMu(Mu);
    s->setNu(Nu);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}
// End Primal Dual methods



// Economic Dispatch methods

bool OAgent::economicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    srand(analogRead(7));
    bool gamma = false;

    if(isLeader())
    { 
        gamma = leaderEconomicDispatchAlgorithm(genBus,alpha,iterations);
    }
    else
    {
        gamma = nonleaderEconomicDispatchAlgorithm(genBus,alpha,iterations);
    }
        //Serial<<"Sup bro?! "<<getbufferdata(0)<<"\n";

    return gamma;

}

bool OAgent::leaderEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    unsigned long t0 = myMillis();
    unsigned long startTime = t0 + ED_DELAY;
    bool gamma = false;
    bool scheduled = _waitForChildSchedulePacketED(SCHEDULE_TIMEOUT,startTime,iterations);
    //Serial<<"Schedule done at "<<myMillis()<<"\n";
    //bool stat = startTime>myMillis();

    //Serial<<"Leader: Startime= "<<startTime<<", Time= "<<myMillis()<<"\n";

    if (!scheduled) 
    {
        Serial<<"ED scheduling was a FAIL"<<endl;
        gamma = false;
    }
    else
    {
        Serial<<"ED scheduling was a SUCCESS"<<endl;
        if(_waitToStart(startTime,true,10000))
        {
            Serial << "Correct Startime is " <<startTime<< ". My startime is "<< myMillis() <<endl;
            // gamma = standardEconomicDispatchAlgorithm(genBus,alpha,iterations);
            gamma = acceleratedEconomicDispatchAlgorithm(genBus,alpha,iterations);
        }
    }        
    return gamma;
}

bool OAgent::nonleaderEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    unsigned long startTime = 0;
    bool gamma = false;
    //delay(50);
    bool scheduled = _waitForParentSchedulePacketED(startTime,iterations,-1);
    //Serial<<"Schedule done at "<<myMillis()<<"\n";
    
    //bool stat = startTime>myMillis();
    //Serial<<"Startime > Time? "<<stat<<"\n";

    //Serial<<"NonLeader: Startime= "<<startTime<<", Time= "<<myMillis()<<"\n";

    if(scheduled)
    {
        Serial<<"ED scheduling was a SUCCESS"<<endl;
        if(_waitToStart(startTime,true,10000))
        {
            Serial << "Correct Startime is " <<startTime<< ". My startime is "<< myMillis() <<endl;
            // gamma = standardEconomicDispatchAlgorithm(genBus,alpha,iterations);
            gamma = acceleratedEconomicDispatchAlgorithm(genBus,alpha,iterations);
        }
        //digitalWrite(48,LOW);
    }
    else
    {
        Serial<<"ED scheduling was a FAIL"<<endl;
        gamma = false;
    }
    return gamma;
}


bool OAgent::standardEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex

    uint16_t nodeID = s->getID();
    ORemoteVertex * nodeP = (n+(nodeID-1));                                                     // pointer to this vertex

    iterations = 750;
    alpha = 0.1;

    uint8_t neighborID;

    float P = (float)genBus*(s->getActiveSetpoint());                                           // active injection
    float Pd = s->getActiveDemand();                                                            // active demand

    float nodeLambda = 0;                                                                       // node state variable lagrange multiplier
    float nodeNu = 1;
    float nodeGamma = 0;

    float lambda;                                                                       // node state variable lagrange multiplier
    float nu;
    
    float neighborLambda = 0;                                                                       // node state variable lagrange multiplier
    float neighborNu = 0;
    float neighborGamma = 0;

    float sumNeighborLambda = 0;                                                                       // node state variable lagrange multiplier
    float sumNeighborNu = 0;
    float sumNeighborGamma = 0;

    float Xi;

    float sumLambda;
    float sumNu;
    float sumGamma;

    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 200;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 25;

    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer

        nodeLambda = ( s->getLambda() - (alpha*(P - Pd)) )/(s->getOutDegree());
        nodeNu = s->getNu()/(s->getOutDegree());

        nodeP->setSumLambda(nodeP->getSumLambda() + nodeLambda);
        nodeP->setSumNu(nodeP->getSumNu() + nodeNu);

        // Serial<<"Iteration "<<k+1<<endl;
        
        txDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForNeighborPacket(neighborID,ED_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

       //           Serial<<neighborID<<" active"<<endl;
                // delay(5);

                //get running sum values for Lambda and NU that are received from this neighbor
                sumLambda =_getSumLambdaFromPacket();                                                   // store incoming value of lambda
                sumNu = _getSumNuFromPacket();                                                          // store incoming value of nu

                neighborP = (n+(neighborID-1));
                
                neighborLambda = sumLambda - neighborP->getSumLambda();
                neighborNu = sumNu - neighborP->getSumNu();

                sumNeighborLambda += neighborLambda;
                sumNeighborNu += neighborNu;

                neighborP->setSumLambda(sumLambda);
                neighborP->setSumNu(sumNu);
            }
            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                _sendToNeighbor(nodeP->getSumLambda(),nodeP->getSumNu());
                // Serial<<"Sent to Neighbors: "<<nodeP->getSumLambda()<<" , "<<nodeP->getSumNu()<<endl;
                // delay(5);
                //Serial<<_FLOAT(nodeP->getSumLambda(),6)<<","<<_FLOAT(nodeP->getSumNu()<<";";
            }
        }
        lambda = sumNeighborLambda + nodeLambda;
        nu = sumNeighborNu + nodeNu;
        P = P - (float)genBus*alpha*( (s->getDp())*P - lambda/nu);

        s->setLambda(lambda);
        s->setNu(nu);
        
        packetReceived += packetReceiveCount;
        packetsLost += (_G->getN() - packetReceiveCount - 1);

        packetReceiveCount = 0;         
        l->resetLinkedListStatus(s->getStatusP());
    }
    s->setActiveSetpoint(P);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}


bool OAgent::acceleratedEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations) {
    OLocalVertex * s = _G->getLocalVertex();                                                    // store pointer to local vertex
    ORemoteVertex * n = _G->getRemoteVertex(1);                                                 // store pointer to remote vertices
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());
    l->updateLinkedList(s->getStatusP());
    uint8_t * neighborStatusP = s->getStatusP();

    ORemoteVertex * neighborP;                                                                  // pointer to a remote vertex

    uint16_t nodeID = s->getID();
    ORemoteVertex * nodeP = (n+(nodeID-1));                                                     // pointer to this vertex

    iterations = 500;
    alpha = 0.1;

    uint8_t neighborID;

    float P = (float)genBus*(s->getActiveSetpoint());                                           // active injection
    float Pd = s->getActiveDemand();                                                            // active demand

    float nodeLambda = 0;                                                                       // node state variable lagrange multiplier
    float nodeNu = 1;
    float nodeGamma = 0;

    float lambda;                                                                       // node state variable lagrange multiplier
    float nu;
    float gamma;
    float gammaTMP;
    
    float neighborLambda = 0;                                                                       // node state variable lagrange multiplier
    float neighborNu = 0;
    float neighborGamma = 0;

    float sumNeighborLambda;                                                                       // node state variable lagrange multiplier
    float sumNeighborNu;
    float sumNeighborGamma;

    float Xi;

    float sumLambda;
    float sumNu;
    float sumGamma;

    bool receivedPacket;

    bool txDone;                                // create variable to keep track of broadcasts

    int timeout = 100;                               // create variable to keep track of broadcasts

    uint16_t packetDropCount = 0;
    uint16_t packetReceiveCount = 0;
    uint16_t packetsLost = 0;
    uint16_t packetReceived = 0;
    //uint16_t period = _windowsPerPeriod*WINDOW_LENGTH;                            // create variable to keep track of broadcasts
    uint16_t period = 500;                                                                  // create variable to keep track of broadcasts
    uint8_t frame = 50;

    unsigned long start = (millis()-period);   // initialize timer                                                                  // create variable to store iteration start time

    srand(millis());    
    uint16_t txTime = (rand() % (period - 2*frame)) + frame;                                //determines the time window in which a payload is transmitted

    for(uint16_t k = 0; k < iterations; k++)
    {
        start = millis();   // initialize timer

        gammaTMP = P - Pd;

        nodeLambda = ( s->getLambda() - (alpha*(s->getGamma())) )/(s->getOutDegree());
        nodeNu = s->getNu()/(s->getOutDegree());
        nodeGamma = ( (s->getGamma()) + (gammaTMP - (s->getGammaTMP())) )/(s->getOutDegree());

        s->setGammaTMP(gammaTMP);

        nodeP->setSumLambda(nodeP->getSumLambda() + nodeLambda);
        nodeP->setSumNu(nodeP->getSumNu() + nodeNu);
        nodeP->setSumGamma(nodeP->getSumGamma() + nodeGamma);       

        sumNeighborLambda = 0;
        sumNeighborNu = 0;
        sumNeighborGamma = 0;

        // Serial<<"Iteration "<<k+1<<endl;
        
        txDone = false;

        while( uint16_t(millis()-start) < period )
        {
            receivedPacket = _waitForNeighborPacket(neighborID,ED_HEADER,true,10);
            if ( receivedPacket && (*(neighborStatusP+neighborID-1) == 2) )                                                                         //get values for fp, fq, and lambda that are received from this neighbor
            {
                packetReceiveCount++;

                *(neighborStatusP+neighborID-1) = 3;                                                            //set neighbor status to 3, i.e., note that node has received from this neighbor in this iteration

                //get running sum values for Lambda, Nu, and Gamma that are received from this neighbor
                sumLambda =_getSumLambdaFromPacket();                                                   // store incoming value of lambda
                sumNu = _getSumNuFromPacket();                                                          // store incoming value of nu
                sumGamma = _getSumGammaFromPacket();                                                    // store incoming value of gamma

                // Serial<<"Received from Node "<<neighborID<<": "<<sumLambda<<" , "<<sumNu<<" , "<<sumGamma<<endl;
                // delay(5);

                neighborP = (n+(neighborID-1));
                neighborP->setLinkStatus(true);

                neighborLambda = sumLambda - neighborP->getSumLambda();
                neighborNu = sumNu - neighborP->getSumNu();
                neighborGamma = sumGamma - neighborP->getSumGamma();

                sumNeighborLambda += neighborLambda;
                sumNeighborNu += neighborNu;
                sumNeighborGamma += neighborGamma;

                neighborP->setSumLambda(sumLambda);
                neighborP->setSumNu(sumNu);
                neighborP->setSumGamma(sumGamma);
            }
            if(!txDone && (uint16_t(millis()-start) >= txTime))
            //if(!txDone && (uint16_t(millis()-start) >= (period-frame)/2))
            {
                txDone = true;
                _sendToNeighbor_ACC(nodeP->getSumLambda(),nodeP->getSumNu(),nodeP->getSumGamma());
                // Serial<<"Sent to Neighbors: "<<nodeP->getSumLambda()<<" , "<<nodeP->getSumNu()<<" , "<<nodeP->getSumGamma()<<endl;
    //             delay(5);
            }
        }
        lambda = sumNeighborLambda + nodeLambda;
        nu = sumNeighborNu + nodeNu;
        gamma = sumNeighborGamma + nodeGamma;
        P = P - (float)genBus*alpha*( (s->getDp())*P - lambda/nu);

        // if(k%10 == 0)
            Serial<<_FLOAT(lambda,6)<<", "<<_FLOAT(nu,6)<<", "<<_FLOAT(gamma,6)<<", "<<_FLOAT(P,6)<<endl;

        s->setLambda(lambda);
        s->setNu(nu);
        s->setGamma(gamma);
        
        packetReceived += packetReceiveCount;
        packetsLost += (_G->getN() - packetReceiveCount - 1);

        packetReceiveCount = 0;

        l->resetLinkedListStatus(s->getStatusP());
    }
    s->setActiveSetpoint(P);
    
    Serial<<packetsLost<<" packets lost"<<endl;
    delay(5);
    Serial<<packetReceived<<" packets received"<<endl;
    delay(5);
    // Serial << "The active power injection at node" << nodeID << " is "<< P <<endl;
    // delay(5);
    // Serial << "The reactive power injection at node" << nodeID << " is "<< Q <<endl;
    // delay(5);
    return true;
}

// End Economic Dispatch Methods







/// Synchronization methods
bool OAgent::sync(uint8_t attempts) {
    
    if(_leader) {
        //Serial << "Is leader, and is in sync\n";
        for(uint8_t i = 0; i < attempts; i++) {
            if(_leaderSync()) {
                _synced = true;
                return true;
            }
            delay(SYNC_RETRY_PERIOD); //SYNC_RETRY_PERIOD = 250ms
        }    
    } else {
        //Serial << " in sync";
        unsigned long tTwo;
        int i = _waitForSyncBeginPacket(tTwo); //it will wait here until it receives a sync or resync packet 
        if(i == 1) {
            //Serial << "Sync Begin packet received \n";
            if(_isTargetNode())
            {
                //Serial << "Target Node \n";
                return _targetSync(tTwo);
            }
            else
                return _nonTargetSync(tTwo);
        }
        else if(i == 2)
        {
            uint8_t ptr = 2;
            long global_offset = _getLongFromPacket(ptr);   //offset between synced node and leader node
            unsigned long neighborID = _getUint32_tFromPacket(ptr); //node id of synced node
            unsigned long period = _getUint32_tFromPacket(ptr);

            unsigned long start = millis();

            while(millis() - start < period)
            {
                //JUST WAIT UNTIL PERIOD IS OVER
            }
            //Serial << "Recieved ID: ";
            //Serial << neighborID;
            //Serial << "\n";
            //Serial << "Response packet sent";
            //Serial << "\n";

            _broadcastResyncResponsePacket(tTwo,neighborID);

            if(_waitForResyncFinalPacket(RESYNC_TOTAL_TIME)) {
            // T = t + d + t2 - t2'
            //Serial << "Final Packet received";
            //Serial << "\n";
            uint8_t ptr = 2;
            long local_offset = _getLongFromPacket(ptr);
            _offset = global_offset + local_offset;
            //Serial<<"Offset= "<<getoffsetdata()<<"\n";
            //_offset = local_offset; //SN Debug to just see local offset
            _synced = true;
            return true;
            }
        }
    }
    return false;
}

//////////////////////////* RESYNC CODE*//////////////////////////////

bool OAgent::resync(){
    OLocalVertex * s = _G->getLocalVertex(); // store pointer to local vertex 
    unsigned long nodeID = s->getID();
    unsigned long start_time = millis();
    unsigned long t = millis() - start_time; 
    bool transmit = false;
    int txtime;
    unsigned long t4;
    unsigned long t1;

    txtime = rand()%RESYNC_BROADCAST_TIME;

    while(t < RESYNC_BROADCAST_TIME)
    {
        

        t = millis() - start_time;
        
        if(t >= txtime && transmit == false)
        {
            transmit = true;
            t1 = _broadcastResyncBeginPacket(RESYNC_BROADCAST_TIME - t);  //period to wait before responding, t1 is the time packet was sent
            //Serial << "Time broadcasted: ";
            //Serial << t;
            //Serial << "\n";
        }
    }

    if(_waitForResyncPacketResponse(t4, RESYNC_TOTAL_TIME - RESYNC_BROADCAST_TIME))   //t4 is the time the response packet was received, waiting timeout
    {   
        
        uint8_t ptr = 2;
        unsigned long t2 = _getUint32_tFromPacket(ptr);
        unsigned long received_id =  _getUint32_tFromPacket(ptr);  //ptr updates by 4 (4 bytes have been read) within _getUint32_tFromPacket()
        unsigned long t3 = _getUint32_tFromPacket(ptr);
        //Serial << "ID: ";
        //Serial << received_id;
        //Serial << "\n";
        if(nodeID == received_id)
        {
            //Serial << "Received Response & broadcasted final packet";
            //Serial << "\n";
            long local_offset = long(t2 + t3 - t1 - t4)/long(2);
            /*SN Debug
            Serial <<"t1 :"; 
            Serial << t1;
            Serial << "\n";
            Serial <<"t2 :"; 
            Serial << t2;
            Serial << "\n";
            Serial <<"t3 :"; 
            Serial << t3;
            Serial << "\n";
            Serial <<"t4 :"; 
            Serial << t4;
            Serial << "\n";
            
            //SN Debug */
            //Serial << "long Local offset \n";
            //Serial << local_offset;
            //Serial << "\n";
            _broadcastResyncFinalPacket(local_offset);
            return true;    
        }
        
    }

    return false;

}


unsigned long OAgent::_broadcastResyncBeginPacket(unsigned long period){

    uint16_t payload[7];
    long offset = _offset;
    OLocalVertex * s = _G->getLocalVertex(); // store pointer to local vertex 
    unsigned long id = s->getID();

    payload[0] = RESYNC_HEADER;
    _addUint32_tToPayload(uint32_t(offset),((uint8_t * )(&payload)),2);
    _addUint32_tToPayload(uint32_t(id),((uint8_t * )(&payload)),6);
    _addUint32_tToPayload(uint32_t(period),((uint8_t * )(&payload)),10);

    _zbTx = ZBTxRequest(_broadcastAddress,((uint8_t *)(&payload)),sizeof(payload));
    // send packet without timestamp and return time packet was sent
    return _xbee->sendTwo(_zbTx,false,true);

}


void OAgent::_broadcastResyncResponsePacket(unsigned long t2, unsigned long received_id){

    uint16_t payload[5];
    payload[0] = RESYNC_RESPONSE_HEADER;
    _addUint32_tToPayload(uint32_t(t2),((uint8_t * )(&payload)),2);
    _addUint32_tToPayload(uint32_t(received_id),((uint8_t * )(&payload)),6);

    _zbTx = ZBTxRequest(_broadcastAddress,((uint8_t *)(&payload)),sizeof(payload));
    // send packet without timestamp and return time packet was sent
    _xbee->sendTwo(_zbTx,true,false);

}


bool OAgent::_waitForResyncPacketResponse(unsigned long &rxTime, uint16_t timeout){

    return _waitForPacket(RESYNC_RESPONSE_HEADER, rxTime, true, timeout);      //assuming response is broadcasted from unsynced not
}

bool OAgent::_waitForResyncFinalPacket(unsigned long timeout){

    return _waitForPacket(RESYNC_HEADER_FINAL, false, timeout);
}

void OAgent::_broadcastResyncFinalPacket(long offset){

    uint16_t payload[3];
    payload[0] = RESYNC_HEADER_FINAL;
    _addUint32_tToPayload(uint32_t(offset),((uint8_t * )(&payload)),2);
    _zbTx = ZBTxRequest(_rx->getRemoteAddress64(),((uint8_t *)(&payload)),sizeof(payload));

    return _xbee->send(_zbTx);

}

////////////////////////////////////////////////

unsigned long OAgent::myMillis() { 
    return millis() - (unsigned long)_offset; 
}

void OAgent::_initializeFairSplitting(OLocalVertex * s, long y, long z) {
    _G->clearAllStates();                   // clear everything
    uint8_t Dout = s->getOutDegree() + 1;   // store out degree

    // // added in by Olaolu. If a node i is out, forloop adds (1/Dout_i) of its initial value
    // for (int i=0; i< NUM_REMOTE_VERTICES; i++){
    //  if ((s->getStatus(i)) == 1){
    //      y = y + getneighborY0(i);
    //         z = z + getneighborZ0(i);
    //  }
    // }   
    s->setYMin(y - s->getMin());            // set initial y value (using yMin) [y - min]
    //Serial << "\nNode "<<s->getID()<<"'s' Y[0] is: "<<s->getYMin()<<"\n";
    s->setMuMin(s->getYMin()/Dout);         // Initialize mu = y/
    s->setZ(z - s->getMin());     // set initial z value [z - min]
    //Serial << "\nNode "<<s->getID()<<"'s' Z[0] is: "<<s->getZ()<<"\n";
    s->setSigma(s->getZ()/Dout);            // Initialize sigma = z/Dout
}

// Resilient version
void OAgent::_initializeFairSplitting_RSL(OLocalVertex * s, float y, float z) {
    _G->clearAllStates();                   // clear everything
    uint8_t Dout = s->getOutDegree() + 1;   // store out degree

    // for (int i=0; i< NUM_REMOTE_VERTICES; i++){
    //     if ((s->getStatus(i)) == 1){
    //         y = y + getneighborY0(i);
    //         z = z + getneighborZ0(i);
    //     }
    // }   
    s->setYMin(y - s->getMin());            // set initial y value (using yMin) [y - min]
    s->setMuMin(s->getYMin()/Dout);         // Initialize mu = y/
    s->setZ(z - s->getMin());               // set initial z value [z - min]
    s->setSigma(s->getZ()/Dout);            // Initialize sigma = z/Dout

    //initialize min and max consensus. Min consensus is used to choose leader, max consensus is used to choose deputy
    //s->setleaderID(s->getID());
    //s->setdeputyID(s->getID());
}

void OAgent::_broadcastFairSplitPacket(OLocalVertex * s) {   
    uint16_t payload[6];           
    float mu    = (s->getMuMin())*BASE;
    float sigma = (s->getSigma())*BASE; 
    uint16_t id = s->getID();

    long Mu = long(mu);
    long Sigma = long(sigma);
    //uint16_t inheritorID = s->chooseInheritor();

    payload[0] = FAIR_SPLITTING_HEADER;
    payload[1] = Mu;
    payload[2] = Mu >> 16;
    payload[3] = Sigma;
    payload[4] = Sigma >> 16;
    payload[5] = id;
    //payload[6] = inheritorID;   //added in by Olaolu

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
#ifdef VERBOSE
    Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
#endif
}

//leaderfailure-resilient version (Olaolu)
void OAgent::_broadcastFairSplitPacket_RSL(OLocalVertex * s) {   
    uint16_t payload[8];                
    float mu    = (s->getMuMin())*BASE;
    float sigma = (s->getSigma())*BASE; 
    uint16_t id = s->getID();

    long Mu = long(mu);
    long Sigma = long(sigma);

    payload[0] = FAIR_SPLITTING_HEADER;
    payload[1] = Mu;
    payload[2] = Mu >> 16;
    payload[3] = Sigma;
    payload[4] = Sigma >> 16;
    payload[5] = id;

    if(mu<0)
    {
        payload[6]=0;   // sign of mu

    }else
    {
        payload[6]=1;    //sign of sigma

    }
    if(sigma<0)
    {
        payload[7]=0;   // sign of mu

    }else
    {
        payload[7]=1;    //sign of sigma

    }
    //payload[6] = inheritorID;   //added in by Olaolu
    //payload[7] = leaderID;   //added in by Olaolu
    //payload[8] = deputyID;   //added in by Olaolu

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
#ifdef VERBOSE
    Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
#endif
}

//leaderfailure-resilient version (Olaolu)
void OAgent::_broadcastMaxMinPacket(long max, long min) {   
    uint16_t payload[5];
    payload[0] = MAXMIN_HEADER;
    payload[1] = max;
    payload[2] = max >> 16;
    payload[3] = min;
    payload[4] = min >> 16;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
#ifdef VERBOSE
    Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
#endif
}

float OAgent::_getMuFromPacket() {
    uint8_t ptr = 2;
    uint8_t ptr_sign = 12;
    long Mu = _getUint32_tFromPacket(ptr);
    float mu = float(Mu);
    long Sign = _getUint32_tFromPacket(ptr_sign);
    
    if(Sign==0)
    {
        return (-mu/BASE);  
    }else
    {
        return (mu/BASE);  
    }
}

float OAgent::_getSigmaFromPacket() {
    uint8_t ptr = 6;
    uint8_t ptr_sign = 14;
    long Sigma = _getUint32_tFromPacket(ptr);
    float sigma = float(Sigma);
    long Sign = _getUint32_tFromPacket(ptr_sign);

    if(Sign==0)
    {
        return (-sigma/BASE);  
    }else
    {
        return (sigma/BASE);  
    }

    
}




/// End fair splitting ratio-consensus methods

/// General xbee methods

/*
 * This function verifies the validity of an xbee packet.
 * It first checks to see if a packet is available then verifies that it is a zigbee rx packet.
 * If the packet it is valid the function will return true and it will store the response in the
 * ZBRxResponse parameter.
 */
bool OAgent::_validPacketAvailable() {
    if(_xbee->getResponse().isAvailable()) {    // packet is available
        // check if recieved data is a zb rx packet
        
        if(_xbee->getResponse().getApiId() == ZB_RX_RESPONSE) { 
#ifdef VERBOSE
            if(!_quiet)
                Serial << _MEM(PSTR("Valid XBee packet received")) << endl;
#endif
            _xbee->getResponse().getZBRxResponse(_rx);       // fill out zb rx class
            return true;
        }
    }
    return false;
}

bool OAgent::_packetAvailable(uint16_t header, bool broadcast) {
    // read packet
    _xbee->readPacket();
    return _packetAvailableHelper(header,broadcast);
}

bool OAgent::_packetAvailable(uint16_t header,  unsigned long &rxTime, bool broadcast) {
    // read packet and save rx time
    rxTime = _xbee->readPacketTwo(true);
    //Serial << "\n Before packetAvailableHelper \n";
    return _packetAvailableHelper(header,broadcast);
}

uint16_t OAgent::_packetAvailable(bool broadcast) {
    _xbee->readPacket();
    if(_validPacketAvailable()) {
        if(((broadcast == true) && ((_rx->getOption() & 0x0F) == ZB_BROADCAST_PACKET)) || ((broadcast == false) && ((_rx->getOption() & 0x0F) == ZB_PACKET_ACKNOWLEDGED)))
            return _getHeaderFromPacket();
    }
    return 0x0;
}


bool  OAgent::_packetAvailable2(unsigned long &rxTime, bool broadcast){

    rxTime = _xbee->readPacketTwo(true);

    if(_validPacketAvailable()) {
        if(((broadcast == true) && ((_rx->getOption() & 0x0F) == ZB_BROADCAST_PACKET)) || ((broadcast == false) && ((_rx->getOption() & 0x0F) == ZB_PACKET_ACKNOWLEDGED)))
            return true;
    }
}


 
bool OAgent::_packetAvailableHelper(uint16_t header, bool broadcast) {
    //SerialUSB << "Here1\n";
    if(_validPacketAvailable()) {
        //SerialUSB << "HERE\n";
        //SerialUSB << _HEX(_getHeaderFromPacket());
        //SerialUSB << "\n";
        if(_getHeaderFromPacket() == header) {
            
            //SerialUSB << _HEX(_rx->getOption());
            //SerialUSB << "\n";
            if(((broadcast == true) && ((_rx->getOption() & 0x0F) == ZB_BROADCAST_PACKET)) || ((broadcast == false) && ((_rx->getOption() & 0x0F) == ZB_PACKET_ACKNOWLEDGED)))
            {
                //Serial << "Packet conditions met \n";
                return true;
            }
        }
    }
    return false; 
}

bool OAgent::_packetACKed(int timeout) {
    if(_xbee->readPacket(timeout)) { // wait for up to ack timeout period
        if(_xbee->getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) { // got a response
            _xbee->getResponse().getZBTxStatusResponse(_txStatus); // fill out status
            if (_txStatus.getDeliveryStatus() == SUCCESS) { // msg was acked
                return true;
            }
        }
    }
    return false;
}

/*
 * This function returns the two byte header of an xbee packet.  The header should be
 * in the first 2 bytes of the received packet.
 */
//uint16_t OAgent::_getHeaderFromPacket() {
//  return (uint16_t(_rx->getData(1)) << 8) + _rx->getData(0);
//}

bool OAgent::_waitForPacket(uint16_t header, unsigned long &rxTime, bool broadcast, int timeout) {
    unsigned long start;

    if(timeout != -1)
        start = millis();
    while(true) {
        if((timeout != -1) && (millis()-start >= timeout))
            return false;
        if(_packetAvailable(header,rxTime,broadcast))
        {
            return true;
        }
    }
}

bool OAgent::_waitForPacket(uint16_t header, bool broadcast, int timeout) { //Where it stays in an endless loop (if timeout =-1) until packet received
    unsigned long start;
    if(timeout != -1)
        start = millis();
    while(true) {
        if((timeout != -1) && (millis()-start >= timeout))
            return false;
        if(_packetAvailable(header,broadcast))    //unless the packet contains the expected header it will keep waiting
            return true;
    }
}

int  OAgent::_waitForPacket2(uint16_t header1, uint16_t header2, unsigned long &rxTime, bool broadcast){

    while(true){

        if(_packetAvailable2(rxTime,broadcast))
        {

            if(_getHeaderFromPacket() == header1)
                return 1;
            else if(_getHeaderFromPacket() == header2){
                uint32_t aLsb = _rx->getRemoteAddress64().getLsb();
                //Serial << "Address: ";
                //Serial << _HEX(aLsb);
                //Serial << "\n";
                if(_G->isInNeighbor(aLsb))
                {
                    //Serial << "Packet received";
                    //Serial << "\n";
                    return 2;
                }
            }
        }
    }   

}

uint16_t OAgent::_waitForValidPacket(bool broadcast, int timeout) {
    unsigned long start;
    // if using timeout, initialize clock
    if(timeout != -1)
        start = millis();
    // do forever
    while(true) {
        // if using timeout and time has elapsed, return 0x0
        if((timeout != -1) && (millis()-start >= timeout))
            return 0x0;
        // look for packet and save header
        uint16_t rsp = _packetAvailable(broadcast);
        // if packet with valid header is available, return it
        if(rsp != 0x0)
            return rsp;
    }
}

/// End general xbee methods
/// General scheduling methods

bool OAgent::_waitToStart(unsigned long startTime, bool useMyMillis, int timeout) {
    long temp;
    unsigned long s = millis();
    while(true) {
        if(useMyMillis == true) {
            temp = startTime - (myMillis());
            if(temp <= 0)
                return true;
        } else {
            temp = startTime - millis(); 
            if(temp <= 0)
                return true;
        }
        if(timeout != -1) {
            temp = millis()-s;
            if(temp > timeout)
                return false;
        }
    }    
}

/// End general scheduling methods
/// General coordination helper functions

/*
 * This function generates a random transmit time given an iteration period and inter-frame spacing.
 * The current time in milliseconds is used as the seed for the random number generation.
 */
uint16_t OAgent::_genTxTime(uint16_t iterationPeriod, uint8_t ITF) {
    srand(analogRead(0));
    return (rand() % (iterationPeriod - 2*ITF)) + ITF;
}

/*
 * This function generates a random transmit time given an iteration period and inter-frame spacing.
 * A seed for the random number generation is accepted as an argument.
 */
uint16_t OAgent::_genTxTime(uint16_t iterationPeriod, uint8_t ITF, int seed) {
    srand(seed);
    return (rand() % (iterationPeriod - 2*ITF)) + ITF;
}

bool OAgent::_timeToTransmit(uint16_t startTime, uint16_t txTime) {
    if((int((millis() - startTime)) >= txTime))
        return true;
    else
        return false;
}




// Primal Dual Algorithm Communication Methods

float OAgent::_getActiveFlowFromPacket_neighbor() {
    int32_t mag_fp = (int32_t(_rx->getData(8)) << 24) + (int32_t(_rx->getData(7)) << 16) + (int16_t(_rx->getData(6)) << 8) + int8_t(_rx->getData(5));
    int8_t sign_fp = -1 + ((_rx->getData(4))*2);
    float fp = (float) (sign_fp*mag_fp);
    fp = fp/BASE;

    return fp;
}

float OAgent::_getReactiveFlowFromPacket_neighbor() {
    int32_t mag_fq = (int32_t(_rx->getData(13)) << 24) + (int32_t(_rx->getData(12)) << 16) + (int16_t(_rx->getData(11)) << 8) + int8_t(_rx->getData(10));
    int8_t sign_fq = -1 + ((_rx->getData(9))*2);
    float fq = (float) (sign_fq*mag_fq);
    fq = fq/BASE;

    return fq;
}

float OAgent::_getLambdaFromPacket_neighbor() {
    int32_t mag_lambda = (int32_t(_rx->getData(18)) << 24) + (int32_t(_rx->getData(17)) << 16) + (int16_t(_rx->getData(16)) << 8) + int8_t(_rx->getData(15));
    int8_t sign_lambda = -1 + ((_rx->getData(14))*2);
    float lambda = (float) (sign_lambda*mag_lambda);
    lambda = lambda/BASE;

    return lambda;
}

float OAgent::_getActiveFlowGradientFromPacket_neighbor() {
    int32_t mag_gfp = (int32_t(_rx->getData(23)) << 24) + (int32_t(_rx->getData(22)) << 16) + (int16_t(_rx->getData(21)) << 8) + int8_t(_rx->getData(20));
    int8_t sign_gfp = -1 + ((_rx->getData(19))*2);
    float gfp = (float) (sign_gfp*mag_gfp);
    gfp = gfp/BASE;

    return gfp;
}

float OAgent::_getReactiveFlowGradientFromPacket_neighbor() {
    int32_t mag_gfq = (int32_t(_rx->getData(28)) << 24) + (int32_t(_rx->getData(27)) << 16) + (int16_t(_rx->getData(26)) << 8) + int8_t(_rx->getData(25));
    int8_t sign_gfq = -1 + ((_rx->getData(24))*2);
    float gfq = (float) (sign_gfq*mag_gfq);
    gfq = gfq/BASE;

    return gfq;
}

float OAgent::_getLambdaGradientFromPacket_neighbor() {
    int32_t mag_glambda = (int32_t(_rx->getData(33)) << 24) + (int32_t(_rx->getData(32)) << 16) + (int16_t(_rx->getData(31)) << 8) + int8_t(_rx->getData(30));
    int8_t sign_glambda = -1 + ((_rx->getData(29))*2);
    float glambda = (float) (sign_glambda*mag_glambda);
    glambda = glambda/BASE;

    return glambda;
}

bool OAgent::_getFlagFromPacket_ACC() {
    bool flag = (bool) _rx->getData(34);
    
    return flag;
}


bool OAgent::_getFlagFromPacket() {
    bool flag = (bool) _rx->getData(19);
    
    return flag;
}

float OAgent::_getActiveFlowFromPacket_node() {
    int32_t mag_fp = (int32_t(_rx->getData(24)) << 24) + (int32_t(_rx->getData(23)) << 16) + (int16_t(_rx->getData(22)) << 8) + int8_t(_rx->getData(21));
    int8_t sign_fp = -1 + ((_rx->getData(20))*2);
    float fp = (float) (sign_fp*mag_fp);
    fp = fp/BASE;

    return fp;
}

float OAgent::_getReactiveFlowFromPacket_node() {
    int32_t mag_fq = (int32_t(_rx->getData(29)) << 24) + (int32_t(_rx->getData(28)) << 16) + (int16_t(_rx->getData(27)) << 8) + int8_t(_rx->getData(26));
    int8_t sign_fq = -1 + ((_rx->getData(25))*2);
    float fq = (float) (sign_fq*mag_fq);
    fq = fq/BASE;

    return fq;
}

float OAgent::_getLambdaFromPacket_node() {
    int32_t mag_lambda = (int32_t(_rx->getData(34)) << 24) + (int32_t(_rx->getData(33)) << 16) + (int16_t(_rx->getData(32)) << 8) + int8_t(_rx->getData(31));
    int8_t sign_lambda = -1 + ((_rx->getData(30))*2);
    float lambda = (float) (sign_lambda*mag_lambda);
    lambda = lambda/BASE;

    return lambda;
}

float OAgent::_getActiveFlowFromPacket_nodeACC() {
    int32_t mag_fp = (int32_t(_rx->getData(39)) << 24) + (int32_t(_rx->getData(38)) << 16) + (int16_t(_rx->getData(37)) << 8) + int8_t(_rx->getData(36));
    int8_t sign_fp = -1 + ((_rx->getData(35))*2);
    float fp = (float) (sign_fp*mag_fp);
    fp = fp/BASE;

    return fp;
}

float OAgent::_getReactiveFlowFromPacket_nodeACC() {
    int32_t mag_fq = (int32_t(_rx->getData(44)) << 24) + (int32_t(_rx->getData(43)) << 16) + (int16_t(_rx->getData(42)) << 8) + int8_t(_rx->getData(41));
    int8_t sign_fq = -1 + ((_rx->getData(40))*2);
    float fq = (float) (sign_fq*mag_fq);
    fq = fq/BASE;

    return fq;
}

float OAgent::_getLambdaFromPacket_nodeACC() {
    int32_t mag_lambda = (int32_t(_rx->getData(49)) << 24) + (int32_t(_rx->getData(48)) << 16) + (int16_t(_rx->getData(47)) << 8) + int8_t(_rx->getData(46));
    int8_t sign_lambda = -1 + ((_rx->getData(45))*2);
    float lambda = (float) (sign_lambda*mag_lambda);
    lambda = lambda/BASE;

    return lambda;
}

float OAgent::_getActiveFlowGradientFromPacket_node() {
    int32_t mag_fp = (int32_t(_rx->getData(54)) << 24) + (int32_t(_rx->getData(53)) << 16) + (int16_t(_rx->getData(52)) << 8) + int8_t(_rx->getData(51));
    int8_t sign_fp = -1 + ((_rx->getData(50))*2);
    float fp = (float) (sign_fp*mag_fp);
    fp = fp/BASE;

    return fp;
}

float OAgent::_getReactiveFlowGradientFromPacket_node() {
    int32_t mag_fq = (int32_t(_rx->getData(59)) << 24) + (int32_t(_rx->getData(58)) << 16) + (int16_t(_rx->getData(57)) << 8) + int8_t(_rx->getData(56));
    int8_t sign_fq = -1 + ((_rx->getData(55))*2);
    float fq = (float) (sign_fq*mag_fq);
    fq = fq/BASE;

    return fq;
}

float OAgent::_getLambdaGradientFromPacket_node() {
    int32_t mag_lambda = (int32_t(_rx->getData(64)) << 24) + (int32_t(_rx->getData(63)) << 16) + (int16_t(_rx->getData(62)) << 8) + int8_t(_rx->getData(61));
    int8_t sign_lambda = -1 + ((_rx->getData(60))*2);
    float lambda = (float) (sign_lambda*mag_lambda);
    lambda = lambda/BASE;

    return lambda;
}

void OAgent::_unicastPacket_P(uint16_t recipientID, float fP, float fQ, float Lambda, bool flag) {
    uint8_t payload[20];
    uint32_t fp;
    uint32_t fq;
    uint32_t lambda;
    uint8_t sign_fp;
    uint8_t sign_fq;
    uint8_t sign_lambda;
    fP = fP*BASE;
    fQ = fQ*BASE;
    Lambda = Lambda*BASE;

    //check if active flow is negative
    if (fP < 0) 
    {
        fP = -1*fP;
        fp = (uint32_t) fP;
        sign_fp = 0;
    }
    else
    {
        fp = (uint32_t) fP;
        sign_fp = 1;
    }

   //check if reactive flow is negative
    if (fQ < 0) 
    {
        fQ = -1*fQ;
        fq = (uint32_t) fQ;
        sign_fq = 0;
    }
    else
    {
        fq = (uint32_t) fQ;
        sign_fq = 1;
    }

   //check if lambda is negative
    if (Lambda < 0) 
    {
        Lambda = -1*Lambda;
        lambda = (uint32_t) Lambda;
        sign_lambda = 0;
    }
    else
    {
        lambda = (uint32_t) Lambda;
        sign_lambda = 1;
    }

    //construct payload
    payload[0] = PD_HEADER;
    payload[1] = PD_HEADER >> 8;
    payload[2] = recipientID;
    payload[3] = recipientID >> 8;
    payload[4] = sign_fp;
    payload[5] = fp;
    payload[6] = fp >> 8;
    payload[7] = fp >> 16;
    payload[8] = fp >> 24;
    payload[9] = sign_fq;
    payload[10] = fq;
    payload[11] = fq >> 8;
    payload[12] = fq >> 16;
    payload[13] = fq >> 24;
    payload[14] = sign_lambda;
    payload[15] = lambda;
    payload[16] = lambda >> 8;
    payload[17] = lambda >> 16;
    payload[18] = lambda >> 24;
    payload[19] = flag;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
    #ifdef VERBOSE
        Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
    #endif
}

void OAgent::_unicastPacket_C(uint16_t recipientID, float fP_c, float fQ_c, float Lambda_c, bool flag, float fP_p, float fQ_p, float Lambda_p) {
    uint8_t payload[35];
    uint32_t fp_c;
    uint32_t fq_c;
    uint32_t lambda_c;
    uint8_t sign_fp_c;
    uint8_t sign_fq_c;
    uint8_t sign_lambda_c;
    fP_c = fP_c*BASE;
    fQ_c = fQ_c*BASE;
    Lambda_c = Lambda_c*BASE;

    uint32_t fp_p;
    uint32_t fq_p;
    uint32_t lambda_p;
    uint8_t sign_fp_p;
    uint8_t sign_fq_p;
    uint8_t sign_lambda_p;
    fP_p = fP_p*BASE;
    fQ_p = fQ_p*BASE;
    Lambda_p = Lambda_p*BASE;

    //check if active flow is negative
    if (fP_c < 0) 
    {
        fP_c = -1*fP_c;
        fp_c = (uint32_t) fP_c;
        sign_fp_c = 0;
    }
    else
    {
        fp_c = (uint32_t) fP_c;
        sign_fp_c = 1;
    }

   //check if reactive flow is negative
    if (fQ_c < 0) 
    {
        fQ_c = -1*fQ_c;
        fq_c = (uint32_t) fQ_c;
        sign_fq_c = 0;
    }
    else
    {
        fq_c = (uint32_t) fQ_c;
        sign_fq_c = 1;
    }

   //check if lambda is negative
    if (Lambda_c < 0) 
    {
        Lambda_c = -1*Lambda_c;
        lambda_c = (uint32_t) Lambda_c;
        sign_lambda_c = 0;
    }
    else
    {
        lambda_c = (uint32_t) Lambda_c;
        sign_lambda_c = 1;
    }
    /////////////////////////////////////////////
    /////////////////////////////////////////////    
    //check if active flow is negative
    if (fP_p < 0) 
    {
        fP_p = -1*fP_p;
        fp_p = (uint32_t) fP_p;
        sign_fp_p = 0;
    }
    else
    {
        fp_p = (uint32_t) fP_p;
        sign_fp_p = 1;
    }

   //check if reactive flow is negative
    if (fQ_p < 0) 
    {
        fQ_p = -1*fQ_p;
        fq_p = (uint32_t) fQ_p;
        sign_fq_p = 0;
    }
    else
    {
        fq_p = (uint32_t) fQ_p;
        sign_fq_p = 1;
    }

   //check if lambda is negative
    if (Lambda_p < 0) 
    {
        Lambda_p = -1*Lambda_p;
        lambda_p = (uint32_t) Lambda_p;
        sign_lambda_p = 0;
    }
    else
    {
        lambda_p = (uint32_t) Lambda_p;
        sign_lambda_p = 1;
    }
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    //construct payload
    payload[0] = PD_HEADER;
    payload[1] = PD_HEADER >> 8;
    payload[2] = recipientID;
    payload[3] = recipientID >> 8;
    payload[4] = sign_fp_c;
    payload[5] = fp_c;
    payload[6] = fp_c >> 8;
    payload[7] = fp_c >> 16;
    payload[8] = fp_c >> 24;
    payload[9] = sign_fq_c;
    payload[10] = fq_c;
    payload[11] = fq_c >> 8;
    payload[12] = fq_c >> 16;
    payload[13] = fq_c >> 24;
    payload[14] = sign_lambda_c;
    payload[15] = lambda_c;
    payload[16] = lambda_c >> 8;
    payload[17] = lambda_c >> 16;
    payload[18] = lambda_c >> 24;

    payload[19] = flag;

    payload[20] = sign_fp_p;
    payload[21] = fp_p;
    payload[22] = fp_p >> 8;
    payload[23] = fp_p >> 16;
    payload[24] = fp_p >> 24;
    payload[25] = sign_fq_p;
    payload[26] = fq_p;
    payload[27] = fq_p >> 8;
    payload[28] = fq_p >> 16;
    payload[29] = fq_p >> 24;
    payload[30] = sign_lambda_p;
    payload[31] = lambda_p;
    payload[32] = lambda_p >> 8;
    payload[33] = lambda_p >> 16;
    payload[34] = lambda_p >> 24;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
    #ifdef VERBOSE
        Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
    #endif
}


void OAgent::_sendToChild(uint16_t recipientID, float fP, float fQ, float Lambda, float gfP, float gfQ, float gLambda, bool flag) {
    uint8_t payload[35];
    uint32_t fp;
    uint32_t fq;
    uint32_t lambda;
    uint32_t gfp;
    uint32_t gfq;
    uint32_t glambda;
    uint8_t sign_fp;
    uint8_t sign_fq;
    uint8_t sign_lambda;
    uint8_t sign_gfp;
    uint8_t sign_gfq;
    uint8_t sign_glambda;
    fP = fP*BASE;
    fQ = fQ*BASE;
    Lambda = Lambda*BASE;
    gfP = gfP*BASE;
    gfQ = gfQ*BASE;
    gLambda = gLambda*BASE;

    //check if active flow is negative
    if (fP < 0) 
    {
        fP = -1*fP;
        fp = (uint32_t) fP;
        sign_fp = 0;
    }
    else
    {
        fp = (uint32_t) fP;
        sign_fp = 1;
    }

   //check if reactive flow is negative
    if (fQ < 0) 
    {
        fQ = -1*fQ;
        fq = (uint32_t) fQ;
        sign_fq = 0;
    }
    else
    {
        fq = (uint32_t) fQ;
        sign_fq = 1;
    }

   //check if lambda is negative
    if (Lambda < 0) 
    {
        Lambda = -1*Lambda;
        lambda = (uint32_t) Lambda;
        sign_lambda = 0;
    }
    else
    {
        lambda = (uint32_t) Lambda;
        sign_lambda = 1;
    }

    //check if active flow gradient is negative
    if (gfP < 0) 
    {
        gfP = -1*gfP;
        gfp = (uint32_t) gfP;
        sign_gfp = 0;
    }
    else
    {
        gfp = (uint32_t) gfP;
        sign_gfp = 1;
    }

   //check if reactive flow gradient is negative
    if (gfQ < 0) 
    {
        gfQ = -1*gfQ;
        gfq = (uint32_t) gfQ;
        sign_gfq = 0;
    }
    else
    {
        gfq = (uint32_t) gfQ;
        sign_gfq = 1;
    }

   //check if lambda gradient is negative
    if (gLambda < 0) 
    {
        gLambda = -1*gLambda;
        glambda = (uint32_t) gLambda;
        sign_glambda = 0;
    }
    else
    {
        glambda = (uint32_t) gLambda;
        sign_glambda = 1;
    }

    //construct payload
    payload[0] = PD_HEADER;
    payload[1] = PD_HEADER >> 8;
    payload[2] = recipientID;
    payload[3] = recipientID >> 8;
    payload[4] = sign_fp;
    payload[5] = fp;
    payload[6] = fp >> 8;
    payload[7] = fp >> 16;
    payload[8] = fp >> 24;
    payload[9] = sign_fq;
    payload[10] = fq;
    payload[11] = fq >> 8;
    payload[12] = fq >> 16;
    payload[13] = fq >> 24;
    payload[14] = sign_lambda;
    payload[15] = lambda;
    payload[16] = lambda >> 8;
    payload[17] = lambda >> 16;
    payload[18] = lambda >> 24;
    payload[19] = sign_gfp;
    payload[20] = gfp;
    payload[21] = gfp >> 8;
    payload[22] = gfp >> 16;
    payload[23] = gfp >> 24;
    payload[24] = sign_gfq;
    payload[25] = gfq;
    payload[26] = gfq >> 8;
    payload[27] = gfq >> 16;
    payload[28] = gfq >> 24;
    payload[29] = sign_glambda;
    payload[30] = glambda;
    payload[31] = glambda >> 8;
    payload[32] = glambda >> 16;
    payload[33] = glambda >> 24;

    payload[34] = flag;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
    #ifdef VERBOSE
        Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
    #endif
}

void OAgent::_sendToParent(uint16_t recipientID, float fP, float fQ, float Lambda, float gfP, float gfQ, float gLambda, bool flag, float fP_p, float fQ_p, float Lambda_p, float gfP_p, float gfQ_p, float gLambda_p) {
    uint8_t payload[65];
    uint32_t fp;
    uint32_t fq;
    uint32_t lambda;
    uint32_t gfp;
    uint32_t gfq;
    uint32_t glambda;
    uint8_t sign_fp;
    uint8_t sign_fq;
    uint8_t sign_lambda;
    uint8_t sign_gfp;
    uint8_t sign_gfq;
    uint8_t sign_glambda;
    uint32_t fp_p;
    uint32_t fq_p;
    uint32_t lambda_p;
    uint32_t gfp_p;
    uint32_t gfq_p;
    uint32_t glambda_p;
    uint8_t sign_fp_p;
    uint8_t sign_fq_p;
    uint8_t sign_lambda_p;
    uint8_t sign_gfp_p;
    uint8_t sign_gfq_p;
    uint8_t sign_glambda_p;
    fP = fP*BASE;
    fQ = fQ*BASE;
    Lambda = Lambda*BASE;
    gfP = gfP*BASE;
    gfQ = gfQ*BASE;
    gLambda = gLambda*BASE;
    fP_p = fP_p*BASE;
    fQ_p = fQ_p*BASE;
    Lambda_p = Lambda_p*BASE;
    gfP_p = gfP_p*BASE;
    gfQ_p = gfQ_p*BASE;
    gLambda_p = gLambda_p*BASE;

    //check if active flow is negative
    if (fP < 0) 
    {
        fP = -1*fP;
        fp = (uint32_t) fP;
        sign_fp = 0;
    }
    else
    {
        fp = (uint32_t) fP;
        sign_fp = 1;
    }

   //check if reactive flow is negative
    if (fQ < 0) 
    {
        fQ = -1*fQ;
        fq = (uint32_t) fQ;
        sign_fq = 0;
    }
    else
    {
        fq = (uint32_t) fQ;
        sign_fq = 1;
    }

   //check if lambda is negative
    if (Lambda < 0) 
    {
        Lambda = -1*Lambda;
        lambda = (uint32_t) Lambda;
        sign_lambda = 0;
    }
    else
    {
        lambda = (uint32_t) Lambda;
        sign_lambda = 1;
    }

    //check if active flow gradient is negative
    if (gfP < 0) 
    {
        gfP = -1*gfP;
        gfp = (uint32_t) gfP;
        sign_gfp = 0;
    }
    else
    {
        gfp = (uint32_t) gfP;
        sign_gfp = 1;
    }

   //check if reactive flow gradient is negative
    if (gfQ < 0) 
    {
        gfQ = -1*gfQ;
        gfq = (uint32_t) gfQ;
        sign_gfq = 0;
    }
    else
    {
        gfq = (uint32_t) gfQ;
        sign_gfq = 1;
    }

   //check if lambda gradient is negative
    if (gLambda < 0) 
    {
        gLambda = -1*gLambda;
        glambda = (uint32_t) gLambda;
        sign_glambda = 0;
    }
    else
    {
        glambda = (uint32_t) gLambda;
        sign_glambda = 1;
    }

    //check if active flow is negative
    if (fP_p < 0) 
    {
        fP_p = -1*fP_p;
        fp_p = (uint32_t) fP_p;
        sign_fp_p = 0;
    }
    else
    {
        fp_p = (uint32_t) fP_p;
        sign_fp_p = 1;
    }

   //check if reactive flow is negative
    if (fQ_p < 0) 
    {
        fQ_p = -1*fQ_p;
        fq_p = (uint32_t) fQ_p;
        sign_fq_p = 0;
    }
    else
    {
        fq_p = (uint32_t) fQ_p;
        sign_fq_p = 1;
    }

   //check if lambda is negative
    if (Lambda_p < 0) 
    {
        Lambda_p = -1*Lambda_p;
        lambda_p = (uint32_t) Lambda_p;
        sign_lambda_p = 0;
    }
    else
    {
        lambda_p = (uint32_t) Lambda_p;
        sign_lambda_p = 1;
    }

    //check if active flow gradient is negative
    if (gfP_p < 0) 
    {
        gfP_p = -1*gfP_p;
        gfp_p = (uint32_t) gfP_p;
        sign_gfp_p = 0;
    }
    else
    {
        gfp_p = (uint32_t) gfP_p;
        sign_gfp_p = 1;
    }

   //check if reactive flow gradient is negative
    if (gfQ_p < 0) 
    {
        gfQ_p = -1*gfQ_p;
        gfq_p = (uint32_t) gfQ_p;
        sign_gfq_p = 0;
    }
    else
    {
        gfq_p = (uint32_t) gfQ_p;
        sign_gfq_p = 1;
    }

   //check if lambda gradient is negative
    if (gLambda_p < 0) 
    {
        gLambda_p = -1*gLambda_p;
        glambda_p = (uint32_t) gLambda_p;
        sign_glambda_p = 0;
    }
    else
    {
        glambda_p = (uint32_t) gLambda_p;
        sign_glambda_p = 1;
    }

    //construct payload
    payload[0] = PD_HEADER;
    payload[1] = PD_HEADER >> 8;
    payload[2] = recipientID;
    payload[3] = recipientID >> 8;
    payload[4] = sign_fp;
    payload[5] = fp;
    payload[6] = fp >> 8;
    payload[7] = fp >> 16;
    payload[8] = fp >> 24;
    payload[9] = sign_fq;
    payload[10] = fq;
    payload[11] = fq >> 8;
    payload[12] = fq >> 16;
    payload[13] = fq >> 24;
    payload[14] = sign_lambda;
    payload[15] = lambda;
    payload[16] = lambda >> 8;
    payload[17] = lambda >> 16;
    payload[18] = lambda >> 24;
    payload[19] = sign_gfp;
    payload[20] = gfp;
    payload[21] = gfp >> 8;
    payload[22] = gfp >> 16;
    payload[23] = gfp >> 24;
    payload[24] = sign_gfq;
    payload[25] = gfq;
    payload[26] = gfq >> 8;
    payload[27] = gfq >> 16;
    payload[28] = gfq >> 24;
    payload[29] = sign_glambda;
    payload[30] = glambda;
    payload[31] = glambda >> 8;
    payload[32] = glambda >> 16;
    payload[33] = glambda >> 24;

    payload[34] = flag;

    payload[35] = sign_fp_p;
    payload[36] = fp_p;
    payload[37] = fp_p >> 8;
    payload[38] = fp_p >> 16;
    payload[39] = fp_p >> 24;
    payload[40] = sign_fq_p;
    payload[41] = fq_p;
    payload[42] = fq_p >> 8;
    payload[43] = fq_p >> 16;
    payload[44] = fq_p >> 24;
    payload[45] = sign_lambda_p;
    payload[46] = lambda_p;
    payload[47] = lambda_p >> 8;
    payload[48] = lambda_p >> 16;
    payload[49] = lambda_p >> 24;
    payload[50] = sign_gfp_p;
    payload[51] = gfp_p;
    payload[52] = gfp_p >> 8;
    payload[53] = gfp_p >> 16;
    payload[54] = gfp_p >> 24;
    payload[55] = sign_gfq_p;
    payload[56] = gfq_p;
    payload[57] = gfq_p >> 8;
    payload[58] = gfq_p >> 16;
    payload[59] = gfq_p >> 24;
    payload[60] = sign_glambda_p;
    payload[61] = glambda_p;
    payload[62] = glambda_p >> 8;
    payload[63] = glambda_p >> 16;
    payload[64] = glambda_p >> 24;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
    #ifdef VERBOSE
        Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
    #endif
}

bool OAgent::_waitForNeighborPacket(uint8_t &neighborID, uint16_t header, bool broadcast, int timeout) {
    unsigned long start;
    if(timeout != -1)
        start = millis();
    while(true) {
        if((timeout != -1) && (millis()-start >= timeout))
            return false;
        if(_waitForPacket(header,true,timeout)) {                       //unless the packet contains the expected header it will keep waiting
            int32_t aLsb = _rx->getRemoteAddress64().getLsb();
            uint8_t index;  
            if(_G->isInNeighbor(aLsb,index)) {
                neighborID = index + 1;
                return true;
            }
        }
    }
}

bool OAgent::_waitForUnicastPacket(uint8_t &neighborID, uint8_t nodeID, uint16_t header, bool broadcast, int timeout) {
    unsigned long start;
    if(timeout != -1)
        start = millis();
    while(true) {
        if((timeout != -1) && (millis()-start >= timeout))
            return false;
        if(_waitForPacket(header,true,timeout)) {                       //unless the packet contains the expected header it will keep waiting
            int32_t aLsb = _rx->getRemoteAddress64().getLsb();
            uint16_t recipientID = _getRecipientIDFromPacket();
            uint8_t index;  
            if(_G->isInNeighbor(aLsb,index)) {
                if(recipientID == nodeID) {    //check that the packet's recipient ID matches the node ID
                    neighborID = index + 1;
                    return true;
                }
            }
        }
    }
}

 // End Primal Dual Algorithm communication methods




// Economic Dispatch Algorithm Communication Methods

float OAgent::_getSumLambdaFromPacket() {
    int32_t mag_sumLambda = (int32_t(_rx->getData(6)) << 24) + (int32_t(_rx->getData(5)) << 16) + (int16_t(_rx->getData(4)) << 8) + int8_t(_rx->getData(3));
    int8_t sign_sumLambda = -1 + ((_rx->getData(2))*2);
    float sumLambda = (float) (sign_sumLambda*mag_sumLambda);
    sumLambda = sumLambda/BASE;

    return sumLambda;
}

float OAgent::_getSumNuFromPacket() {
    int32_t mag_sumNu = (int32_t(_rx->getData(11)) << 24) + (int32_t(_rx->getData(10)) << 16) + (int16_t(_rx->getData(9)) << 8) + int8_t(_rx->getData(8));
    int8_t sign_sumNu = -1 + ((_rx->getData(7))*2);
    float sumNu = (float) (sign_sumNu*mag_sumNu);
    sumNu = sumNu/BASE;

    return sumNu;
}

float OAgent::_getSumGammaFromPacket() {
    int32_t mag_sumGamma = (int32_t(_rx->getData(16)) << 24) + (int32_t(_rx->getData(15)) << 16) + (int16_t(_rx->getData(14)) << 8) + int8_t(_rx->getData(13));
    int8_t sign_sumGamma = -1 + ((_rx->getData(12))*2);
    float sumGamma = (float) (sign_sumGamma*mag_sumGamma);
    sumGamma = sumGamma/BASE;

    return sumGamma;
}

void OAgent::_sendToNeighbor(float sumLAMBDA, float sumNU) {
    uint8_t payload[12];
    uint32_t sumLambda;
    uint32_t sumNu;
    uint8_t sign_sumLambda;
    uint8_t sign_sumNu;
    sumLAMBDA = sumLAMBDA*BASE;
    sumNU = sumNU*BASE;

    //check if sumLambda is negative
    if (sumLAMBDA < 0) 
    {
        sumLAMBDA = -1*sumLAMBDA;
        sumLambda = (uint32_t) sumLAMBDA;
        sign_sumLambda = 0;
    }
    else
    {
        sumLambda = (uint32_t) sumLAMBDA;
        sign_sumLambda = 1;
    }

   //check if sumNuis negative
    if (sumNU < 0) 
    {
        sumNU = -1*sumNU;
        sumNu = (uint32_t) sumNU;
        sign_sumNu = 0;
    }
    else
    {
        sumNu = (uint32_t) sumNU;
        sign_sumNu = 1;
    }


    //construct payload
    payload[0] = ED_HEADER;
    payload[1] = ED_HEADER >> 8;
    payload[2] = sign_sumLambda;
    payload[3] = sumLambda;
    payload[4] = sumLambda >> 8;
    payload[5] = sumLambda >> 16;
    payload[6] = sumLambda >> 24;
    payload[7] = sign_sumNu;
    payload[8] = sumNu;
    payload[9] = sumNu >> 8;
    payload[10] = sumNu >> 16;
    payload[11] = sumNu >> 24;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
    #ifdef VERBOSE
        Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
    #endif
}

void OAgent::_sendToNeighbor_ACC(float sumLAMBDA, float sumNU, float sumGAMMA) {
    uint8_t payload[17];
    uint32_t sumLambda;
    uint32_t sumNu;
    uint32_t sumGamma;
    uint8_t sign_sumLambda;
    uint8_t sign_sumNu;
    uint8_t sign_sumGamma;
    sumLAMBDA = sumLAMBDA*BASE;
    sumNU = sumNU*BASE;
    sumGAMMA = sumGAMMA*BASE;

    //check if sumLambda is negative
    if (sumLAMBDA < 0) 
    {
        sumLAMBDA = -1*sumLAMBDA;
        sumLambda = (uint32_t) sumLAMBDA;
        sign_sumLambda = 0;
    }
    else
    {
        sumLambda = (uint32_t) sumLAMBDA;
        sign_sumLambda = 1;
    }

   //check if sumNuis negative
    if (sumNU < 0) 
    {
        sumNU = -1*sumNU;
        sumNu = (uint32_t) sumNU;
        sign_sumNu = 0;
    }
    else
    {
        sumNu = (uint32_t) sumNU;
        sign_sumNu = 1;
    }

   //check if sumGamma is negative
    if (sumGAMMA < 0) 
    {
        sumGAMMA = -1*sumGAMMA;
        sumGamma = (uint32_t) sumGAMMA;
        sign_sumGamma = 0;
    }
    else
    {
        sumGamma = (uint32_t) sumGAMMA;
        sign_sumGamma = 1;
    }



    //construct payload
    payload[0] = ED_HEADER;
    payload[1] = ED_HEADER >> 8;
    payload[2] = sign_sumLambda;
    payload[3] = sumLambda;
    payload[4] = sumLambda >> 8;
    payload[5] = sumLambda >> 16;
    payload[6] = sumLambda >> 24;
    payload[7] = sign_sumNu;
    payload[8] = sumNu;
    payload[9] = sumNu >> 8;
    payload[10] = sumNu >> 16;
    payload[11] = sumNu >> 24;
    payload[12] = sign_sumGamma;
    payload[13] = sumGamma;
    payload[14] = sumGamma >> 8;
    payload[15] = sumGamma >> 16;
    payload[16] = sumGamma >> 24;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
    #ifdef VERBOSE
        Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
    #endif
}

 // End Economic Dispatch Algorithm communication methods


void OAgent::_broadcastSchedulePacket(uint16_t header, unsigned long startTime, uint8_t numIterations, uint16_t period) {
    uint8_t payload[9];
    // put header in payload array
    payload[0] = header;
    payload[1] = header >> 8;
    // put start time in payload array
    payload[2] = startTime;
    payload[3] = startTime >> 8;
    payload[4] = startTime >> 16;
    payload[5] = startTime >> 24;
    // put number of iterations in payload array
    payload[6] = numIterations;
    // put iteration period in payload array
    payload[7] = period;
    payload[8] = period >> 8;
    // put payload in zigbee transmit object
    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); 
    // transmit packet
    _xbee->send(_zbTx);
}

void OAgent::_broadcastSchedulePacketPD(unsigned long startTime, uint16_t numIterations) {
    uint8_t payload[8];
    // put header in payload array
    payload[0] = SCHEDULE_PD_HEADER;
    payload[1] = SCHEDULE_PD_HEADER >> 8;
    // put start time in payload array
    payload[2] = startTime;
    payload[3] = startTime >> 8;
    payload[4] = startTime >> 16;
    payload[5] = startTime >> 24;
    // put number of iterations in payload array
    payload[6] = numIterations;
    payload[7] = numIterations >> 8;
    // put payload in zigbee transmit object
    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); 
    // transmit packet
    _xbee->send(_zbTx);
}

void OAgent::_broadcastSchedulePacketED(unsigned long startTime, uint16_t numIterations) {
    uint8_t payload[8];
    // put header in payload array
    payload[0] = SCHEDULE_ED_HEADER;
    payload[1] = SCHEDULE_ED_HEADER >> 8;
    // put start time in payload array
    payload[2] = startTime;
    payload[3] = startTime >> 8;
    payload[4] = startTime >> 16;
    payload[5] = startTime >> 24;
    // put number of iterations in payload array
    payload[6] = numIterations;
    payload[7] = numIterations >> 8;
    // put payload in zigbee transmit object
    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); 
    // transmit packet
    _xbee->send(_zbTx);
}

void OAgent::_waitForSchedulePacket(uint16_t header, unsigned long &startTime, uint8_t &iterations, uint16_t &period, uint8_t id, int timeout) {
    if(_waitForPacket(header,true,timeout)) {  //stays in loop until desired packet received
        if(header == SCHEDULE_FAIR_SPLIT_HEADER || header == SCHEDULE_OPTIMAL_DISPATCH_HEADER) {
            startTime   = _getStartTimeFromPacket();
            iterations  = _getIterationsFromPacket();
            period      = _getPeriodFromPacket();
            
            
                uint16_t txtime;
                txtime = rand()%50;    //so that transmission occurs at different points in time per node
                delay(txtime);
                _broadcastACKPacket(ACK_START_HEADER,id);    
                
            
        }
    }
}

bool OAgent::_waitForSchedulePacket_RSL(uint16_t header, unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout) {
    uint8_t neighborID;
    LinkedList * l = _G->getLinkedList();                               //get pointer to linked list
    OLocalVertex * s = _G->getLocalVertex();                            // store pointer to local vertex 
    l->resetLinkedListStatus(s->getStatusP());                          //gets linkedlist and resets status of online neighbors to 2 
    
    Serial << "Waiting for Schedule Packet"<<endl;
    delay(5);

    if(_waitForNeighborPacket(neighborID,header,true,timeout)) {        //stays in loop until desired packet received
        if(header == SCHEDULE_FAIR_SPLIT_HEADER)
        {       
            startTime   = _getStartTimeFromPacket();
            iterations  = _getIterationsFromPacket();
            period      = _getPeriodFromPacket();
            uint16_t start = millis();
            while(uint16_t(millis()-start) < 10)
            {
                _broadcastScheduleFairSplitPacket(startTime,iterations,period);
                _broadcastACKPacket(ACK_START_HEADER,neighborID);
            }
            return true;
        }
        else if(header == SCHEDULE_MAXMIN_HEADER)
        {
            Serial << "Received a Schedule Packet"<<endl;
            delay(5);

            startTime   = _getStartTimeFromPacket();
            iterations  = _getIterationsFromPacket();
            period      = _getPeriodFromPacket();
            uint16_t start = millis();
            s->setStatus(neighborID, 3);
            l->updateLinkedList(s->getStatusP());                       //update linked list
            while(true)
            {
                Serial << "Sending Schedule ACK Packet to node " << neighborID<<endl;
                delay(5);

                _broadcastACKPacket(SCHEDULE_MAXMIN_ACK_HEADER,neighborID);
                if(_waitForUnicastPacket(neighborID,(_G->getLocalVertex())->getID(),SCHEDULE_MAXMIN_ACKACK_HEADER,true,100))                        //wait for an acknowledgement for candactcode packet
                {
                    Serial << "received schedule ACKACK from node " << neighborID<<endl;
                    delay(5);
                    break;
                }
            }
            return _waitForChildSchedulePacketRC(SCHEDULE_MAXMIN_HEADER,SCHEDULE_TIMEOUT, startTime, iterations, period);
        }
    }
    else
        return false;
}

bool OAgent::_waitForParentSchedulePacketRC(unsigned long &startTime, uint8_t &iterations, uint16_t &period,int timeout) {
    uint8_t neighborID;
    uint16_t header = SCHEDULE_FAIR_SPLIT_HEADER;
    LinkedList * l = _G->getLinkedList();                                           //get pointer to linked list
    OLocalVertex * s = _G->getLocalVertex();                                        // store pointer to local vertex
    l->resetLinkedListStatus(s->getStatusP());                   //gets linkedlist and resets status of online neighbors to 2
    uint8_t counter = 1;

    Serial << "Waiting for Schedule RC Packet"<<endl;
    delay(5);

    if(_waitForNeighborPacket(neighborID,header,true,timeout))                      //stays in loop until desired packet received
    {
        // Serial << "Received Schedule RC Packet from node " << neighborID<<endl;
        // delay(5);
    
        startTime   = _getStartTimeFromPacket();
        iterations  = _getIterationsFromPacket();
        period      = _getPeriodFromPacket();
        uint16_t start = millis();
        s->setStatus(neighborID, 3);
        counter++;

        if(counter==_G->getN())
        {
            Serial << "No Schedule RC ACK is required from neighbors"<<endl;
            delay(5);
            // Serial << "Sending Schedule RC ACK to parent"<<endl;
            // delay(5);
        
            while(true)
            {
                if(_waitForNeighborPacket(neighborID,header,true,SCHEDULE_TIMEOUT/4))                        //wait for acknowledgement packets
                {
                    delay(5);
                    _broadcastSchedulePacket(header,startTime,iterations,period);
                }
                else
                {
                    Serial << "Reset LinkedList!"<<endl;
                    delay(5);
                    l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                    return true;
                }
            }
        }

        // Serial << "Waiting for Schedule RC ACKs"<<endl;
        // delay(5);

        while(true)
        {
            _broadcastSchedulePacket(header,startTime,iterations,period);
            if(_waitForNeighborPacket(neighborID,header,true,200))                        //wait for acknowledgement packets
            {
                if(s->getStatus(neighborID) < 3)
                {
                 //    Serial << "received Schedule RC ACK from node " << neighborID<<endl;
                    // delay(5);
                    s->setStatus(neighborID, 3);
                    counter++;
                }

                if(counter==_G->getN())
                {
                    Serial << "All neighbors scheduled for ratio consensus algorithm"<<endl;
                    delay(5);
                    l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                    return true;
                }
            }
        }
    }
    else
        return false;
}

bool OAgent::_waitForParentSchedulePacketPD(unsigned long &startTime, uint16_t &iterations, int timeout) {
    uint8_t neighborID;
    uint16_t header = SCHEDULE_PD_HEADER;
    LinkedList * l = _G->getLinkedList();                                           //get pointer to linked list
    OLocalVertex * s = _G->getLocalVertex();                                        // store pointer to local vertex
    l->resetLinkedListStatus(s->getStatusP());                   //gets linkedlist and resets status of online neighbors to 2
    uint8_t counter = 1;

    Serial << "Waiting for Schedule PD Packet"<<endl;
    delay(5);

    if(_waitForNeighborPacket(neighborID,header,true,timeout))                      //stays in loop until desired packet received
    {
        Serial << "Received Schedule PD Packet from node " << neighborID<<endl;
        delay(5);
    
        startTime   = _getStartTimeFromPacket();
        iterations  = _getIterationsFromPacketPD();
        uint16_t start = millis();
        s->setStatus(neighborID, 3);
        counter++;

        if(counter==_G->getN())
        {
            Serial << "No Schedule PD ACK is required from neighbors"<<endl;
            delay(5);
            Serial << "Sending Schedule PD ACK to parent"<<endl;
            delay(5);
        
            while(true)
            {
                if(_waitForNeighborPacket(neighborID,header,true,SCHEDULE_TIMEOUT/2))                        //wait for acknowledgement packets
                {
                    delay(10);
                    _broadcastSchedulePacketPD(startTime,iterations);
                }
                else
                {
                    l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                    return true;
                }
            }
        }

        Serial << "Waiting for Schedule PD ACKs"<<endl;
        delay(5);

        while(true)
        {
            _broadcastSchedulePacketPD(startTime,iterations);
            if(_waitForNeighborPacket(neighborID,header,true,200))                        //wait for acknowledgement packets
            {
                if(s->getStatus(neighborID) < 3)
                {
                 //    Serial << "received Schedule PD ACK from node " << neighborID<<endl;
                    // delay(5);
                    s->setStatus(neighborID, 3);
                    counter++;
                }

                if(counter==_G->getN())
                {
                    Serial << "All neighbors scheduled for primal dual algorithm"<<endl;
                    delay(5);
                    l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                    return true;
                }
            }
        }
    }
    else
        return false;
}

bool OAgent::_waitForParentSchedulePacketED(unsigned long &startTime, uint16_t &iterations, int timeout) {
    uint8_t neighborID;
    uint16_t header = SCHEDULE_ED_HEADER;
    LinkedList * l = _G->getLinkedList();                                           //get pointer to linked list
    OLocalVertex * s = _G->getLocalVertex();                                        // store pointer to local vertex
    l->resetLinkedListStatus(s->getStatusP());                   //gets linkedlist and resets status of online neighbors to 2
    uint8_t counter = 1;

    Serial << "Waiting for Schedule ED Packet"<<endl;
    delay(5);

    if(_waitForNeighborPacket(neighborID,header,true,timeout))                      //stays in loop until desired packet received
    {
        Serial << "Received Schedule ED Packet from node " << neighborID<<endl;
        delay(5);
    
        startTime   = _getStartTimeFromPacket();
        iterations  = _getIterationsFromPacketPD();
        uint16_t start = millis();
        s->setStatus(neighborID, 3);
        counter++;

        if(counter==_G->getN())
        {
            Serial << "No Schedule ED ACK is required from neighbors"<<endl;
            delay(5);
            Serial << "Sending Schedule ED ACK to parent"<<endl;
            delay(5);
        
            while(true)
            {
                if(_waitForNeighborPacket(neighborID,header,true,SCHEDULE_TIMEOUT/2))                        //wait for acknowledgement packets
                {
                    delay(10);
                    _broadcastSchedulePacketED(startTime,iterations);
                }
                else
                {
                    l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                    return true;
                }
            }
        }

        Serial << "Waiting for Schedule ED ACKs"<<endl;
        delay(5);

        while(true)
        {
            _broadcastSchedulePacketED(startTime,iterations);
            if(_waitForNeighborPacket(neighborID,header,true,200))                        //wait for acknowledgement packets
            {
                if(s->getStatus(neighborID) < 3)
                {
                 //    Serial << "received Schedule ED ACK from node " << neighborID<<endl;
                    // delay(5);
                    s->setStatus(neighborID, 3);
                    counter++;
                }

                if(counter==_G->getN())
                {
                    Serial << "All neighbors scheduled for economic dispatch algorithm"<<endl;
                    delay(5);
                    l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                    return true;
                }
            }
        }
    }
    else
        return false;
}

// uint16_t OAgent::_waitForSchedulePacket(unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout) {
//     uint16_t rsp = _waitForValidPacket(true,timeout);
//     if(rsp != 0x0) {
//         startTime   = _getStartTimeFromPacket();
//         iterations  = _getIterationsFromPacket();
//         period      = _getPeriodFromPacket();    
//         return rsp;
//     }
//     return 0x0;
// }

void OAgent::_broadcastACKPacket(uint16_t header, uint8_t recipientID)
{
    uint8_t payload[4];
    // put header in payload array
    payload[0] = header;
    payload[1] = header >> 8;
    payload[2] = recipientID;
    payload[3] = recipientID >> 8;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload));
    _xbee->send(_zbTx);
}

void OAgent::_broadcastHeaderPacket(uint16_t header)
{
    uint8_t payload[2];
    // put header in payload array
    payload[0] = header;
    payload[1] = header >> 8;

    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload));
    _xbee->send(_zbTx);
}

void OAgent::_waitForACKPacket(uint16_t header, unsigned long t0, unsigned long startTime, uint8_t iterations, uint16_t period)
{   
    int nodes = 9; //number of online neighbors in the network
    int counter = 0;
    bool check = false;
    unsigned long start = t0;
    uint8_t id;

    // int node_array[nodes];

    // for(int i = 0;i < nodes;i++)
    // {
    //     node_array[i] = 0;       // initialize all entries in array to zero
    // }


    while(uint16_t(millis()-t0) < 600 && counter < nodes )  //change parameters globally
    {
        while(uint16_t(millis()-start) < 250)
        {
            if(_waitForPacket(header,true,50))
            {
                id = _getIDFromPacket();
                counter++;
                /*
                check = true;
                
                for(int k=0; k< counter;k++)
                {
                    if(node_array[k] == id)
                        check = false;       //meaning the node with that id number has already been accepted
                }
                if(check)
                {
                    node_array[counter] = id;
                    counter++;
                }
                */
            }
            
        }

        if(counter == nodes)
        {
            break;
        }
        
        _broadcastScheduleFairSplitPacket(startTime,iterations,period);
        start = millis();
    }
   
}

//Resilient Version
bool OAgent::_waitForChildSchedulePacketRC(uint16_t header, int timeout, unsigned long startTime, uint8_t iterations, uint16_t period  ) { 
    unsigned long start = millis();
    OLocalVertex * s = _G->getLocalVertex(); // store pointer to local vertex 
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());                   //gets linkedlist and resets status of online neighbors to 2     

    uint8_t counter = 1;    
    uint8_t neighborID;
    
    _broadcastSchedulePacket(header,startTime,iterations,period);


    Serial << "Waiting for Schedule Ratio Consensus packet"<<endl;
    delay(5);    

    while (uint16_t(millis()-start) < timeout)
    {
       if(_waitForNeighborPacket(neighborID,header,true,100))
        {
            if(s->getStatus(neighborID) < 3)
            {
                // Serial << "received Schedule RC ACK from node " << neighborID<<endl;
                // delay(5);
                s->setStatus(neighborID, 3);
                counter++;
            }

            if(counter==_G->getN())
            {
                Serial << "All neighbors scheduled for ratio consensus algorithm"<<endl;
                delay(5);
                l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                return true;
            }
        }
        _broadcastSchedulePacket(header,startTime,iterations,period);
        //Serial << "received acknowledgements from " << counter << " nodes"<<endl;
        //Serial << "Number of neighbors is " << _G->getN()<<endl;
    }
    return false;
}

// Primal Dual Algorithm
bool OAgent::_waitForChildSchedulePacketPD(int timeout, unsigned long startTime, uint16_t iterations)  {
    unsigned long start = millis();
    unsigned long restart = start;
    OLocalVertex * s = _G->getLocalVertex();                                                            // store pointer to local vertex 
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());                   //gets linkedlist and resets status of online neighbors to 2     

    uint8_t counter = 1;    
    uint8_t neighborID;

    _broadcastSchedulePacketPD(startTime,iterations);

    Serial << "Waiting for Schedule PD ACKs"<<endl;
    delay(5);
    
    while(uint16_t(millis()-start) < timeout)
    {
        if(_waitForNeighborPacket(neighborID,SCHEDULE_PD_HEADER,true,200))                        //wait for acknowledgement packets
        {
            if(s->getStatus(neighborID) < 3)
            {
                Serial << "received Schedule PD ACK from node " << neighborID<<endl;
                delay(5);
                s->setStatus(neighborID, 3);
                counter++;
            }

            if(counter==_G->getN())
            {
                Serial << "All neighbors scheduled for primal dual algorithm"<<endl;
                delay(5);
                l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                return true;
            }
        }
        _broadcastSchedulePacketPD(startTime,iterations);
    }
    return false;
}

// Economic Dispatch Algorithm
bool OAgent::_waitForChildSchedulePacketED(int timeout, unsigned long startTime, uint16_t iterations)  {
    unsigned long start = millis();
    OLocalVertex * s = _G->getLocalVertex();                                                            // store pointer to local vertex 
    LinkedList * l = _G->getLinkedList();
    l->resetLinkedListStatus(s->getStatusP());                   //gets linkedlist and resets status of online neighbors to 2     

    uint8_t counter = 1;    
    uint8_t neighborID;

    _broadcastSchedulePacketED(startTime,iterations);

    Serial << "Waiting for Schedule ED ACKs"<<endl;
    delay(5);
    
    while(uint16_t(millis()-start) < timeout)
    {
        if(_waitForNeighborPacket(neighborID,SCHEDULE_ED_HEADER,true,200))                        //wait for acknowledgement packets
        {
            if(s->getStatus(neighborID) < 3)
            {
                // Serial << "received Schedule ED ACK from node " << neighborID<<endl;
                // delay(5);
                s->setStatus(neighborID, 3);
                counter++;
            }

            if(counter==_G->getN())
            {
                Serial << "All neighbors scheduled for economic dispatch algorithm"<<endl;
                delay(5);
                l->resetLinkedListStatus(s->getStatusP());                                      //gets linkedlist and resets status of online neighbors to 2
                return true;
            }
        }
        _broadcastSchedulePacketED(startTime,iterations);
    }
    return false;
}

uint32_t OAgent::_getAvailableAgentLsb(uint8_t i) {
    return _availableAgentLsb[i-1];
}

uint8_t OAgent::_getUint8_tFromPacket(uint8_t &byteNumber) {
    byteNumber++;
    return _rx->getData(byteNumber-1);
}

uint32_t OAgent::_getUint32_tFromPacket(uint8_t &lsbByteNumber) {
    lsbByteNumber += 4;
    return (uint32_t(_rx->getData(lsbByteNumber-1)) << 24) + (uint32_t(_rx->getData(lsbByteNumber-2)) << 16) + (uint16_t(_rx->getData(lsbByteNumber-3)) << 8) + _rx->getData(lsbByteNumber-4);
}

//  long OAgent::_getLongFromPacket(uint8_t &lsbByteNumber) {
//  return long(_getUint32_tFromPacket(lsbByteNumber));
//}
/// End general coordination helper functions

/// Synchronization helper functions
bool OAgent::_leaderSync() {
    // only attempt to sync if there is at least one other vertex
    //Serial << "i AM HERE";
    if(_G->getN() > 1) 
    {
        // seed random number generator with millis
        srand(millis());
        // get the index of a neighbor at random                  //getN() returns the number of nodes in the network
        uint8_t *p = (_G->getLocalVertex())->getStatusP();
        for (int i = 0; i < NUM_REMOTE_VERTICES; i++)
        {
            if(*(p+i)==2)
            {
                // broadcast sync begin packet
                //Serial << i+1 <<" is the neighbor chosen\n";
                unsigned long tOne = _broadcastSyncBeginPacket(i+1);
                // variable to store receive time of final packet
                unsigned long tFour;
                // wait until sync response packet arrives or timeout
                if(_waitForSyncResponsePacket(tFour))
                {
                    //Serial << "Unicast Response Received\n";
                    uint8_t ptr = 2;
                    unsigned long tTwo = _getUint32_tFromPacket(ptr);
                    long d = tTwo + _getUint32_tFromPacket(ptr) - tOne - tFour;
                    // compute error between target and base
                    d = float(d)/float(2);
                    // broadcast final packet
                    _broadcastSyncFinalPacket(tTwo,d);
                }
            }
        }
        return true;
    }
    return false;
}

bool OAgent::_targetSync(unsigned long tTwo) {
    if(_unicastSyncResponsePacket(tTwo))
    {
        //Serial << "Unicast Response Sent\n";
        if(_waitForSyncFinalPacket(SYNC_TIMEOUT))
        {
            // T = t + d
            uint8_t ptr = 6;
            _offset = _getLongFromPacket(ptr) + SYNC_ERROR;
            _synced = true;
            return true;        
        }
    }
    return false;
}


bool OAgent::_nonTargetSync(unsigned long tTwo) {
    if(_waitForSyncFinalPacket(SYNC_TIMEOUT+ACK_TIMEOUT)) {
        // T = t + d + t2 - t2'
        uint8_t ptr = 6;
        long d = _getLongFromPacket(ptr);
        ptr = 2;
        _offset = d + tTwo - _getUint32_tFromPacket(ptr) + SYNC_ERROR;
        _synced = true;
        return true;
    }
    return false;
}

unsigned long OAgent::_broadcastSyncBeginPacket(uint8_t i) {
    uint16_t payload[3];
    // put header in bytes 0 and 1
    payload[0] = SYNC_BEGIN_HEADER;
    // put lsb of address in bytes 2-5
    _addUint32_tToPayload(_G->getVertexByUniqueID(i)->getAddressLsb(),((uint8_t * )(&payload)),2);
    // prepare zbTx object with broadcast address, pointer to payload and number of bytes in payload
    _zbTx = ZBTxRequest(_broadcastAddress,((uint8_t *)(&payload)),sizeof(payload));
    // send packet without timestamp and return time packet was sent
    return _xbee->sendTwo(_zbTx,false,true);
}

void OAgent::_broadcastSyncFinalPacket(unsigned long tTwo, long d) {
    uint16_t payload[5];
    // put sync final header in bytes 0 and 1
    payload[0] = SYNC_FINAL_HEADER;
    // put tTwo into bytes 2-5
    _addUint32_tToPayload(uint32_t(tTwo),((uint8_t * )(&payload)),2);
    // put offset into bytes 6-9
    _addUint32_tToPayload(uint32_t(d),((uint8_t * )(&payload)),6);
    // prepare zbTx object with broadcast address, pointer to payload and number of bytes in payload
    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload));
    _xbee->send(_zbTx); // transmit
}

bool OAgent::_unicastSyncResponsePacket(unsigned long tTwo) {
    uint16_t payload[3];
    // put sync response header in bytes 0 and 1
    payload[0] = SYNC_RESPONSE_HEADER;
    // put receive time into bytes
    _addUint32_tToPayload(uint32_t(tTwo),((uint8_t * )(&payload)),2);
    _zbTx = ZBTxRequest(_rx->getRemoteAddress64(), ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    _xbee->sendTwo(_zbTx,true); // transmit
    return _packetACKed(ACK_TIMEOUT);
}

bool OAgent::_isTargetNode() {
    uint8_t ptr = 2;
    
    return _G->isLocalVertex(_getUint32_tFromPacket(ptr));
}

uint8_t OAgent:: getStatusData(uint8_t neighborID)
{
     OLocalVertex * s = _G->getLocalVertex();
     return s->getStatus(neighborID - 1); 
}

uint8_t OAgent::_addUint32_tToPayload(uint32_t data, uint8_t payload[], uint8_t ptr) {
    //Serial << "byte to payload: " << _DEC(data) << endl;
    payload[ptr]    = data;
    payload[ptr+1]  = data >> 8;
    payload[ptr+2]  = data >> 16;
    payload[ptr+3]  = data >> 24;
    return ptr + 4;
}

/// End synchronization helper functions
/// General helper functions

void OAgent::_prepareOAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader, bool quiet) {
    _xbee = xbee;
    _G = G;
    _leader = leader;
    _quiet = quiet;
    _synced = false;
    _offset = 0;
    _windowsPerPeriod = 0;
    _zbTx = ZBTxRequest();
    _rx = rx;
    _txStatus = ZBTxStatusResponse();
    _broadcastAddress = XBeeAddress64(0x0, BROADCAST_ADDRESS);
    _aMsb = 0x0013A200;
    _availableAgentLsb[0] = 0x404980AE;
    _availableAgentLsb[1] = 0x40498074;
    _availableAgentLsb[2] = 0x40498060;
    _availableAgentLsb[3] = 0x40498065;
    _availableAgentLsb[4] = 0x405D9805;
    _availableAgentLsb[5] = 0x405D9822;
    _availableAgentLsb[6] = 0x4068B69B;
    _availableAgentLsb[7] = 0x405D980A;

    for(int i =0; i < 200; i++)
    {

        _buffer[i] = 0.0;
    }

    for(int i=0; i < NUM_REMOTE_VERTICES; i++)
    {
        node_counter[i] = -1;                     // -1 indicates an offline link status, so we assume initially an offline link status with all other noodes (Note: status of node itself isn't important)
    } 

}
/// End general helper functions

float OAgent::VoltageControl( float V, float theta, float q, float qrise, float qlower, float D, float P, float secPercentage )  //it is going to give back the q required to rise or lower
    {
      OLocalVertex * s = _G->getLocalVertex();  
      _initializeVoltageControl(s,V,theta,q,qrise,qlower,D,P,secPercentage); 
      






        
    }
void OAgent::_initializeVoltageControl( OLocalVertex * s, float V, float theta, float q, float qrise, float qlower, float D, float P, float securityPercent )
    {
    _G->clearAllStates(); 

    s->setVoltage(V - getMin());
    s->setTheta(theta - getMin());
    s->setQtotal(q - getMin());
    s->setQrise(qrise - getMin());
    s->setQlower(qlower - getMin());
    s->setPtotal(P- getMin());
    s->setD(D - getMin());
    s->setVref(float(1));

    //check for overvolrage and
    isOverVoltage(s, securityPercent);
    isUnderVoltage(s, securityPercent);

    }

void OAgent::isOverVoltage(OLocalVertex * s, float secPercentage)
    {
        if(V > (s->getVref() + (s->getVref() * secPercentage/float (100))))
        {
            uint8_t ID = s->getID();
            Serial<<"node "<<ID<<"is working with over-voltage condition"; 
            setStateOver(true); 
            return true;   

        }else
        {
            return false;
        }
    }

void OAgent::isUnderVoltage(OLocalVertex * s, float secPercentage)
    {
        if(V < (s->getVref() - (s->getVref() * secPercentage/float (100))))
        {
            uint8_t ID = s->getID();
            Serial<<"node "<<ID<<"is working with under-voltage condition";  
            setStateUnde(true);
            return true;   

        }else
        {
            return false;
        }
    }

