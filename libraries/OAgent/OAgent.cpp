/*
 *  Agent.cpp
 *  
 *
 *  Created by Stanton T. Cady on 06/15/12
 *  Edited by Laura Haller on 01/11/13
 *  Last edited by Stanton T. Cady on 01/16/13
 *
 */



#include "OAgent.h"
#include "Streaming.h"
//#define VERBOSE


//// Public methods
/// Constructors
OAgent::OAgent() {
    _prepareOAgent(&XBee(),&ZBRxResponse(),&OGraph());
}

OAgent::OAgent(XBee * xbee, OGraph * G, bool leader, bool quiet) {
    _prepareOAgent(xbee,&ZBRxResponse(),G,leader,quiet);
}

OAgent::OAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader, bool quiet) {
    _prepareOAgent(xbee,rx,G,leader,quiet);
}

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

float OAgent::fairSplitRatioConsensus(long x, uint8_t iterations, uint16_t period) {
    OLocalVertex * s = _G->getLocalVertex(); // store pointer to local vertex object///NO ENTIENDO ESTE TIPO DE PUNTEROS !!!
    float Dout = float(s->getOutDegree() + 1);    // store out degree 
    _initializeFairSplitting(s,x);      // initialize state variables                           
    unsigned long start;                // create variable to store iteration start time
    bool txDone;                        // create variable to keep track of broadcasts
    uint16_t txTime = _genTxTime(period,5,analogRead(0));   // get transmit time
    long inY;                           // incoming state variable
    long inZ;
    for(uint8_t k = 0; k < iterations; k++) {
        txDone = false;     // initialize toggle to keep track of broadcasts
        start = millis();   // initialize timer
        // clear in y and in z
        inY = 0;
        inZ = 0;
        while(uint16_t(millis()-start) < period) {
            if(_fairSplitPacketAvailable()) {                                   // robust, coordinate value packet available
                uint8_t i;                                                      // index of remote device in graph in-neighborhood
                if(_G->isInNeighbor(_rx->getRemoteAddress64().getLsb(),i)) {    // check if remote device is in in-neighborhood
                    long inMu = _getMuFromPacket();                             // store incoming value of mu
                    inY += inMu - s->getNuMin(i);                               // add mu from incoming device and subtract last received value
                    s->setNuMin(i,inMu);                                        // save received mu as new nu (nuMin)
                    long inSigma = _getSigmaFromPacket();                       // store incoming value of sigma
                    inZ += inSigma - s->getTau(i);                              // add sigma from incoming device and subtract last received value
                    s->setTau(i,inSigma);                                       // save received sigma as new tau
                }
            } else if((int((millis() - start)) >= txTime) && !txDone) {
                txDone = true; // toggle txDone
                _broadcastFairSplitPacket(s);
            }
            delay(5);
        }
        if(!_quiet) {
           // Serial << _MEM(PSTR("y(")) << (k+1) << _MEM(PSTR(") = ")) << s->getYMin() << _MEM(PSTR(";")) << endl;
            //Serial << _MEM(PSTR("z(")) << (k+1) << _MEM(PSTR(") = ")) << s->getZ() << _MEM(PSTR(";")) << endl;
            delay(10);
        } else {
            delay(25);
        }
        s->setYMin(long(float(s->getYMin())/Dout) + inY);
        s->setMuMin(s->getMuMin() + long(float(s->getYMin())/Dout));
        s->setZ(long(float(s->getZ())/Dout) + inZ);
        s->addToSigma(long(float(s->getZ())/Dout));
    }
    return float(s->getYMin())/float(s->getZ());
}

long OAgent::computeFairSplitFinalValue(float gamma) {
    OLocalVertex * s = _G->getLocalVertex();
	if(gamma <= 0)
		return s->getMin();
    else if(gamma < 1)
        // if under maximum system capacity
         return s->getMin() + long(gamma*float(s->getMax()-s->getMin()));
    else if(gamma >= 1)
        return s->getMax();    
}



// void OAgent::incrementalLeaderRatioConsensusWithDyno(Dyno &d, uint8_t iterations, uint16_t period, uint8_t &ledPin) {
//     _start_millis = millis() + 1200;
// 	OLocalVertex * s = _G->getLocalVertex();
// 	float base = float(s->getBase());
//     _broadcastScheduleFairSplitPacket(_start_millis,iterations,period);
// 	// get the torque command from python
//     float t = d.getTorqueFloat();
// 	float tErr = d.getErrorTorqueFloat();
// 	// scale torque to base and call it initial
// 	long error = long(tErr*base);
// 	long current = long(t*base);
// 	//d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
//     long final = initial;
//     float gamma = 0;
//     if(_waitToStart(_start_millis,false,1800)) {
// 		digitalWrite(ledPin,HIGH);
//         gamma = incrementalRatioConsensus(initial,iterations,period);
// 		d.sendShortLogData(LOG_GAMMA_KEY,uint16_t(float(1000)*gamma));
//         final = computeIncrementalFairSplitValue(gamma);
// 		
// 		digitalWrite(ledPin,LOW);
//     }
// 	d.setTorque(float(final)/base);
// }

