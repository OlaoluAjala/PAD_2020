/*
 *  OGraph.cpp
 *  
 *
 *  Created by Stanton T. Cady on 06/18/12
 *  Last edited by Stanton T. Cady on 08/02/12
 *
 */



#include "OGraph.h"
#include "Streaming.h"



//// OVertex
/// Public methods
// Constructors
OVertex::OVertex() {
    _prepareOVertex(0x0,0,0);
}

OVertex::OVertex(uint32_t aLsb) {
	_prepareOVertex(aLsb,0,0);
}

OVertex::OVertex(XBeeAddress64 a) {
	_prepareOVertex(a.getLsb(),0,0);
}

OVertex::OVertex(uint32_t aLsb, long lambdaMin, long lambdaMax) {
    _prepareOVertex(aLsb,lambdaMin,lambdaMax);
}

OVertex::OVertex(XBeeAddress64 a, long lambdaMin, long lambdaMax) {
    _prepareOVertex(a.getLsb(),lambdaMin,lambdaMax);
}

// Address least significant bytes
//uint32_t OVertex::getAddressLsb() {
//    return _aLsb;
//}

// States (yMin and yMax)
//void OVertex::setYMin(long yMin) {
//    _yMin = yMin;
//}

//void OVertex::setYMax(long yMax) {
//    _yMax = yMax;
//}
//
//void OVertex::setYMinYMax(long yMin, long yMax) {
//    setYMin(yMin);
//    setYMax(yMax);
//}
//
//void OVertex::addToYMin(long increment) {
//    _yMin += increment;
//}
//
//void OVertex::addToYMax(long increment) {
//    _yMax += increment;
//}
//
//void OVertex::addToYMinYMax(long incrementYMin, long incrementYMax) {
//    addToYMin(incrementYMin);
//    addToYMax(incrementYMax);
//}

//long OVertex::getYMin() {
//    return _yMin;
//}

//long OVertex::getYMax() {
//    return _yMax;
//}
//
//void OVertex::getYMinYMax(long &yMin, long &yMax) {
//    yMin = getYMin();
//    yMax = getYMax();
//}
//
//void OVertex::clearYMinYMax() {
//    setYMinYMax(0,0);
//}

void OVertex::updateYMinYMax(uint8_t weight) {
    setYMinYMax(_yMin/weight+_yMinIn,_yMax/weight+_yMaxIn);
}
//
//// incoming states yMinIn and yMaxIn
//void OVertex::setYMinIn(long yMinIn) {
//    _yMinIn = yMinIn;
//}
//
//void OVertex::setYMaxIn(long yMaxIn) {
//    _yMaxIn = yMaxIn;
//}
//
//void OVertex::setYMinInYMaxIn(long yMinIn, long yMaxIn) {
//    setYMinIn(yMinIn);
//    setYMaxIn(yMaxIn);
//}
//
//void OVertex::addToYMinIn(long increment) {
//    _yMinIn += increment;
//}
//
//void OVertex::addToYMaxIn(long increment) {
//    _yMaxIn += increment;
//}
//
//void OVertex::addToYMinInYMaxIn(long incrementYMinIn, long incrementYMaxIn) {
//    addToYMinIn(incrementYMinIn);
//    addToYMaxIn(incrementYMaxIn);
//}
//
//void OVertex::clearYMinInYMaxIn() {
//    setYMinInYMaxIn(0,0);
//}

// Broadcast states muMin and muMax
//void OVertex::setMuMin(long muMin) {
//    _muMin = muMin;
//}

//void OVertex::setMuMax(long muMax) {
//    _muMax = muMax;
//}
//
//void OVertex::setMuMinMuMax(long muMin, long muMax) {
//    setMuMin(muMin);
//    setMuMax(muMax);
//}

//long OVertex::getMuMin() {
//    return _muMin;
//}

