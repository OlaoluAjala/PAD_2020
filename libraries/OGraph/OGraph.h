/*
 *  OGraph.h
 *  
 *
 *  Created by Stanton T. Cady on 06/15/12
 *  Last edited by Stanton T. Cady on 07/30/12
 *
 */

#ifndef OGraph_h
#define OGraph_h

#define INVALID_ARRAY_INDEX 255
#define NUM_BROADCAST_LAMBDA 3
#define LONG_ERROR 2147483647

#define NUM_REMOTE_VERTICES 22
#define NUM_IN_NEIGHBORS 22

//#define VERBOSE

#include "../XBee/XBee.h" // Include header for xbee api

class OVertex {
    public:
        // Constructors
        OVertex();
        OVertex(uint32_t aLsb);
        OVertex(XBeeAddress64 a);
        OVertex(uint32_t aLsb, long lambdaMin, long lambdaMax);
        OVertex(XBeeAddress64 a, long lambdaMin, long lambdaMax);
        // Address
        inline uint32_t getAddressLsb() { return _aLsb; }
        
        // States
        inline void setYMin(long yMin) { _yMin = yMin; }
        //redefine function to recive float value
        inline void setYMinP(float yMin) { _yMin = yMin; }


        inline void setYMax(long yMax) { _yMax = yMax; }
        inline void setYMinYMax(long yMin, long yMax) { _yMin = yMin; _yMax = yMax; }
        inline void addToYMin(long increment) { _yMin += increment; }
        inline void addToYMax(long increment) { _yMax += increment; }
        inline void addToYMinYMax(long incrementYMin, long incrementYMax) { _yMin += incrementYMin; _yMax += incrementYMax; }
        inline long getYMin() { return _yMin; }

        //redefine the function to operate with float
        inline float getYMinP() { return _yMin; }

        inline long getYMax() { return _yMax; }
        inline void getYMinYMax(long &yMin, long &yMax) { yMin = _yMin; yMax = _yMax; }
        inline void clearYMinYMax() { _yMin = 0; _yMax = 0; }
        void updateYMinYMax(uint8_t weight);
        
        // incoming states
        inline void setYMinIn(long yMinIn) { _yMinIn = yMinIn; }
        inline void setYMaxIn(long yMaxIn) { _yMaxIn = yMaxIn; }
        inline void setYMinInYMaxIn(long yMinIn, long yMaxIn) { _yMinIn = yMinIn; _yMaxIn = yMaxIn; }
        inline void addToYMinIn(long increment) { _yMinIn += increment; }
        inline void addToYMaxIn(long increment) { _yMaxIn += increment; }
        inline void addToYMinInYMaxIn(long incrementYMinIn, long incrementYMaxIn) { _yMinIn += incrementYMinIn; _yMaxIn += incrementYMaxIn; }
        inline void clearYMinInYMaxIn() { _yMinIn = 0; _yMaxIn = 0; }

        // broadcast states
        inline void setMuMin(long muMin) { _muMin = muMin; }
        //redefine function to work with    
        inline void setMuMinP(long muMin) { _muMin = muMin; }

        inline void setMuMax(long muMax) { _muMax = muMax; }
        inline void setMuMinMuMax(long muMin, long muMax) { _muMin = muMin; _muMax = muMax; }
        inline long getMuMin() { return _mu; }
        //same function as avobe but with changes
        inline float getMUminP(){return _muMin ;}

        inline long getMuMax() { return _muMax; }
        inline void getMuMinMuMax(long &muMin, long &muMax) { muMin = _muMin; muMax = _muMax; }
        inline void clearMuMinMuMax() { _muMin = 0; _muMax = 0; }
        void updateMuMinMuMax(uint8_t weight);

        // Previously received values for robust algorithm
        bool setNuMin(uint8_t i, long nuMin);
        //redefine the function to be able to recibe float
        bool setNuMinP(uint8_t i, float nuMin);

        bool setNuMax(uint8_t i, long nuMax);
        long getNuMin(uint8_t i);
        //redefine the function to be able to send floatvalues
         float getNuMinP(uint8_t i);

        long getNuMax(uint8_t i);
        bool clearNuMinNuMax(uint8_t i);
        void clearAllNuMinNuMax();

        // Lambdas
        inline void setLambdaMin(long lambdaMin) { _lambdaMin = lambdaMin; }
        inline void setLambdaMax(long lambdaMax) { _lambdaMax = lambdaMax; }
        inline void setLambdaMinMax(long lambdaMin, long lambdaMax) { _lambdaMin = lambdaMin; _lambdaMax = lambdaMax; }
        inline long getLambdaMin() { return _lambdaMin; }
        inline long getLambdaMax() { return _lambdaMax; }
        inline void getLambdaMinMax(long &lambdaMin, long &lambdaMax) { lambdaMin = _lambdaMin; lambdaMax = _lambdaMax; }
        // Keep track of broadcasting lambdas
        inline void setBroadcastLambda(uint8_t broadcastLambda) { _broadcastLambda = broadcastLambda; }
        inline void clearBroadcastLambda() { _broadcastLambda = 0; }
        inline void incrementBroadcastLambda() { _broadcastLambda++; }
        inline uint8_t getBroadcastLambda() { return _broadcastLambda; }
        bool unsentValidLambda();