// void OAgent::leaderDGC(Dyno &d, float k, int vref, uint8_t epsilon) {
//     // get pointer to local vertex
//     OLocalVertex * s = _G->getLocalVertex();
//     //long base = s->getBase();
//     long final;
//     int vError;
//     do {
//         // save start time
//         unsigned long startTime = millis() + 1200;
//         // broadcast schedule packet
//         _broadcastScheduleFairSplitPacket(startTime,50,50);
//         // get current torque command as integer
//         uint16_t t = d.getTorqueInt();
//         //uint16_t t = 218;
//         // get current velocity of dyno
//         uint16_t v = d.getVelocity();
//         //uint16_t v = 1203;
// 		// calculate estimated torque errpr
// 		long tError = long(k*float(vref-int(v)));
// 		d.sendLogData(LOG_TORQUE_ERROR_KEY,tError);
//         // determine initial command for algorithm
//         long initial = s->getBase()*(tError + long(t));
// 		d.sendLogData(LOG_INITIAL_TORQUE_KEY,initial);
//         // save initial value as final value in case _waitToStart times out
//         final = initial;
//         // create variable to store ratio
//         float gamma = 0;
//         if(_waitToStart(startTime,false,1800)) {
// 			digitalWrite(48,HIGH);
//             gamma = fairSplitRatioConsensus(initial,50,50);
// 			d.sendLogData(LOG_GAMMA_KEY,s->getBase()*gamma);
//             final = computeFairSplitFinalValue(gamma);
// 			digitalWrite(48,LOW);
//         }
//         // set torque
//         d.setTorqueInt(uint16_t(float(final)/float(s->getBase())));
//         delay(500);
//         // compute speed error
//         vError = vref - d.getVelocity();
//     } while(abs(vError) <= epsilon);
//     delay(7500);
//     final = leaderOptimalDispatch(final,50,250);
//     d.setTorqueInt(uint16_t(float(final)/float(s->getBase())));
// }

// void OAgent::nonleaderDGC() {
//     OLocalVertex * s = _G->getLocalVertex();
//     long initial;
//     long final;
//     unsigned long startTime;
//     uint8_t iterations;
//     uint16_t period;
//     uint16_t header = _waitForSchedulePacket(startTime,iterations,period);
// 	uint8_t cPin = 48;
// 	uint8_t oPin = 53;
// 	digitalWrite(cPin,HIGH);
// 	digitalWrite(oPin,HIGH);
//     //if(header == SCHEDULE_FAIR_SPLIT_HEADER || header == SCHEDULE_OPTIMAL_DISPATCH_HEADER) {
//         // get current torque command
//         //uint16_t t = d.getTorqueInt();
// 		//float t = 1.81;
//         //initial = long(t*float(s->getBase()));
// 		// d.sendLogData(LOG_INITIAL_TORQUE_KEY,initial);
// 		initial = long(75)*s->getBase();
//         final = initial;
//         if(header == SCHEDULE_FAIR_SPLIT_HEADER) {
// 			digitalWrite(oPin,LOW);
//             float gamma = 0;
//             if(_waitToStart(startTime,true,1800)) {
//                 gamma = fairSplitRatioConsensus(initial,iterations,period);
// 				//d.sendLogData(LOG_GAMMA_KEY,s->getBase()*gamma);
//                 final = computeFairSplitFinalValue(gamma);
//             }
// 			digitalWrite(cPin,LOW);
//         } else if(header == SCHEDULE_OPTIMAL_DISPATCH_HEADER) {
//             digitalWrite(cPin,LOW);
//             if(_waitToStart(startTime,true,1800)) {
//                 final = 0;//optimalDispatch(initial,iterations,period);
//             }
// 			digitalWrite(oPin,LOW);
//         }
// 		//if(final != initial)
// 		//	d.setTorqueInt(uint16_t(float(final)/float(s->getBase())));
// 		//}
// 	digitalWrite(cPin,LOW);
// 	digitalWrite(oPin,LOW);
// }

// void OAgent::reserveDGC(Dyno &d, bool &limit_exceeded) {
//     OLocalVertex * s = _G->getLocalVertex();
//     long initial;
//     long final;
//     unsigned long startTime;
//     uint8_t iterations;
//     uint16_t period;
//     uint16_t header = _waitForSchedulePacket(startTime,iterations,period);
//     if(header == SCHEDULE_FAIR_SPLIT_HEADER || header == SCHEDULE_OPTIMAL_DISPATCH_HEADER) {
//         // get current torque command
//         uint16_t t = d.getTorqueInt();
//         initial = long(t)*s->getBase();
// 		// d.sendLogData(LOG_INITIAL_TORQUE_KEY,initial);
//         final = initial;
//         if(header == SCHEDULE_FAIR_SPLIT_HEADER) {
// 			digitalWrite(48,HIGH);
//             float gamma = 0;
//             if(_waitToStart(startTime,true,1800)) {
//                 gamma = fairSplitRatioConsensus(initial,iterations,period);
// 				// d.sendLogData(LOG_GAMMA_KEY,s->getBase()*gamma);
//                 final = computeFairSplitFinalValue(gamma);
//             }
// 			digitalWrite(48,LOW);
//         } else if(header == SCHEDULE_OPTIMAL_DISPATCH_HEADER) {
//             digitalWrite(51,HIGH);
//             if(_waitToStart(startTime,true,1800)) {
//                 final = optimalDispatch(initial,iterations,period);
//             }
// 			digitalWrite(51,LOW);
//         }
// 		if(final != initial)
// 			d.setTorqueInt(uint16_t(float(final)/float(s->getBase())));
//     }
// }

long OAgent::leaderFairSplitRatioConsensus(long initial, uint8_t iterations, uint16_t period) {
    unsigned long startTime = millis() + 1200;
    
    _broadcastScheduleFairSplitPacket(startTime,iterations,period);
    long final = initial;
    float gamma = 0;
    if(_waitToStart(startTime,false,1800)) {
        gamma = fairSplitRatioConsensus(initial,iterations,period);
        final = computeFairSplitFinalValue(gamma);
    }
    return final;
}

void OAgent::leaderFairSplitRatioConsensusWithDyno(Dyno &d, uint8_t iterations, uint16_t period, uint8_t &ledPin) {
    _start_millis = millis() + 1750;
	OLocalVertex * s = _G->getLocalVertex();
	float base = float(s->getBase());
    _broadcastScheduleFairSplitPacket(_start_millis,iterations,period);
	// get the torque command from python
    float t = d.getTorqueFloat();
	// scale torque to base and call it initial
	long initial = long(t*base);
	//d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
    long final = initial;
    float gamma = 0;
    if(_waitToStart(_start_millis,false,2000)) {
		digitalWrite(ledPin,HIGH);
        gamma = fairSplitRatioConsensus(initial,iterations,period);
		d.sendShortLogData(LOG_GAMMA_KEY,uint16_t(float(1000)*gamma));
        final = computeFairSplitFinalValue(gamma);
		digitalWrite(ledPin,LOW);
    }
	d.setTorque(float(final)/base);
}