//long OVertex::getMuMax() {
//    return _muMax;
//}
//
//void OVertex::getMuMinMuMax(long &muMin, long &muMax) {
//    muMin = getMuMin();
//    muMax = getMuMax();
//}
//
//void OVertex::clearMuMinMuMax() {
//    setMuMinMuMax(0,0);
//}

void OVertex::updateMuMinMuMax(uint8_t weight) {
    setMuMinMuMax(_muMin+_yMin/weight,_muMax+_yMax/weight);
}

// Robust algorithm states nuMin and nuMax
bool OVertex::setNuMin(uint8_t i, long nuMin) {
    if(i < NUM_IN_NEIGHBORS) {
        _nuMin[i] = nuMin;
        return true;
    }
    return false;
}
//redefine the fucntion to be able to recive float
bool OVertex::setNuMinP(uint8_t i, float nuMin) {
    if(i < NUM_IN_NEIGHBORS) {
        _nuMin[i] = nuMin;
        return true;
    }
    return false;
}

bool OVertex::setNuMax(uint8_t i, long nuMax) {
    if(i < NUM_IN_NEIGHBORS) {
        _nuMax[i] = nuMax;
        return true;
    }
    return false;
}

long OVertex::getNuMin(uint8_t i) {
    if(i < NUM_IN_NEIGHBORS)
        return _nuMin[i];
    return LONG_ERROR;
}

float OVertex::getNuMinP(uint8_t i) {
    if(i < NUM_IN_NEIGHBORS)
        return _nuMin[i];
    return LONG_ERROR;
}


long OVertex::getNuMax(uint8_t i) {
    if(i < NUM_IN_NEIGHBORS)
        return _nuMax[i];
    return LONG_ERROR;
}

bool OVertex::clearNuMinNuMax(uint8_t i) {
    if(i < NUM_IN_NEIGHBORS) {
        _nuMin[i] = 0;
        _nuMax[i] = 0;
        return true;
    }
    return false;
}

void OVertex::clearAllNuMinNuMax() {
    for(uint8_t i = 0; i < NUM_IN_NEIGHBORS; i++) {
        _nuMin[i] = 0;
        _nuMax[i] = 0;
    }        
}

// Lambda_min and lambda_max
//void OVertex::setLambdaMin(long lambdaMin) {
//    _lambdaMin = lambdaMin;
//}
//
//void OVertex::setLambdaMax(long lambdaMax) {
//    _lambdaMax = lambdaMax;
//}
//
//void OVertex::setLambdaMinMax(long lambdaMin, long lambdaMax) {
//    setLambdaMin(lambdaMin);
//    setLambdaMax(lambdaMax);
//}
//
//long OVertex::getLambdaMin() {
//    return _lambdaMin;
//}
//
//long OVertex::getLambdaMax() {
//    return _lambdaMax;
//}
//
//void OVertex::getLambdaMinMax(long &lambdaMin, long &lambdaMax) {
//    lambdaMin = getLambdaMin();
//    lambdaMax = getLambdaMax();
//}
//
//void OVertex::setBroadcastLambda(uint8_t broadcastLambda) {
//    _broadcastLambda = broadcastLambda;
//}
//
//void OVertex::clearBroadcastLambda() {
//    setBroadcastLambda(0);
//}
//
//void OVertex::incrementBroadcastLambda() {
//    _broadcastLambda++;
//}
//
//uint8_t OVertex::getBroadcastLambda() {
//    return _broadcastLambda;
//}

bool OVertex::unsentValidLambda() {
    if(_broadcastLambda <= NUM_BROADCAST_LAMBDA && _lambdaMin != 0 && _lambdaMax != 0)
        return true;
    return false;
}

/// End public methods
/// Private methods
// Helper functions
void OVertex::_prepareOVertex(uint32_t aLsb, long lambdaMin, long lambdaMax) {
    _aLsb = aLsb;
    _lambdaMin = lambdaMin;
    _lambdaMax = lambdaMax;
    _broadcastLambda = 0;
    _yMin   = 0;
    _yMax   = 0;
    _yMinIn = 0;
    _yMaxIn = 0;
    _muMin  = 0;
    _muMax  = 0;
    clearAllNuMinNuMax();
}
/// End private methods
//// End OVertex