    protected:
        /// Properties
        uint32_t _aLsb;
        long _lambdaMin;
        long _lambdaMax;
        // Boolean variable to keep track of broadcasting lambdas
        uint8_t _broadcastLambda;
        // States
        //long _yMin;
        //redefine varible to float
         float _yMin;

        long _yMax;
        // Variable to keep track of incoming states
        long _yMinIn;
        long _yMaxIn;
        // Broadcast states for robust algorithm
        //long _muMin;
        //redefine the variable to float
        float _muMin;
        float _mu;

        long _muMax;
        // Previous received values for robust algorithm

        //long _nuMin[NUM_IN_NEIGHBORS];

        //modificamos a float 
        float _nuMin[NUM_IN_NEIGHBORS];

        long _nuMax[NUM_IN_NEIGHBORS];
        /// Methods
        // Helper functions
        void _prepareOVertex(uint32_t aLsb, long lambdaMin, long lambdaMax);
};

class OLocalVertex : public OVertex {
    public:
        // Constructors
        OLocalVertex();
        OLocalVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base);
        OLocalVertex(XBeeAddress64 a, long min, long max, long alpha, long beta, uint8_t Dout, long base);
        // In-degree methods
        inline uint8_t getInDegree() { return _inDegree; }
        inline void incrementInDegree() { _inDegree++; }
        // Out-degree
        inline void setOutDegree(uint8_t Dout) { _outDegree = Dout; }
        inline uint8_t getOutDegree() { return _outDegree; }
        // Optimal dispatch functions
        long g(long lambda);
        // Base
        inline void setBase(long base) { _base = base; }
        inline long getBase() { return _base; }
        // States yMin yMax
        void initializeYMinYMax();
        // State Z
        inline void setZ(long z) { _z = z; }
        //redefine function to work with float
        inline void setZP(long z) { _z = z; }

        inline void addToZ(long increment) { _z += increment; }
        inline long getZ() { return _z; }
        //redefine function to work with float
        inline float getZP() { return _z; }
        void updateZ();
        // Incoming state Z
        inline void setZIn(long zIn) { _zIn = zIn; }
        inline void addToZIn(long increment) { _zIn += increment; }
        inline void clearZIn() { _zIn = 0; }
        inline long getZIn() { return _zIn; }
        // Broadcast states muMin and muMax
        void initializeMuMinMuMax();
        // Sigma -- broadcast state for robust algorithm
        inline void setSigma(long sigma) { _sigma = sigma; }
        inline void addToSigma(long increment) { _sigma += increment; }
        //redefine the function to work with float
        inline void addToSigmaP(float increment) { _sigma += increment; }
        
        inline long getSigma() { return _sigma; }

         inline float getSigmaP() { return _sigma; }

        void updateSigma();
        void initializeSigma();
        inline void clearSigma() { _sigma = 0; }
        // Tau -- state for robust algorithm
        bool setTau(uint8_t i, long tau);
        //redefine the function to accept float values
        bool setTauP(uint8_t i, float tau);

        long getTau(uint8_t i);
        //redefine the function to send float
        float getTauP(uint8_t i);
        void clearAllTau();
        // Limits and cost coefficients
        inline long getMin() { return _min; }
        inline long getMax() { return _max; }
		inline void setMax(long max) { _max = max; }
        inline long getAlpha() { return _alpha; }
        inline long getBeta() { return _beta; }
        // Clear temporary states (zIn and sigma)
        inline void clearZInSigma() { _zIn = 0; _sigma = 0; }
	protected:
        /// Properties
        long _base;
        // Ratio-consensus states
        //long _z;

        //redefine variable to float
        float _z;
        long _zIn;
        // Robust algorithm states
        //long _sigma;

        //redefine the variable to float
        float _sigma;

        //long _tau[NUM_IN_NEIGHBORS];

        //redefine the vector to float tipe
        float _tau[NUM_IN_NEIGHBORS];
        // Limits and cost coefficients
        long _min;
        long _max;
        long _alpha;
        long _beta;
        // Graph degrees
        uint8_t _inDegree;
        uint8_t _outDegree;
        /// Methods
        void _prepareOLocalVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base);
        long _computeLambda(long limit);  
};

class OLocalReserveVertex : public OLocalVertex {
	public:
		OLocalReserveVertex();
        OLocalReserveVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base);
        OLocalReserveVertex(XBeeAddress64 a, long min, long max, long alpha, long beta, uint8_t Dout, long base);
		inline long getReserveMin() { return _reserveMin; }
		inline void setReserveMin(long reserveMin) { _reserveMin = reserveMin; }
		inline long getReserveMax() { return _reserveMax; }
		inline void setReserveMax(long reserveMax) { _reserveMax = reserveMax; }
		inline bool getLimitExceeded() { return _limitExceeded; }
		inline void setLimitExceeded(bool limitExceeded) { _limitExceeded = limitExceeded; }
		inline float getLimitGamma() { return _limitGamma; }
		inline void setLimitGamma(float limitGamma) { _limitGamma = limitGamma; }
	private:	
		/// Properties
		long _reserveMin;
		long _reserveMax;
		bool _limitExceeded;
		float _limitGamma;
		/// Methods
		void _prepareOLocalReserveVertex(uint32_t aLsb, long min, long max, long alpha, long beta, uint8_t Dout, long base);
};