long OAgent::nonleaderFairSplitRatioConsensus(long initial) {
    unsigned long startTime;
    uint8_t iterations;
    uint16_t period;
    _waitForScheduleFairSplitPacket(startTime,iterations,period);
    digitalWrite(48,HIGH);
    long final = initial;
    float gamma = 0;
    if(_waitToStart(startTime,true,1800)) {
        gamma = fairSplitRatioConsensus(initial,iterations,period);
        final = computeFairSplitFinalValue(gamma);
    }
    digitalWrite(48,LOW);
    return final;
}

void OAgent::nonleaderFairSplitRatioConsensusWithDyno(Dyno &d, uint8_t &ledPin) {
    // get pointer to local vertex
    OLocalVertex * s = _G->getLocalVertex();
	float base = s->getBase();
    // wait for schedule packet
    _waitForScheduleFairSplitPacket(_start_millis,_iterations,_period);
    // get current torque command
	float t = d.getTorqueFloat();
	long initial = long(t*base);
	//d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
    long final = initial;
    float gamma = 0;
    if(_waitToStart(_start_millis,true,2000)) {
		digitalWrite(ledPin,HIGH);
        gamma = fairSplitRatioConsensus(initial,_iterations,_period);
		d.sendShortLogData(LOG_GAMMA_KEY,uint16_t(float(1000)*gamma));
        final = computeFairSplitFinalValue(gamma);
		digitalWrite(ledPin,LOW);
    }
	d.setTorque(float(final)/base);
}

void OAgent::reserveFairSplitRatioConsensusWithDyno(Dyno &d, long trueMax, uint8_t &ledPin) {
    // get pointer to local vertex
    OLocalVertex * s = _G->getLocalVertex();
	float base = s->getBase();
    // wait for schedule packet
    _waitForScheduleFairSplitPacket(_start_millis,_iterations,_period);
    // get current torque command
	float t = d.getTorqueFloat();
	long initial = long(t*base);
	//d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
    long final = initial;
    float gamma = 0;
    if(_waitToStart(_start_millis,true,2000)) {
		digitalWrite(ledPin,HIGH);
        gamma = fairSplitRatioConsensus(initial,_iterations,_period);
		d.sendShortLogData(LOG_GAMMA_KEY,uint16_t(float(1000)*gamma));
        final = computeFairSplitFinalValue(gamma);
		if(s->getMax() != trueMax && gamma > 1)
			s->setMax(trueMax);
		digitalWrite(ledPin,LOW);
    }
	d.setTorque(float(final)/base);
}

//void OAgent::reserveFairSplitRatioConsensusWithDyno(Dyno &d, uint8_t &ledPin) {
	//     // get pointer to local vertex
	//     OLocalReserveVertex * s = dynamic_cast<OLocalReserveVertex*> (_G->getLocalVertex());
	// float base = s->getBase();
	//     // wait for schedule packet
	//     _waitForScheduleFairSplitPacket(_start_millis,_iterations,_period);
	//     // get current torque command
	// float t = d.getTorqueFloat();
	// long initial = long(t*base);
	// d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
	//     long final = initial;
	//     float gamma = 0;
	//     if(_waitToStart(_start_millis,true,1800)) {
	// 	digitalWrite(ledPin,HIGH);
	//         gamma = fairSplitRatioConsensus(initial,_iterations,_period);
	// 	d.sendShortLogData(LOG_GAMMA_KEY,uint16_t(float(100)*gamma));
	// 	final = computeFairSplitFinalValue(gamma);
	// 	// if gamma is bigger than 1 and the system limit has not been exceeded
	// 	if(gamma > 1 && s->getLimitExceeded() == false) {
	// 		// change to true limit
	// 	}
	// 	// if the system limit has been exceeded and the limit gamma has not been set, save it
	// 	if(s->getLimitGamma() == 0 && s->getLimitExceeded() == true)
	// 		s->setLimitGamma(gamma);
	// 	// if we are near the limit gamma
	// 	if(gamma <= (s->getLimitGamma()-0.05) && s->getLimitExceeded() == true) {
	// 		// reset limit gamma and the limit exceeded flag
	// 		s->setLimitGamma(0);
	// 		s->setLimitExceeded(false);
	// 		// restore to fake limit
	// 	}	
	// 	digitalWrite(ledPin,LOW);
	//     }
	// d.setTorque(float(final)/base);
	//}


// End fair splitting
/// End ratio-consensus
/// Optimal dispatch
long OAgent::optimalDispatch(long x, uint8_t iterations, uint16_t period) {
 	OLocalVertex * s = _G->getLocalVertex();           // store pointer to local vertex object
    uint8_t Dout = s->getOutDegree() + 1;       // store out degree
    //Serial << "Out-degree: " << _DEC(Dout) << endl;
    _initializeOptimalDispatch(s,x);            // initialize state variables
    delay(20);
    bool txDone;                                // create variable to keep track of broadcasts
    uint16_t txTime = _genTxTime(period,5);     // time to broadcast packet
    for(uint8_t k = 0; k < iterations; k++) {
        // initialize toggle to keep track of broadcasts
        txDone = false;  
        // initialize timer   
        _start_millis = millis();   
        // do for iteration period
        while(uint16_t(millis()-_start_millis) < period) {
            // look for optimal dispatch packet
            if(_optimalDispatchPacketAvailable()) {
            	// index of in-neighbor packet received from
            	uint8_t i;
                // check if sender is in-neighbor
                uint32_t rv = _rx->getRemoteAddress64().getLsb();
                if(_G->isInNeighbor(rv,i)) {
                    // process optimal dispatch packet
					//Serial << _MEM(PSTR("packet from: ")) << _DEC(i) << endl;
                    _processOptimalDispatchPacket(s,_G->getRemoteVertex(i)->getIndex());
                }
            // check if time to transmit and if transmit has already occured
            } else if(_timeToTransmit(_start_millis,txTime) && !txDone) {
                // toggle txDone
                txDone = true; 
                // broadcast optimal dispatch packet
                _broadcastOptimalDispatchPacket(s);
            }
            delay(5);
        }
#ifdef VERBOSE
        if(!_quiet) {
            _printStates(s,k,true);
            //delay(30);
        } else {
            delay(15);
        }
#endif
		delay(15);
        // update all states (z = z/Dout + zIn, etc)
        _updateOptimalDispatchStates(s,Dout);
        // clear all incoming states before next iteration
        _G->clearAllInStates();
    }
#ifdef VERBOSE
    uint8_t states = 2*(_G->getN());
    float ratios[states];
    _computeFinalOptimalDispatchRatios(s,ratios);
    if(!_quiet)
        _printRatios(ratios,states);
	_printStates(s,iterations,false);
	_printLambdas(s);
#endif
    long lambdaStar = _findLambdaStar(s);
	delay(15);
    //Serial << _MEM(PSTR("lambda* = ")) << _DEC(lambdaStar) << endl;
    return s->g(lambdaStar);
}