//// OLocalVertex
/// Public methods
// Constructors
OLocalVertex::OLocalVertex() {
    _prepareOLocalVertex(0x0,0,0,0,0,0,100000);
}

OLocalVertex::OLocalVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base) {
    _prepareOLocalVertex(aLsb,min,max,alpha,beta,Dout,base);
}

OLocalVertex::OLocalVertex(XBeeAddress64 a, long min, long max, long alpha, long beta, uint8_t Dout, long base) {
    _prepareOLocalVertex(a.getLsb(),min,max,alpha,beta,Dout,base);
}

// In-degree methods
//uint8_t OLocalVertex::getInDegree() {
//    return _inDegree;
//}
//
//void OLocalVertex::incrementInDegree() {
//	_inDegree++;
//}
//
//// Out-degree methods
//void OLocalVertex::setOutDegree(uint8_t Dout) {
//    _outDegree = Dout;
//}
//
//uint8_t OLocalVertex::getOutDegree() {
//    return _outDegree;
//}

// Optimal dispatch functions
long OLocalVertex::g(long lambda) {
	float b = float(_base);
    if(0 <= lambda && lambda < _lambdaMin)
        return _min;
    else if(_lambdaMin <= lambda && lambda <= _lambdaMax)
		return _alpha + long(b*((float(lambda)/b)*float(_beta)/b));
        //return _alpha + _base*((lambda/_base)*float(_beta)/float(_base));
    else if(_lambdaMax < lambda)
        return _max;
    else
        return LONG_ERROR;
}

// Base functions
//void OLocalVertex::setBase(long base) {
//    _base = base;
//}
//
//long OLocalVertex::getBase() {
//    return _base;
//}

// Internal local states yMin and yMax
void OLocalVertex::initializeYMinYMax() {
    _yMin = g(_lambdaMin);
    _yMax = g(_lambdaMax);
}

// Internal state z
//void OLocalVertex::setZ(long z) {
//    _z = z;
//}
//
//void OLocalVertex::addToZ(long increment) {
//    _z += increment;
//}
//
//long OLocalVertex::getZ() {
//    return _z;
//}

void OLocalVertex::updateZ() {
    //Serial << "zinitial: " << _DEC(_z) << ", zIn: " << _DEC(_zIn);
    _z = _z/(_outDegree+1) + _zIn;
    //Serial << " zfinal: " << _DEC(_z) << endl;
}

// Incoming state z
//void OLocalVertex::setZIn(long zIn) {
//    _zIn = zIn;
//}
//
//void OLocalVertex::addToZIn(long increment) {
//    //Serial << "zIn: " << _DEC(_zIn) << ", increment: " << _DEC(increment);
//    _zIn += increment;
//    //Serial << ", new zIn: " << _DEC(_zIn) << endl;
//}
//
//void OLocalVertex::clearZIn() {
//    setZIn(0);
//    //Serial << "Clear zIn: " << _DEC(_zIn) << endl;
//}
//
//long OLocalVertex::getZIn() {
//    return _zIn;
//}

// Local broadcast states muMin and muMax
void OLocalVertex::initializeMuMinMuMax() {
    _muMin = _yMin/(_outDegree+1);
    _muMax = _yMax/(_outDegree+1);
}

// Broadcast state sigma

//void OLocalVertex::setSigma(long sigma) {
//    _sigma = sigma;
//}
//
//void OLocalVertex::addToSigma(long increment) {
//    _sigma += increment;
//}
//
//long OLocalVertex::getSigma() {
//    return _sigma;
//}

void OLocalVertex::updateSigma() {
    //Serial << "update sigma, z: " << _DEC(_z) << ", sigma: " << _DEC(_sigma);
    _sigma += _z/(_outDegree+1);
    //Serial << " new sigma: " << _DEC(_sigma) << endl;
}