class ORemoteVertex : public OVertex {
    public:
        // Constructors
        ORemoteVertex();
        ORemoteVertex(XBeeAddress64 a, bool inNeighbor = false, uint8_t index = 0);
        ORemoteVertex(uint32_t aLsb, bool inNeighbor = false, uint8_t index = 0);
        ORemoteVertex(XBeeAddress64 a, long lambdaMin, long lambdaMax, bool inNeighbor = false, uint8_t index = 0);
        ORemoteVertex(uint32_t aLsb, long lambdaMin , long lambdaMax, bool inNeighbor = false, uint8_t index = 0);
        
        // Get directive for inNeighbor
        inline bool isInNeighbor() { return _inNeighbor; }
        inline uint8_t getIndex() { return _index; }
        
        inline void clearLambdaMinLambdaMax() { _lambdaMin = 0; _lambdaMax = 0; }
    private:
        /// Properties
        bool _inNeighbor;
        uint8_t _index;
        /// Methods
        // Constructor helper
        void _prepareORemoteVertex(uint32_t aLsb = 0x0, long lambdaMin = 0, long lambdaMax = 0, bool inNeighbor = false, uint8_t index = 0);
};

class OGraph {
	public:
        /// Constructors
		OGraph();
        OGraph(OLocalVertex * s);
        
        /// Local vertex
        inline OLocalVertex * getLocalVertex() { return _self; }
        bool isLocalVertex(uint32_t aLsb);
        inline bool isLocalVertex(XBeeAddress64 a) { return isLocalVertex(a.getLsb()); }
        
        /// Graph properties
        inline uint8_t getN() { return _n; }
        inline uint8_t getNumberNonNeighbors() { return _n - _self->getInDegree() - 1; }
        
        /// In-neighbors
        // Add in-neighbor
        bool addInNeighbor(XBeeAddress64 a);
        bool addInNeighbor(uint32_t aLsb);
        bool addInNeighbor(XBeeAddress64 a, long lambdaMin, long lambdaMax);
        bool addInNeighbor(uint32_t aLsb, long lambdaMin, long lambdaMax);
        // Determine if vertex is in-neighbor
        bool isInNeighbor(XBeeAddress64 a);
        bool isInNeighbor(uint32_t aLsb);
        bool isInNeighbor(XBeeAddress64 a, uint8_t &i);
        bool isInNeighbor(uint32_t aLsb, uint8_t &i);
        bool isInNeighbor(uint32_t aLsb, ORemoteVertex * &v);
        
        /// Non-neighbors
        // Add non-neighbor to array
        bool addNonNeighbor(XBeeAddress64 a);
        bool addNonNeighbor(uint32_t aLsb);
        bool addNonNeighbor(XBeeAddress64 a, long lambdaMin, long lambdaMax);
        bool addNonNeighbor(uint32_t aLsb, long lambdaMin, long lambdaMax);
        // Determine if vertex is non-neighbor
        bool isNonNeighbor(uint32_t aLsb);
        bool isNonNeighbor(XBeeAddress64 a);
        bool isNonNeighbor(uint32_t aLsb, uint8_t &i);
        bool isNonNeighbor(XBeeAddress64 a, uint8_t &i);
                        
        /// Common vertices
        // Methods to clear various states
        void clearAllNuTau();
        void clearAllYMinYMax();
        void clearAllInStates();
        void clearAllBroadcastStates();
        void clearAllBroadcastLambda();
        void clearAllStates();
        // Methods to get pointer to vertex
        OVertex * getVertexByAddressLsb(uint32_t aLsb);
        OVertex * getVertexByAddressLsb(uint32_t aLsb, uint8_t &i);
        OVertex * getVertexByAddress(XBeeAddress64 a);
        OVertex * getVertexByUniqueID(uint8_t i);
        ORemoteVertex * getRemoteVertex(uint8_t i);
	private:
        /// Properties
        uint8_t _n;                         // Total number of vertices
        OLocalVertex * _self;               // Local vertex
        ORemoteVertex _remoteVertices[NUM_REMOTE_VERTICES];
        
        // Methods
        uint8_t _getRemoteVertexIndex(XBeeAddress64 a);
    	uint8_t _getRemoteVertexIndex(uint32_t aLsb);
        bool _isRemoteVertex(uint32_t aLsb);
        bool _isRemoteVertex(uint32_t aLsb, uint8_t &i);
        bool _addRemoteVertex(uint32_t aLsb, long lambdaMin, long lambdaMax, bool inNeighbor = false);
};

#endif // OGraph_h