long OAgent::optimalDispatchWithDyno(long x, uint8_t iterations, uint16_t period, Dyno &d) {
 	OLocalVertex * s = _G->getLocalVertex();           // store pointer to local vertex object
    uint8_t Dout = s->getOutDegree() + 1;       // store out degree
    //Serial << "Out-degree: " << _DEC(Dout) << endl;
    _initializeOptimalDispatch(s,x);            // initialize state variables
    delay(20);
    bool txDone;                                // create variable to keep track of broadcasts
    uint16_t txTime = _genTxTime(period,5);     // time to broadcast packet
    for(uint8_t k = 0; k < iterations; k++) {
        // initialize toggle to keep track of broadcasts
        txDone = false;  
        // initialize timer   
        _start_millis = millis();   
        // do for iteration period
        while(uint16_t(millis()-_start_millis) < period) {
            // look for optimal dispatch packet
            if(_optimalDispatchPacketAvailable()) {
            	// index of in-neighbor packet received from
            	uint8_t i;
                // check if sender is in-neighbor
                uint32_t rv = _rx->getRemoteAddress64().getLsb();
                if(_G->isInNeighbor(rv,i)) {
                    // process optimal dispatch packet
                    _processOptimalDispatchPacket(s,_G->getRemoteVertex(i)->getIndex());
                }
            // check if time to transmit and if transmit has already occured
            } else if(_timeToTransmit(_start_millis,txTime) && !txDone) {
                // toggle txDone
                txDone = true; 
                // broadcast optimal dispatch packet
                _broadcastOptimalDispatchPacket(s);
            }
            delay(5);
        }
#ifdef VERBOSE
        if(!_quiet) {
            _printStates(s,k,false);
            //delay(30);
        } else {
            delay(15);
        }
#endif
		delay(15);
        // update all states (z = z/Dout + zIn, etc)
        _updateOptimalDispatchStates(s,Dout);
        // clear all incoming states before next iteration
        _G->clearAllInStates();
    }
#ifdef VERBOSE
	uint8_t states = 2*(_G->getN());
    float ratios[states];
    _computeFinalOptimalDispatchRatios(s,ratios);
    /if(!_quiet)
        _printRatios(ratios,states);
#endif
    long lambdaStar = _findLambdaStar(s);
	delay(15);
    //Serial << _MEM(PSTR("lambda* = ")) << _DEC(lambdaStar) << endl;
	d.sendShortLogData(LOG_LAMBDA_KEY,uint16_t(float(lambdaStar)/float(100)));
    return s->g(lambdaStar);
}

long OAgent::leaderOptimalDispatch(long initial, uint8_t iterations, uint16_t period, uint8_t &ledPin) {
    _start_millis = millis() + 1750;
    _broadcastScheduleOptimalDispatchPacket(_start_millis,iterations,period);
	long final = initial;
    if(_waitToStart(_start_millis,false,2000)) {
		digitalWrite(ledPin,HIGH);
        final = optimalDispatch(initial,iterations,period);
		digitalWrite(ledPin,LOW);
    }
	return final;
}

void OAgent::leaderOptimalDispatchWithDyno(Dyno &d, uint8_t iterations, uint16_t period, uint8_t &ledPin) {
    _start_millis = millis() + 1750;
	OLocalVertex * s = _G->getLocalVertex();
	// save base as floating point number
	float base = float(s->getBase());
     _broadcastScheduleOptimalDispatchPacket(_start_millis,iterations,period);
	// get the torque command from python
	float t = d.getOTorqueFloat();
	//float t = 1.12;
	// scale toruqe to base and call it initial
	//long initial = long(float(s->getBase())*t);
	long initial = long(t*base);
	d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
    long final = initial;
    if(_waitToStart(_start_millis,false,2000)) {
		digitalWrite(ledPin,HIGH);
		//Serial << "initial: " << _DEC(initial) << endl;
        final = optimalDispatchWithDyno(initial,iterations,period,d);
		digitalWrite(ledPin,LOW);
    }
	d.setTorque(float(final)/base);
}

long OAgent::nonleaderOptimalDispatch(long initial, uint8_t &ledPin) {
    uint8_t iterations;
    uint16_t period;
    _waitForScheduleOptimalDispatchPacket(_start_millis,iterations,period);
    //Serial << "start: " << _DEC(startTime) << ", myMillis: " << _DEC(myMillis()) << endl;
    long final = initial;
    if(_waitToStart(_start_millis,true,2000)) {
		digitalWrite(ledPin,HIGH);
        final = optimalDispatch(initial,iterations,period);
		digitalWrite(ledPin,LOW);
    }
	return final;
}

void OAgent::nonleaderOptimalDispatchWithDyno(Dyno &d, uint8_t &ledPin) {	
    OLocalVertex * s = _G->getLocalVertex();
	// save base as floating point number
	float base = float(s->getBase());
    _waitForScheduleOptimalDispatchPacket(_start_millis,_iterations,_period);
    // get current torque command
    float t = d.getTorqueFloat();
    long initial = long(t*base);
	d.sendShortLogData(LOG_INITIAL_TORQUE_KEY,uint16_t(t*float(100)));
    long final = initial;
    if(_waitToStart(_start_millis,true,2000)) {
		digitalWrite(ledPin,HIGH);
		//Serial << "initial: " << _DEC(initial) << endl;
        final = optimalDispatchWithDyno(initial,_iterations,_period,d);
		digitalWrite(ledPin,LOW);
    }
    d.setTorque(float(final)/base);
}