void OLocalVertex::initializeSigma() {
    _sigma = _z/(_outDegree+1);
}

//void OLocalVertex::clearSigma() {
//	setSigma(0);
//}

// Robust algorithm state tau
bool OLocalVertex::setTau(uint8_t i, long tau) {
    if(i < NUM_IN_NEIGHBORS) {
        _tau[i] = tau;
        return true;
    }
    return false;
}
//redefine the function to be able to geat float values
bool OLocalVertex::setTauP(uint8_t i, float tau) {
    if(i < NUM_IN_NEIGHBORS) {
        _tau[i] = tau;
        return true;
    }
    return false;
}

long OLocalVertex::getTau(uint8_t i) {
    if(i < NUM_IN_NEIGHBORS)
        return _tau[i];
    return LONG_ERROR;
}
//redefine the function to send float
float OLocalVertex::getTauP(uint8_t i) {
    if(i < NUM_IN_NEIGHBORS)
        return _tau[i];
    return LONG_ERROR;
}

void OLocalVertex::clearAllTau() {
    for(uint8_t i = 0; i < NUM_IN_NEIGHBORS; i++)
        setTau(i,0);
}

// Limits and cost coefficients
//long OLocalVertex::getMin() {
//    return _min;
//}
//
//long OLocalVertex::getMax() {
//    return _max;
//}
//
//long OLocalVertex::getAlpha() {
//    return _alpha;
//}
//
//long OLocalVertex::getBeta() {
//    return _beta;
//}
//
//void OLocalVertex::clearZInSigma() {
//    clearZIn();
//    clearSigma();
//}
/// End public methods
/// Private methods
// Helper functions
void OLocalVertex::_prepareOLocalVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base) {
    // set base
    _base = base;
    // initialize states
    _z 		= 0;
    _zIn 	= 0;
    _sigma 	= 0;
    clearAllTau();
    // set limits
    _min = min;
    _max = max;
    _alpha = alpha;
    _beta = beta;
    // set graph out-degree
    _inDegree = 0;
    _outDegree = Dout;
    // set address and lambda_min and lambda_max
    _prepareOVertex(aLsb,_computeLambda(_min),_computeLambda(_max));
}

// Optimal dispatch functions
long OLocalVertex::_computeLambda(long limit) {
	float b = float(_base);
    long rslt = long(b*(float(limit - _alpha)/float(_beta)));
    if(rslt > 0)
        return rslt;
    else
        return LONG_ERROR;
}
//////////////////

void OLocalVertex::setFlowLimits( float fijmax[],float fijmin[]) {   
    uint8_t k;
    for(k=0;k<NUM_IN_NEIGHBORS;k++){
        _Fijmin[k]=fijmin[k];
        _Fijmax[k]=fijmax[k];
         }
    }

//creamos una funcion en OLocelVertex para inicializar flujos y genenracion 
void OLocalVertex::InitializeFlowLimits(){
uint8_t i;

  for(i=0;i<NUM_IN_NEIGHBORS,i++){
     _Fij0[i]=0.5*(_Fijmax[i]+_Fijmin[i]);
  }
}


//////////////////////
/// End private methods
//// End OLocalVertex

//// OLocalReserveVertex
/// Public methods
// Constructors
OLocalReserveVertex::OLocalReserveVertex() {
	_prepareOLocalReserveVertex(0x0,0,0,0,0,0,100000);
}

OLocalReserveVertex::OLocalReserveVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base) {
	_prepareOLocalReserveVertex(aLsb,min,max,alpha,beta,Dout,base);
}

OLocalReserveVertex::OLocalReserveVertex(XBeeAddress64 a, long min, long max, long alpha, long beta, uint8_t Dout, long base) {
	_prepareOLocalReserveVertex(a.getLsb(),min,max,alpha,beta,Dout,base);
}
/// End public methods
/// Private methods
void OLocalReserveVertex::_prepareOLocalReserveVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base) {
    _reserveMin = min;
    _reserveMax = max;
	_limitExceeded = false;
	_limitGamma = 1;
    _prepareOLocalVertex(aLsb,min,max,alpha,beta,Dout,base);
}

/// End private methods
//// End OLocalVertexReserve

//// OInNeighbor
/// Public methods
// Constructors
ORemoteVertex::ORemoteVertex() {
    _prepareORemoteVertex();
}

ORemoteVertex::ORemoteVertex(XBeeAddress64 a, bool inNeighbor, uint8_t index) {
	_prepareORemoteVertex(a.getLsb(),0,0,inNeighbor,index);
}

ORemoteVertex::ORemoteVertex(uint32_t aLsb, bool inNeighbor, uint8_t index) {
	_prepareORemoteVertex(aLsb,0,0,inNeighbor,index);
}

ORemoteVertex::ORemoteVertex(XBeeAddress64 a, long lambdaMin, long lambdaMax, bool inNeighbor, uint8_t index) {
    _prepareORemoteVertex(a.getLsb(),lambdaMin,lambdaMax,inNeighbor,index);
}

ORemoteVertex::ORemoteVertex(uint32_t aLsb, long lambdaMin, long lambdaMax, bool inNeighbor, uint8_t index) {
    _prepareORemoteVertex(aLsb,lambdaMin,lambdaMax,inNeighbor,index);
}

//bool ORemoteVertex::isInNeighbor() {
//    return _inNeighbor;
//}
//
//uint8_t ORemoteVertex::getIndex() {
//    return _index;
//}
//
//void ORemoteVertex::clearLambdaMinLambdaMax() {
//    _lambdaMin = 0;
//    _lambdaMax = 0;
//}

/// End public methods
/// Private methods
void ORemoteVertex::_prepareORemoteVertex(uint32_t aLsb, long lambdaMin, long lambdaMax, bool inNeighbor, uint8_t index) {
    _inNeighbor = inNeighbor;
    _index = index;
    _prepareOVertex(aLsb,lambdaMin,lambdaMax);
}
/// End private methods
//// End ORemoteVertex

//// OGraph
/// Public methods
// Constructors
OGraph::OGraph() {
    _n = 0;
    OLocalVertex temp1=OLocalVertex();
    _self = &temp1;
}

OGraph::OGraph(OLocalVertex * s) {
    _n = 1;
    _self = s;
}

// Local vertex
//OLocalVertex * OGraph::getLocalVertex() {
//    return _self;
//}

bool OGraph::isLocalVertex(uint32_t aLsb) {
    if(aLsb == _self->getAddressLsb())
        return true;
    else
        return false;
}

//bool OGraph::isLocalVertex(XBeeAddress64 a) {
//	return isLocalVertex(a.getLsb());
//}

// Graph properties
//uint8_t OGraph::getN() {
//    return _n;
//}

//uint8_t OGraph::getNumberNonNeighbors() {
//    return _n - _self->getInDegree() - 1;
//}

// Add and remove vertices from in-neighborhood
bool OGraph::addInNeighbor(XBeeAddress64 a) {
    return addInNeighbor(a.getLsb());
}

bool OGraph::addInNeighbor(uint32_t aLsb) {
    return addInNeighbor(aLsb,0,0);
}

bool OGraph::addInNeighbor(XBeeAddress64 a, long lambdaMin, long lambdaMax) {
    return addInNeighbor(a.getLsb(),lambdaMin,lambdaMax);
}

bool OGraph::addInNeighbor(uint32_t aLsb, long lambdaMin, long lambdaMax) {
    // make sure we can add another inNeighbor
    if((_self->getInDegree()) < NUM_IN_NEIGHBORS)
        return _addRemoteVertex(aLsb,lambdaMin,lambdaMax,true);
    return false;
}