/// End optimal dispatch
/// Synchronization methods
bool OAgent::sync(uint8_t attempts) {
    if(_leader) {
		for(uint8_t i = 0; i < attempts; i++) {
			if(_leaderSync()) {
                _synced = true;
				return true;
			}
			delay(SYNC_RETRY_PERIOD);
		}    
    } else {
    	unsigned long tTwo;
    	if(_waitForSyncBeginPacket(tTwo)) {
    		if(_isTargetNode())
    			return _targetSync(tTwo);
    		else
    			return _nonTargetSync(tTwo);
    	}
    }
    return false;
}

unsigned long OAgent::myMillis() { 
	return millis() - (unsigned long)_offset; 
}

//bool OAgent::isSynced() {
//    return _synced;
//}
//
//unsigned long OAgent::myMillis() {
//    return millis() - (unsigned long)_offset;
//}
/// End synchronization methods
//// End public methods
//// Private Methods
/// General XBee methods

/// Fair splitting ratio-consensus methods
//void OAgent::_waitForScheduleFairSplitPacket(unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout) {
//    _waitForSchedulePacket(SCHEDULE_FAIR_SPLIT_HEADER,startTime,iterations,period,timeout);
//}
//
//void OAgent::_broadcastScheduleFairSplitPacket(unsigned long startTime, uint8_t iterations, uint16_t period) {
//    _broadcastSchedulePacket(SCHEDULE_FAIR_SPLIT_HEADER,startTime,iterations,period);
//}
//
//bool OAgent::_fairSplitPacketAvailable() {
//    return _packetAvailable(FAIR_SPLITTING_HEADER,true);
//}

void OAgent::_initializeFairSplitting(OLocalVertex * s, long x) {
    _G->clearAllStates();                   // clear everything
    uint8_t Dout = s->getOutDegree() + 1;   // store out degree
    s->setYMin(x - s->getMin());            // set initial y value (using yMin) [x - min]
    s->setMuMin(s->getYMin()/Dout);         // Initialize mu = y/Dout
    s->setZ(s->getMax() - s->getMin());     // set initial z value [max - min]
    s->setSigma(s->getZ()/Dout);            // Initialize sigma = z/Dout
}

void OAgent::_broadcastFairSplitPacket(OLocalVertex * s) {
    uint16_t payload[5];           
    long mu    = s->getMuMin(); //definimos una variable mu y le asignamos el valor de mumin que cogemos de la función gatMuMin 
    long sigma = s->getSigma();// definimos la variable sigma y le asignamos el valor que cogemos de la funcion get sigma
    payload[0] = FAIR_SPLITTING_HEADER; // en el primer elemento del vector guardamos este valor predefinido //titulo de la informacion
    payload[1] = mu; // guardamos el valor de mu los primeros?? 16 bits
    payload[2] = mu >> 16;// guardamos el valor de mu los ultimos?? 16 bits
    payload[3] = sigma;// guardamos el valor de sigma los ultimos?? 16 bits
    payload[4] = sigma >> 16;// guardamos el valor de sigma los ultimos?? 16 bits
    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), sizeof(payload)); // create zigbee transmit class
    unsigned long txTime = _xbee->sendTwo(_zbTx,false,true); // transmit with time stamp
#ifdef VERBOSE
    Serial << _MEM(PSTR("Transmit time: ")) << txTime << endl;
#endif
}
//funcion patricia

long OAgent::_getMuFromPacket() {
    uint8_t ptr = 2;
    return _getUint32_tFromPacket(ptr);
}

long OAgent::_getSigmaFromPacket() {
    uint8_t ptr = 6;
    return _getUint32_tFromPacket(ptr);
}
/// End fair splitting ratio-consensus methods

/// Optimal dispatch methods
void OAgent::_initializeOptimalDispatch(OLocalVertex * s, long x) {
    // clear all algorithm states (yMin, yMax, z)
    // clear all robust states (nuMin, nuMax, tau)
    // clear all incoming states (yMinIn, yMaxIn and zIn)
    // clear all broadcast states (muMin, muMax and sigma)
    // clear all remote vertex lambdaMin and lambdaMax
    _G->clearAllStates();
    // initialize ymin and ymax of local vertex
    s->initializeYMinYMax();
    // initialize muMin and muMax of local vertex
    s->initializeMuMinMuMax();
    // initialize z     
    s->setZ(x);
    // initialize sigma
    s->initializeSigma();
	// cheat by initializing assuming we have all lambdas
	for(uint8_t i=1; i < _G->getN(); i++) {
		OVertex * r = _G->getVertexByUniqueID(i);
		//Serial << _DEC(r->getLambdaMin()) << endl;
		//Serial << _DEC(r->getLambdaMax()) << endl;
		r->setYMinYMax(s->g(r->getLambdaMin()),s->g(r->getLambdaMax()));
		r->setMuMinMuMax(r->getYMin()/(s->getOutDegree()+1),r->getYMax()/(s->getOutDegree()+1));
	}
}

void OAgent::_updateOptimalDispatchStates(OLocalVertex * s, uint8_t Dout) {
    // update 
    s->updateZ();
    s->updateSigma();
    // update yMin and yMax for each remote vertex
    for(uint8_t i = 0; i < _G->getN(); i++) {
    	_G->getVertexByUniqueID(i)->updateYMinYMax(Dout);
    	_G->getVertexByUniqueID(i)->updateMuMinMuMax(Dout);
    }
}