// Determine if vertex is in in-neighborhood
bool OGraph::isInNeighbor(XBeeAddress64 a) {
    return isInNeighbor(a.getLsb());
}

bool OGraph::isInNeighbor(uint32_t aLsb) {
    uint8_t i;
    return isInNeighbor(aLsb,i);
}

bool OGraph::isInNeighbor(XBeeAddress64 a, uint8_t &i) {
    return isInNeighbor(a.getLsb(),i);
}

bool OGraph::isInNeighbor(uint32_t aLsb, uint8_t &i) {
    if(_isRemoteVertex(aLsb,i))
        return _remoteVertices[i].isInNeighbor();
    return false;
}

bool OGraph::isInNeighbor(uint32_t aLsb, ORemoteVertex * &v) {
    uint8_t i;
    if(isInNeighbor(aLsb,i)) {
        v = &_remoteVertices[i];
        return true;
    }
    return false;
}

// Add and remove vertices from list of non-neighbors
bool OGraph::addNonNeighbor(XBeeAddress64 a) {
    return addNonNeighbor(a.getLsb());
}

bool OGraph::addNonNeighbor(uint32_t aLsb) {
    return addNonNeighbor(aLsb,0,0);
}

bool OGraph::addNonNeighbor(XBeeAddress64 a, long lambdaMin, long lambdaMax) {
    return addNonNeighbor(a.getLsb(),lambdaMin,lambdaMax);
}

bool OGraph::addNonNeighbor(uint32_t aLsb, long lambdaMin, long lambdaMax) {
    return _addRemoteVertex(aLsb,lambdaMin,lambdaMax);
}

// Determine if vertex is non-neighbor
bool OGraph::isNonNeighbor(XBeeAddress64 a) {
    return isNonNeighbor(a.getLsb());
}

bool OGraph::isNonNeighbor(uint32_t aLsb) {
    uint8_t i;
    return isNonNeighbor(aLsb,i);
}

bool OGraph::isNonNeighbor(XBeeAddress64 a, uint8_t &i) {
    return isNonNeighbor(a.getLsb(),i);
}

bool OGraph::isNonNeighbor(uint32_t aLsb, uint8_t &i) {
    if(_isRemoteVertex(aLsb,i))
        return !(_remoteVertices[i].isInNeighbor());
    return false;
}

// Methods to clear various states
void OGraph::clearAllNuTau() {
	_self->clearAllTau();
    _self->clearAllNuMinNuMax();
    for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++) {
        _remoteVertices[i].clearAllNuMinNuMax();
    }
}

void OGraph::clearAllYMinYMax() {
    _self->clearYMinYMax();
	for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++)
		_remoteVertices[i].clearYMinYMax();
}

void OGraph::clearAllInStates() {
    _self->clearZIn();
    _self->clearYMinInYMaxIn();
	for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++)
		_remoteVertices[i].clearYMinInYMaxIn();
}

void OGraph::clearAllBroadcastStates() {
	_self->clearSigma();
    _self->clearMuMinMuMax();
	for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++)
		_remoteVertices[i].clearMuMinMuMax();
}

void OGraph::clearAllBroadcastLambda() {
    _self->clearBroadcastLambda();
    for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++)
        _remoteVertices[i].clearBroadcastLambda();
}

void OGraph::clearAllStates() {
    _self->clearZInSigma();
    _self->setZ(0);
    _self->clearAllTau();
    _self->clearYMinYMax();
    _self->clearYMinInYMaxIn();
    _self->clearMuMinMuMax();
    _self->clearAllNuMinNuMax();
    _self->clearBroadcastLambda();
    for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++) {
        _remoteVertices[i].clearYMinYMax();
        _remoteVertices[i].clearYMinInYMaxIn();
        _remoteVertices[i].clearMuMinMuMax();
        _remoteVertices[i].clearAllNuMinNuMax();
        _remoteVertices[i].clearBroadcastLambda();
        //_remoteVertices[i].clearLambdaMinLambdaMax();
    }    
}

OVertex * OGraph::getVertexByAddressLsb(uint32_t aLsb) {
    uint8_t i;
    return getVertexByAddressLsb(aLsb,i);
}

// Methods to pointer to vertex
OVertex * OGraph::getVertexByAddressLsb(uint32_t aLsb, uint8_t &i) {
	uint8_t j;	// index of in-neighbor or non-neighbor
	if(isLocalVertex(aLsb)) {
        // vertex id of local vertex is 0
        i = 0;
		return dynamic_cast<OVertex*> (_self);
	} else if(_isRemoteVertex(aLsb,j)) {
        // vertex id of in-neighbor is index of inNeighbor + 1
        i = j + 1;
		return dynamic_cast<OVertex*> (&_remoteVertices[j]);
	}
}

OVertex * OGraph::getVertexByAddress(XBeeAddress64 a) {
	return getVertexByAddressLsb(a.getLsb());
}

OVertex * OGraph::getVertexByUniqueID(uint8_t i) {
	if(i == 0) {
		return dynamic_cast<OVertex*> (_self);
	} else if(i >= 1 && i <= NUM_REMOTE_VERTICES) {
		return dynamic_cast<OVertex*> (&_remoteVertices[i-1]);
	}
}

ORemoteVertex * OGraph::getRemoteVertex(uint8_t i) {
    if(i < NUM_REMOTE_VERTICES)
        return &_remoteVertices[i];
    return 0x0;
}

/// End public methods
/// Private methods


uint8_t OGraph::_getRemoteVertexIndex(XBeeAddress64 a) {
	return _getRemoteVertexIndex(a.getLsb());
}

uint8_t OGraph::_getRemoteVertexIndex(uint32_t aLsb) {
	for(uint8_t i = 0; i < NUM_REMOTE_VERTICES; i++) {
		if(_remoteVertices[i].getAddressLsb() == aLsb)
			return i;
	}
	return INVALID_ARRAY_INDEX;
}

bool OGraph::_isRemoteVertex(uint32_t aLsb) {
    uint8_t i = _getRemoteVertexIndex(aLsb);
    return _isRemoteVertex(aLsb,i);
}

bool OGraph::_isRemoteVertex(uint32_t aLsb, uint8_t &i) {
    i = _getRemoteVertexIndex(aLsb);
    if(i != INVALID_ARRAY_INDEX)
        return true;
    return false;
}

bool OGraph::_addRemoteVertex(uint32_t aLsb, long lambdaMin, long lambdaMax, bool inNeighbor) {
    // get current number of vertices in array of non-neighbors
    uint8_t s = _n - 1;
	// check if another vertex can be stored
	if(s+1 <= NUM_REMOTE_VERTICES) {
		// there is room for one more non-neighbor in array
		// check if vertex is already stored in array
        if(!_isRemoteVertex(aLsb)) {
            uint8_t index = 0;
            // if vertex is an in-neighbor, increment local vertex in-degree variable
            if(inNeighbor) {
                index = _self->getInDegree();
                _self->incrementInDegree();
            }
        	// create new instance of OVertex object and store in array
            _remoteVertices[s] = ORemoteVertex(aLsb,lambdaMin,lambdaMax,inNeighbor,index);
            // increment total number of vertices current initialized
            _n++;
#ifdef VERBOSE
            Serial << _MEM(PSTR("Vertex with address lsb 0x")) << _HEX(aLsb) << _MEM(PSTR(" added array of remote vertices.")) << endl;
#endif
            return true;            
        } else {
#ifdef VERBOSE
            Serial << _MEM(PSTR("Vertex not added. A vertex with the same address is already in the array of remote vertices.")) << endl;
#endif
        }
	} else {
#ifdef VERBOSE
        Serial << _MEM(PSTR("No room to add another vertex to array of remote vertices.")) << endl;
#endif
    }
	return false; // anything that comes here is an error /// End private methods
}
//// End OGraph