long OAgent::_findLambdaStar(OLocalVertex * s) {
    float min = 0;
    float max = 1000000;
    long lambdaMinus;
    long lambdaPlus;
	//float base = s->getBase();
    long z = s->getZ();
    for(uint8_t i = 0; i < _G->getN(); i++)
        _updateLambdaMinLambdaMax(_G->getVertexByUniqueID(i),z,min,max,lambdaMinus,lambdaPlus);
	//Serial << "+:" << _DEC(lambdaPlus) << ", -:" << _DEC(lambdaMinus) << endl << "max:" << _FLOAT(max,3) << ", min:" << _FLOAT(min,3) << endl;

	return lambdaPlus - long((max-float(1))*(float(lambdaPlus-lambdaMinus))/(max-min));
	// Serial << "lambdaStar: " << _DEC(lambdastar) << endl;
	//     return lambdaPlus - long((max-float(1))*float((lambdaPlus-lambdaMinus)/s->getBase())/(max-min))*s->getBase();
}

void OAgent::_updateLambdaMinLambdaMax(OVertex * v, long &z, float &min, float &max, long &lambdaMinus, long &lambdaPlus) {
    uint8_t rsp;
    rsp = _checkRatio(min,max,_computeRatio(v->getYMin(),z));
    if(rsp == 1)
        lambdaMinus = v->getLambdaMin();
    else if(rsp == 2)
        lambdaPlus = v->getLambdaMin();
    rsp = _checkRatio(min,max,_computeRatio(v->getYMax(),z));
    if(rsp == 1)
        lambdaMinus = v->getLambdaMax();
    else if(rsp == 2)
        lambdaPlus = v->getLambdaMax();
}

uint8_t OAgent::_checkRatio(float &min, float &max, float ratio) {
    if(ratio < 1 && ratio > min) {
        min = ratio;
        return 1;
    } else if(ratio > 1 && ratio < max) {
        max = ratio;
        return 2;
    }
    return 0;
}

void OAgent::_computeFinalOptimalDispatchRatios(OLocalVertex * s, float ratios[]) {
    long z = s->getZ();
    for(uint8_t i = 0; i < _G->getN(); i++) {
        ratios[2*i]   = _computeRatio(_G->getVertexByUniqueID(i)->getYMin(),z);
        ratios[2*i+1] = _computeRatio(_G->getVertexByUniqueID(i)->getYMax(),z);;
    }
}

//float OAgent::_computeRatio(long num, long dem) {
//    return float(num)/float(dem);
//}

void OAgent::_findMinRatioMaxRatio(float ratios[], uint8_t num, float &min, float &max) {
    min = 0;
    max = 1000000;
    for(uint8_t i = 0; i < num; i++) {
        if(ratios[i] < 1 && ratios[i] > min)
            min = ratios[i];
        else if(ratios[i] > 1 && ratios[i] < max)
            max = ratios[i];
    }
}

//void OAgent::_waitForScheduleOptimalDispatchPacket(unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout) {
//    _waitForSchedulePacket(SCHEDULE_OPTIMAL_DISPATCH_HEADER,startTime,iterations,period,timeout);
//}
//
//void OAgent::_broadcastScheduleOptimalDispatchPacket(unsigned long startTime, uint8_t iterations, uint16_t period) {
//    _broadcastSchedulePacket(SCHEDULE_OPTIMAL_DISPATCH_HEADER,startTime,iterations,period);
//}

//bool OAgent::_optimalDispatchPacketAvailable() {
//    return _packetAvailable(OPTIMAL_DISPATCH_HEADER,true);
//}

void OAgent::_broadcastOptimalDispatchPacket(OLocalVertex * s) {
    uint8_t p = _getPayloadSize();
    uint8_t payload[p];
#ifdef VERBOSE
    Serial << "packet bytes: " << _DEC(p) << endl;
#endif
    uint8_t ptr = 0;  // pointer to next open position in payload array
    payload[0] = OPTIMAL_DISPATCH_HEADER;
    payload[1] = OPTIMAL_DISPATCH_HEADER >> 8;
    ptr = 2;          // account for header
    // add sigma to payload
    ptr = _addUint32_tToPayload(s->getSigma(),payload,ptr);
    // add broadcast states (muMin and muMax) and (optionally) lamdbaMin and lambdaMax to payload for all vertices
    for(uint8_t i = 0; i < (_G->getN()); i++) {
        ptr = _addDataToPayload(_G->getVertexByUniqueID(i),payload,ptr);
    }
#ifdef VERBOSE
    Serial << "payload: 0x"; 
    for(uint8_t i = 0; i < p; i++) {
        Serial << _HEX(payload[i]);
    }
    Serial << endl;
#endif
    // put payload in zigbee transmit object
    _zbTx = ZBTxRequest(_broadcastAddress, ((uint8_t * )(&payload)), p); 
    // transmit packet
    _xbee->send(_zbTx);
}

uint8_t OAgent::_getPayloadSize() {
	// get total number of devices
    uint8_t n = _G->getN();        
    // get number of lambdas to include in payload
    uint8_t l = _getNumberLambdaMinMax();
	//Serial << _MEM(PSTR("l=")) << _DEC(l) << endl;
    // header + sigma + (address lsb + subheader + muMin + muMax) * devices + (lambda min + lambda max) * lambdas
    return 2 + 4 + (4+1+4+4)*n + (4+4)*l;     
}

/*
 *
 * Returns the number of vertices that have valid lambda min and max that have not
 * yet been broadcasted.
 *
 */
uint8_t OAgent::_getNumberLambdaMinMax() {
    // initialize counter
    uint8_t l = 0;
    // check each vertex
    for(uint8_t i = 0; i < (_G->getN()); i++) {
    	if(_G->getVertexByUniqueID(i)->unsentValidLambda())
    		l++;
    }    
    return l;
}


uint8_t OAgent::_addDataToPayload(OVertex * v, uint8_t payload[], uint8_t ptr) {
    // get address lsb and put in playload
    ptr = _addUint32_tToPayload(v->getAddressLsb(),payload,ptr);
    // create byte to store subheader
    uint8_t subheader;
    // get state of broadcast lambda
    bool bl = v->unsentValidLambda();
    // determine if lambdaMin and lambdaMax of this vertex should be broadcast
    if(bl)
        subheader = INCLUDE_LAMBDA_MIN_MAX_SUBHEADER;
    else 
        subheader = NO_LAMBDA_MIN_MAX_SUBHEADER;
    // add subheader to payload
    payload[ptr] = subheader;
    // increment payload pointer
    ptr++;
    // put muMin in payload
    ptr = _addUint32_tToPayload(v->getMuMin(),payload,ptr);
    // put muMax in payload
    ptr = _addUint32_tToPayload(v->getMuMax(),payload,ptr);
    if(bl) {
		//Serial << _MEM(PSTR("includes lambdas")) << endl;
        ptr = _addUint32_tToPayload(v->getLambdaMin(),payload,ptr);
        ptr = _addUint32_tToPayload(v->getLambdaMax(),payload,ptr);
        v->incrementBroadcastLambda();
    }
    return ptr;
}

uint8_t OAgent::_addUint32_tToPayload(uint32_t data, uint8_t payload[], uint8_t ptr) {
    //Serial << "byte to payload: " << _DEC(data) << endl;
    payload[ptr]    = data;
    payload[ptr+1]  = data >> 8;
    payload[ptr+2]  = data >> 16;
    payload[ptr+3]  = data >> 24;
    return ptr + 4;
}

void OAgent::_processOptimalDispatchPacket(OLocalVertex * s, uint8_t i) {
    // Initialize pointer for traversing packet (header is first two bytes)
    uint8_t ptr = 2;
    long inData;
    // get sigma from packet
    inData = _getLongFromPacket(ptr);
    // add (sigma - tau) to zIn
    s->addToZIn(inData - (s->getTau(i)));
    // save received sigma as new tau to sender object
    s->setTau(i,inData);
    // variable to store address lsb from packet
    uint32_t inLsb;
    // variable to store subheader
    uint8_t subheader;
    // Do for each vertex since there are 2*n states
    for(uint8_t j = 0; j < _G->getN(); j++) {
        // get address lsb from packet
        inLsb = _getUint32_tFromPacket(ptr);
        // variable to store unique id of vertex
        // get pointer to vertex object and unique id of vertex
        OVertex * v = _G->getVertexByAddressLsb(inLsb);
        // get subheader from packet
        subheader = _getUint8_tFromPacket(ptr);
        // get muMin from packet
        inData = _getLongFromPacket(ptr);
        // add (muMin - nuMin) to yMinIn
        v->addToYMinIn(inData - (v->getNuMin(i)));
        // save received muMin as new nuMin to sender object
        v->setNuMin(i,inData);
        // get muMax from packet
        inData = _getLongFromPacket(ptr);
        // add (muMax - nuMax) to yMaxIn
        v->addToYMaxIn(inData - (v->getNuMax(i)));
        // save received muMax as new nuMax to sender object
        v->setNuMax(i,inData);
        // check if lambdas are included in packet
        if(subheader == INCLUDE_LAMBDA_MIN_MAX_SUBHEADER) {
        	// check if already have lambdaMin and lambdaMax
        	if(v->getLambdaMin() == 0 && v->getLambdaMax() == 0) {
    			// get lambdaMin from packet
    			inData = _getLongFromPacket(ptr);
    			// save lambdaMin
    			//v->setLambdaMin(inData);
    			// add g(lambdaMin) to yMinIn
    			//v->addToYMinIn(s->g(inData));
    			// get lambdaMax from packet
    			inData = _getLongFromPacket(ptr);
    			// save lambdaMax
    			//v->setLambdaMax(inData);
    			// add g(lambdaMax) to yMaxIn
    			//v->addToYMaxIn(s->g(inData));
        	} else {
        		// ignoring lambdas since we already have them
        		ptr += 8;
        	}
        }
    }
}

void OAgent::_printFinalYMinYMax(OLocalVertex * s) {
    for(uint8_t i = 0; i < _G->getN(); i++)
        Serial << _DEC(i) << " yMin: " << _DEC(_G->getVertexByUniqueID(i)->getYMin()) << ", yMax: " << _DEC(_G->getVertexByUniqueID(i)->getYMax()) << endl;
}

void OAgent::_printStates(OLocalVertex * s, uint8_t k, bool printY) {
    uint8_t nodeNum = 1;
    Serial << _MEM(PSTR("z(")) << (k+1) << _MEM(PSTR(",")) << _DEC(nodeNum) << _MEM(PSTR(") = ")) << _DEC(s->getZ()) << endl;
    if(printY == true) {
        for(uint8_t i = 0; i < _G->getN(); i++) {
            Serial << _MEM(PSTR("y")) << (i+1) << _MEM(PSTR("Min(")) << (k+1) << _MEM(PSTR(",")) << _DEC(nodeNum) << _MEM(PSTR(") = ")) << _DEC(_G->getVertexByUniqueID(i)->getYMin()) << _MEM(PSTR(";")) << endl;
            Serial << _MEM(PSTR("y")) << (i+1) << _MEM(PSTR("Max(")) << (k+1) << _MEM(PSTR(",")) << _DEC(nodeNum) << _MEM(PSTR(") = ")) << _DEC(_G->getVertexByUniqueID(i)->getYMax()) << _MEM(PSTR(";")) << endl;
        }
    }
}

void OAgent::_printLambdas(OLocalVertex * s) {
    uint8_t nodeNum = 1;
    for(uint8_t i = 0; i < _G->getN(); i++) {
        Serial << _MEM(PSTR("lambda")) << (i+1) << _MEM(PSTR("Min(")) << _DEC(nodeNum) << _MEM(PSTR(") = ")) << _DEC(_G->getVertexByUniqueID(i)->getLambdaMin()) << _MEM(PSTR(";")) << endl;
        Serial << _MEM(PSTR("lambda")) << (i+1) << _MEM(PSTR("Max("))  << _DEC(nodeNum) << _MEM(PSTR(") = ")) << _DEC(_G->getVertexByUniqueID(i)->getLambdaMax()) << _MEM(PSTR(";")) << endl;
    }
}
//#ifdef VERBOSE

void OAgent::_printRatios(float ratios[], uint8_t num) {
    for(uint8_t i = 0; i < num; i++)
        Serial << _FLOAT(ratios[i],3) << endl;
}

//#endif

/// End optimal dispatch methods

/// General xbee methods

/*
 * This function verifies the validity of an xbee packet.
 * It first checks to see if a packet is available then verifies that it is a zigbee rx packet.
 * If the packet it is valid the function will return true and it will store the response in the
 * ZBRxResponse parameter.
 */
bool OAgent::_validPacketAvailable() {
	if(_xbee->getResponse().isAvailable()) {	// packet is available
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
    return _packetAvailableHelper(header,broadcast);
}

uint16_t OAgent::_packetAvailable(bool broadcast) {
    _xbee->readPacket();
    if(_validPacketAvailable()) {
        if(((broadcast == true) && (_rx->getOption() == ZB_BROADCAST_PACKET)) || ((broadcast == false) && (_rx->getOption() == ZB_PACKET_ACKNOWLEDGED)))
            return _getHeaderFromPacket();
    }
    return 0x0;
}

bool OAgent::_packetAvailableHelper(uint16_t header, bool broadcast) {
    if(_validPacketAvailable()) {
        if(_getHeaderFromPacket() == header) {
        	if(((broadcast == true) && (_rx->getOption() == ZB_BROADCAST_PACKET)) || ((broadcast == false) && (_rx->getOption() == ZB_PACKET_ACKNOWLEDGED)))
	            return true;
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
//	return (uint16_t(_rx->getData(1)) << 8) + _rx->getData(0);
//}

bool OAgent::_waitForPacket(uint16_t header, unsigned long &rxTime, bool broadcast, int timeout) {
    unsigned long start;
	if(timeout != -1)
		start = millis();
	while(true) {
		if((timeout != -1) && (millis()-start >= timeout))
			return false;
		if(_packetAvailable(header,rxTime,broadcast))
			return true;
	}
}

bool OAgent::_waitForPacket(uint16_t header, bool broadcast, int timeout) {
	unsigned long start;
    if(timeout != -1)
		start = millis();
	while(true) {
		if((timeout != -1) && (millis()-start >= timeout))
			return false;
		if(_packetAvailable(header,broadcast))
			return true;
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
    unsigned long s = millis();
    while(true) {
        if(useMyMillis == true) {
            if(startTime - (myMillis()) <= 0)
                return true;
        } else {
            if(startTime - millis() <= 0)
                return true;
        }
        if(timeout != -1) {
            if(int(millis()-s) > timeout)
                return false;
        }
    }    
}

//uint32_t OAgent::_getStartTimeFromPacket() {
//    return (uint32_t(_rx->getData(5)) << 24) + (uint32_t(_rx->getData(4)) << 16) + (uint16_t(_rx->getData(3)) << 8) + _rx->getData(2);
//}
//
//uint8_t OAgent::_getIterationsFromPacket() {
//    return _rx->getData(6);
//}
//
//uint16_t OAgent::_getPeriodFromPacket() {
//    return (uint16_t(_rx->getData(8)) << 8) + _rx->getData(7);
//}

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

void OAgent::_waitForSchedulePacket(uint16_t header, unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout) {
    if(_waitForPacket(header,true,timeout)) {
		if(header == SCHEDULE_FAIR_SPLIT_HEADER || header == SCHEDULE_OPTIMAL_DISPATCH_HEADER) {
			startTime   = _getStartTimeFromPacket();
			iterations  = _getIterationsFromPacket();
			period      = _getPeriodFromPacket();
		}
	}
}

uint16_t OAgent::_waitForSchedulePacket(unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout) {
    uint16_t rsp = _waitForValidPacket(true,timeout);
    if(rsp != 0x0) {
        startTime   = _getStartTimeFromPacket();
        iterations  = _getIterationsFromPacket();
        period      = _getPeriodFromPacket();    
        return rsp;
    }
    return 0x0;
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

//long OAgent::_getLongFromPacket(uint8_t &lsbByteNumber) {
//	return long(_getUint32_tFromPacket(lsbByteNumber));
//}
/// End general coordination helper functions

/// Synchronization helper functions
bool OAgent::_leaderSync() {
    // only attempt to sync if there is at least one other vertex
    if(_G->getN() > 1) {
    	// seed random number generator with millis
        srand(millis());
        // get the index of a neighbor at random
        int i = (rand() % (_G->getN())) + 1;
        // broadcast sync begin packet
        unsigned long tOne = _broadcastSyncBeginPacket(i);
        // variable to store receive time of final packet
        unsigned long tFour;
        // wait until sync response packet arrives or timeout
        if(_waitForSyncResponsePacket(tFour)) {
            uint8_t ptr = 2;
            unsigned long tTwo = _getUint32_tFromPacket(ptr);
            long d = tTwo + _getUint32_tFromPacket(ptr) - tOne - tFour;
        	// compute error between target and base
        	d = float(d)/float(2);
        	// broadcast final packet
			_broadcastSyncFinalPacket(tTwo,d);
			return true;
        }
    }
    return false;
}

bool OAgent::_targetSync(unsigned long tTwo) {
	if(_unicastSyncResponsePacket(tTwo)) {
		if(_waitForSyncFinalPacket(SYNC_TIMEOUT)) {
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

//bool OAgent::_waitForSyncBeginPacket(unsigned long &rxTime) {
//	return _waitForPacket(SYNC_BEGIN_HEADER,rxTime,true,-1);
//}
//
//bool OAgent::_waitForSyncResponsePacket(unsigned long &rxTime) {
//	return _waitForPacket(SYNC_RESPONSE_HEADER,rxTime,false,SYNC_TIMEOUT);
//}
//
//bool OAgent::_waitForSyncFinalPacket(int timeout) {
//	return _waitForPacket(SYNC_FINAL_HEADER,true,timeout);
//}
/// End synchronization helper functions
/// General helper functions

void OAgent::_prepareOAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader, bool quiet) {
    _xbee = xbee;
    _G = G;
    _leader = leader;
    _quiet = quiet;
    _synced = false;
    _offset = 0;
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
}
/// End general helper functions