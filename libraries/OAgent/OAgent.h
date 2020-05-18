/*
 *  OAgent.h
 *  
 *
 *  Created by Stanton T. Cady on 06/15/12
 *  Edited by Laura Haller on 01/11/13
 *  Last edited by Stanton T. Cady on 01/16/13
 *
 */

#ifndef OAgent_h
#define OAgent_h


//#define VERBOSE

#include "XBee.h"   // Include header for xbee api
#include "OGraph.h"

#define SCHEDULE_MAXMIN_HEADER           0x7330 // schedule coordinate header is ascii s0
#define SCHEDULE_MAXMIN_ACK_HEADER       0x7332 // schedule coordinate header
#define SCHEDULE_MAXMIN_ACKACK_HEADER    0x7400 // schedule coordinate header
#define SCHEDULE_PD_HEADER               0x7340 // schedule coordinate header is ascii s@
#define SCHEDULE_ED_HEADER               0x7320 // schedule coordinate header is ascii s@
#define SCHEDULE_FAIR_SPLIT_HEADER       0x8346 // schedule coordinate header is ascii sF
#define FAIR_SPLITTING_HEADER            0x6653 // fair splitting ratio-consensus header is ascii fS
#define MAXMIN_HEADER                    0x6650 // maxmin consensus header is ascii fP
#define PD_HEADER                        0x7550 // Unicast Primal-dual header is ascii uP
#define ED_HEADER                        0x7540 // Unicast Primal-dual header is ascii uP
#define PD_ACK_HEADER                    0x6B50 // Primal-dual acknowledgment header is kP

#define WINDOW_LENGTH                    5000     // time length for each window in a period
#define BASE                             10000000  // base for transmitting decimals

#define OPTIMAL_DISPATCH_HEADER          0x6f44 // optimal dispatch header is ascii oD
#define ACK_START_HEADER                 0x6B55 //acknowledgment header is ascii kU (used to ensure start packet has been received by all neighbor nodes)
//#define TEST_PACKET_HEADER               0x7450
//#define LAMBDA_MIN_MAX_HEADER            0x624C // broadcast lambda header is ascii bL
#define SCHEDULE_OPTIMAL_DISPATCH_HEADER 0x7344 // schedule optimal dispatch header is ascii sD
#define INCLUDE_LAMBDA_MIN_MAX_SUBHEADER 0x0F   // include lambda min max subheader is 0b00001111
#define NO_LAMBDA_MIN_MAX_SUBHEADER      0x03   // no lambda min max subheader is 0b00000011
#define SYNC_BEGIN_HEADER                0x7342 // HRTS sync_begin header is ascii sB
#define SYNC_RESPONSE_HEADER             0x7352 // HRTS sync_response header is ascii sR
#define SYNC_FINAL_HEADER                0x7346 // HRTS sync_final header is ascii sF
#define SYNC_TIMEOUT                     2500    // (was 2500) time out period to wait for response to HRTS sync_begin broadcast in milliseconds
#define ACK_TIMEOUT                      500    // time out period to wait for an ack
#define SCHEDULE_TIMEOUT                 5000    // time out period (in milliseconds) to wait for schedule packet from leader node
#define RC_DELAY                         5000    // delay before ratio consensus starts
#define MC_DELAY                         5000    // delay before maxmin consensus starts
#define PD_DELAY                         5000   // delay before primal dual algorithm starts
#define ED_DELAY                         5000   // delay before primal dual algorithm starts
#define SYNC_RETRY_PERIOD                250    // period to wait between broadcasting HRTS sync_begin packet
#define SYNC_ERROR                       8      // calibrate for small amount of error
#define RESYNC_HEADER                    0x7353 // used as the header to indicate the resync process is taking place (1st transaction)
#define RESYNC_RESPONSE_HEADER           0x7354
#define RESYNC_HEADER_FINAL              0x7355 //used as the header to indicate the resync process is taking place (final transaction)     
#define RESYNC_TOTAL_TIME                500    //total time resync period will last
#define RESYNC_BROADCAST_TIME            150    //time spent by all synced nodes broadcasting its 1st resync packet for any potential unsynced nodes
     
#define LOG_TORQUE_ERROR_KEY             0x45   // log torque error key is ascii E
#define LOG_INITIAL_TORQUE_KEY           0x49   // log initial torque key is ascii I
#define LOG_GAMMA_KEY                    0x47   // log gamma key is ascii G  
#define LOG_LAMBDA_KEY                   0x4C   // log gamma key is ascii L 

class OAgent {
    public:
        /// Constructors
        OAgent();
        OAgent(XBee * xbee, OGraph * G, bool leader = false, bool quiet = true);
        OAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader = false, bool quiet = true);
        //OAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader = false, bool quiet = true, int RS = 0);
        
        /// Methods
        inline OGraph * getGraph() { return _G; }
        //unsigned int getRxAddress();
        //void broadcastTestPacket();
        //bool waitForTestPacket();
        
        // Get / set leader directive
        inline void setLeader(bool leader) { _leader = leader; }
        inline bool isLeader() { return _leader; }
        
        // Get / set quiet directive
        inline void setQuiet(bool quiet) { _quiet = quiet; }
        inline bool isQuiet() { return _quiet; }

        // Set Resilience Strategy for Agent
        //inline void setRS(int RS) { _RS = RS;}
        
        // Fair splitting methods
        float fairSplitRatioConsensus(long y, long z, uint8_t iterations, uint16_t period);
        // long computeFairSplitFinalValue(float gamma);
        long leaderFairSplitRatioConsensus(long y, long z, uint8_t iterations, uint16_t period);
        long nonleaderFairSplitRatioConsensus(long y, long z);
        
        // Resilient consensus methods
        float fairSplitRatioConsensus_RSL(float y, float z, uint8_t iterations, uint16_t period, uint8_t diameter, float eps);
        float leaderFairSplitRatioConsensus_RSL(float y, float z, uint8_t iterations, uint16_t period, uint8_t diameter, float eps);
        float nonleaderFairSplitRatioConsensus_RSL(float y, float z, uint8_t iterations, uint16_t period, uint8_t diameter, float eps);
        void maxMinConsensus_RSL(OLocalVertex * s, float Epsilon, uint8_t diameter, uint16_t period);
        void leaderMaxMinConsensus_RSL(OLocalVertex * s, float Epsilon, uint8_t diameter, uint16_t period);
        void nonleaderMaxMinConsensus_RSL(OLocalVertex * s, float Epsilon, uint8_t diameter, uint16_t period);
        void maxMin_RSL(OLocalVertex * s, float Epsilon, uint8_t diameter, uint16_t period);
 
        float ratiomaxminConsensus(float y, float z, uint8_t iterations, uint16_t period, uint8_t diameter, float eps);

        long maxminConsensusAlgorithm(bool isMax, long max, long min, uint8_t iterations, uint16_t period);
        long leaderMaxMinConsensus(bool isMax, long max, long min, uint8_t iterations, uint16_t period);
        long nonleaderMaxMinConsensus(bool isMax, long max, long min, uint8_t iterations, uint16_t period);
        long maxminConsensus(bool isMax, long max, long min, uint8_t iterations, uint16_t period);
        
        // Primal Dual methods
        bool primalDualAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool leaderPrimalDualAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool nonleaderPrimalDualAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool standardPrimalDualAlgorithm0(bool genBus, float alpha, uint16_t iterations);                       //includes a minimization of voltage deviations form nominal value
        bool acceleratedPrimalDualAlgorithm0(bool genBus, float alpha, uint16_t iterations);                    //includes a minimization of voltage deviations form nominal value
        bool standardPrimalDualAlgorithm1(bool genBus, float alpha, uint16_t iterations);                       //includes a minimization of voltage deviations form nominal value
        bool acceleratedPrimalDualAlgorithm1(bool genBus, float alpha, uint16_t iterations);                    //includes a minimization of voltage deviations form nominal value
        bool standardPrimalDualAlgorithm2(bool genBus, float alpha, uint16_t iterations);                       //includes a minimization of line losses
        bool acceleratedPrimalDualAlgorithm2(bool genBus, float alpha, uint16_t iterations);                    //includes a minimization of line losses
                       //includes a minimization of line losses
        // Economic Dispatch Algorithm
        bool economicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool leaderEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool nonleaderEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool standardEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations);
        bool acceleratedEconomicDispatchAlgorithm(bool genBus, float alpha, uint16_t iterations);

        // communication link activation methods
        bool linkActivationAlgorithm();

        // HRT Synchronization methods
        bool resync();
        bool sync(uint8_t attempts = 10);
        inline bool isSynced() { return _synced; }
        unsigned long myMillis();
        inline float getbufferdata(int index){ return _buffer[index]; }
        //Sid
        inline float getbufferdataY(int index){ return _bufferY[index]; }
        inline float getbufferdataZ(int index){ return _bufferZ[index]; }
        inline long getoffsetdata(){ return _offset; }
        //Sid
        inline long getbuffer2() {return _buffer2; }
        uint8_t getStatusData(uint8_t neighborID);
        //Olaolu
        //inline void setneighborY0(int index, float y) { _neighborY0[index] = y; }
        //inline void setneighborZ0(int index, float z) { _neighborZ0[index] = z; }
        //inline float getneighborY0(int index){ return _neighborY0[index]; }
        //inline float getneighborZ0(int index){ return _neighborZ0[index]; }
               
        //Distribute VC
        float voltageControl_dist( float diffV, float Vref, float secPercentage, float p, float q, float qtop, float qbottom, float Sij, float alphaVC, uint8_t iterations, uint16_t period , uint8_t diameter, float eps);
        void firstStageControl(OLocalVertex * s);
        void secondStageControl( OLocalVertex * s, uint8_t iterations, uint16_t period, uint8_t diameter, float eps );
        void isOverVoltage(OLocalVertex * s);
        void isUnderVoltage(OLocalVertex * s);       
        void shareFlag(OLocalVertex * s, uint8_t iterations, uint16_t period );
        bool getSecondStageFlagfromPackage(OLocalVertex * s);

    private:
        // Properties
        // XBee stuff
        XBee * _xbee;
        ZBRxResponse * _rx;
        ZBTxRequest _zbTx;
        ZBTxStatusResponse _txStatus;
        XBeeAddress64 _broadcastAddress;
        uint32_t _aMsb;
        uint32_t _availableAgentLsb[8];
        
        //Graph
        OGraph * _G;

        //Agent properties
        bool _leader;
        bool _quiet;
        bool _synced;
        long _offset;
        unsigned long _start_millis;
        uint8_t _iterations;
        uint16_t _period;
        uint16_t _windowsPerPeriod;

        //Resilience Strategy Property
        //int _RS;
        
        //Sammy's addition to contain iterates
        float _buffer[200];
        //Sid
        float _bufferY[200];
        float _bufferZ[200];
        //Sid
        long _buffer2;

        int node_counter[NUM_REMOTE_VERTICES];          //a counter for each neighbor (defined based on max number) which increments when data is NOT received at a ratio-consensus iteration and resets when data is received
        //Methods
        //Fair splitting
        inline void _waitForScheduleFairSplitPacket(unsigned long &startTime, uint8_t &iterations, uint16_t &period, uint8_t id,int timeout = -1) {
            _waitForSchedulePacket(SCHEDULE_FAIR_SPLIT_HEADER,startTime,iterations,period,id,timeout);
        }
        //Resilient version
        inline bool _waitForScheduleFairSplitPacket_RSL(unsigned long &startTime, uint8_t &iterations, uint16_t &period,int timeout) {
            return _waitForSchedulePacket_RSL(SCHEDULE_FAIR_SPLIT_HEADER,startTime,iterations,period,timeout);
        }
        inline bool _waitForScheduleMaxMinPacket_RSL(unsigned long &startTime, uint8_t &iterations, uint16_t &period,int timeout) {
            return _waitForSchedulePacket_RSL(SCHEDULE_MAXMIN_HEADER,startTime,iterations,period,timeout);
        }
        inline void _broadcastScheduleFairSplitPacket(unsigned long startTime, uint8_t iterations, uint16_t period) {
            _broadcastSchedulePacket(SCHEDULE_FAIR_SPLIT_HEADER,startTime,iterations,period);
        }
        inline void _broadcastSchedulePrimalDualPacket(unsigned long startTime, uint8_t iterations, uint16_t period) {
            _broadcastSchedulePacket(SCHEDULE_PD_HEADER,startTime,iterations,period);
        }
        inline bool _fairSplitPacketAvailable() { return _packetAvailable(FAIR_SPLITTING_HEADER,true); }
        inline bool _primaldualPacketAvailable() { return _packetAvailable(PD_HEADER,true); }
        inline bool _maxminPacketAvailable() { return _packetAvailable(MAXMIN_HEADER,true); }
        inline bool _maxminPacketAvailable_RSL() { return _packetAvailable(MAXMIN_HEADER,true); }//(diego)
        //Primal Dual Algorithm
        //inline bool PacketAvailable() { return _packetAvailable(PD_PACKET_HEADER,true); }
        void _initializeFairSplitting(OLocalVertex * s, long y, long z);
        //Leader failure-resilient version
        void _initializeFairSplitting_RSL(OLocalVertex * s, float y, float z, float eps);

        void _broadcastFairSplitPacket(OLocalVertex * s);
        //Leader failure-resilient version
        void _broadcastFairSplitPacket_RSL(OLocalVertex * s);
        
        void _broadcastMaxMinPacket(long max, long min);
        void _broadcastMaxMinPacket_RSL(OLocalVertex * s);//(diego)

        //Unicast Primal Dual Packet - SN addition edited by Olaolu
        void _unicastPacket_P(uint16_t recipientID, float fP, float fQ, float Lambda, bool flag);
        void _unicastPacket_C(uint16_t recipientID, float fP_c, float fQ_c, float Lambda_c, bool flag, float fP_p, float fQ_p, float Lambda_p);
        void _sendToChild(uint16_t recipientID, float fP, float fQ, float Lambda, float gfP, float gfQ, float gLambda, bool flag);
        void _sendToParent(uint16_t recipientID, float fP, float fQ, float Lambda, float gfP, float gfQ, float gLambda, bool flag, float fP_p, float fQ_p, float Lambda_p, float gfP_p, float gfQ_p, float gLambda_p);

        //Multicast Economic Dispatch Packet
        void _sendToNeighbor(float sumLAMBDA, float sumNU);
        void _sendToNeighbor_ACC(float sumLAMBDA, float sumNU, float sumGAMMA);

        //link activation packets - added by Olaolu
        void _candactcodePacket(uint16_t recipientID);
        void _actcodePacket(uint16_t recipientID, uint8_t actcode);
        void _linksactPacket(uint16_t recipientID);

        ////link activation methods - added by Olaolu
        uint8_t _assignLinkACTCODES();
        void _listenForLinkACTCODES(int timeout);

        inline uint8_t _getACTCODEFromPacket() {  return _rx->getData(4); }
        inline uint8_t _getCANDACTCODEFromPacket(uint8_t i) {  return _rx->getData(4+i); }

        //methods to get packet data
        long _getMaxFromPacket();
        long _getMinFromPacket();
        float _getMaxFromPacket_RSL();
        float _getMinFromPacket_RSL();
        float _getMuFromPacket();
        float _getSigmaFromPacket();
        long _getpacketcheck();
        bool _getSecondStageFlagfromPackage();
        
        // Methods dealing with packets
        void _broadcastOptimalDispatchPacket(OLocalVertex * s);
        inline bool _optimalDispatchPacketAvailable() { return _packetAvailable(OPTIMAL_DISPATCH_HEADER,true); }
        void _processOptimalDispatchPacket(OLocalVertex * s, uint8_t i);
        uint8_t _getPayloadSize();
        uint8_t _getNumberLambdaMinMax();
        uint8_t _addDataToPayload(OVertex * v, uint8_t payload[], uint8_t ptr);

        void _broadcastACKPacket(uint16_t header, uint8_t recipientID);
        void _broadcastHeaderPacket(uint16_t header);

        
        // Methods for finding final solution
        void _computeFinalOptimalDispatchRatios(OLocalVertex * s, float ratios[]);
        //uint8_t _addRatioToArray(OVertex * v, long &z, float ratios[], uint8_t ptr);
        inline float _computeRatio(long num, long dem) { return float(num)/float(dem); }
        void _printRatios(float ratios[], uint8_t num);
        void _findMinRatioMaxRatio(float ratios[], uint8_t num, float &min, float &max);
        void _printFinalYMinYMax(OLocalVertex * s);
        void _printStates(OLocalVertex * s, uint8_t k, bool printY = false);
        void _printLambdas(OLocalVertex * s);
        long _findLambdaStar(OLocalVertex * s);
        void _updateLambdaMinLambdaMax(OVertex * v, long &z, float &min, float &max, long &lambdaMinus, long &lambdaPlus);
        uint8_t _checkRatio(float &min, float &max, float ratio);
        
        // Synchronization helper functions
        bool _leaderSync();
        bool _targetSync(unsigned long tTwo);
        bool _nonTargetSync(unsigned long tTwo);
        unsigned long _broadcastSyncBeginPacket(uint8_t i);
        void _broadcastSyncFinalPacket(unsigned long tTwo, long d);
        bool _unicastSyncResponsePacket(unsigned long tTwo);
        inline int /*bool*/ _waitForSyncBeginPacket(unsigned long &rxTime) { return _waitForPacket2(SYNC_BEGIN_HEADER, RESYNC_HEADER,rxTime,true);  /*_waitForPacket(SYNC_BEGIN_HEADER,rxTime,true,-1);*/ }
        inline bool _waitForSyncResponsePacket(unsigned long &rxTime) { return _waitForPacket(SYNC_RESPONSE_HEADER,rxTime,false,SYNC_TIMEOUT); }
        inline bool _waitForSyncFinalPacket(int timeout = -1) { return _waitForPacket(SYNC_FINAL_HEADER,true,timeout); }
        bool _isTargetNode();
        //new additions 
        
        unsigned long _broadcastResyncBeginPacket(unsigned long period);
        void _broadcastResyncFinalPacket(long offset);
        void _broadcastResyncResponsePacket(unsigned long t2, unsigned long node_id);
        bool _waitForResyncPacketResponse(unsigned long &rxTime, uint16_t timeout);
        int  _waitForPacket2(uint16_t header1, uint16_t header2, unsigned long &rxTime, bool broadcast = false);
        bool  _packetAvailable2(unsigned long &rxTime, bool broadcast = false);
        bool _waitForResyncFinalPacket(unsigned long timeout);
        
        // General xbee methods
        bool _validPacketAvailable();
        bool _packetAvailable(uint16_t header, bool broadcast = false);
        bool _packetAvailable(uint16_t header, unsigned long &rxTime, bool broadcast = false);
        uint16_t _packetAvailable(bool broadcast);
        bool _packetAvailableHelper(uint16_t header, bool broadcast = false);
        bool _packetACKed(int timeout);
        inline uint16_t _getHeaderFromPacket() { return (uint16_t(_rx->getData(1)) << 8) + _rx->getData(0); }
        inline uint16_t _getNeighborIDFromPacket()    { return (uint16_t(_rx->getData(11)) << 8) + _rx->getData(10);  }

        //Primal Dual Algorithm
        float _getActiveFlowFromPacket_neighbor();
        float _getReactiveFlowFromPacket_neighbor();
        float _getLambdaFromPacket_neighbor();
        float _getActiveFlowGradientFromPacket_neighbor();
        float _getReactiveFlowGradientFromPacket_neighbor();
        float _getLambdaGradientFromPacket_neighbor();
        
        bool _getFlagFromPacket();
        bool _getFlagFromPacket_ACC();

        float _getActiveFlowFromPacket_node();
        float _getReactiveFlowFromPacket_node();
        float _getLambdaFromPacket_node();
        
        float _getActiveFlowFromPacket_nodeACC();
        float _getReactiveFlowFromPacket_nodeACC();
        float _getLambdaFromPacket_nodeACC();
        float _getActiveFlowGradientFromPacket_node();
        float _getReactiveFlowGradientFromPacket_node();
        float _getLambdaGradientFromPacket_node();
        
        inline uint16_t _getRecipientIDFromPacket()    { return (uint16_t(_rx->getData(3)) << 8) + _rx->getData(2);  }        

        //Economic Dispatch Algorithm
        float _getSumLambdaFromPacket();
        float _getSumNuFromPacket();
        float _getSumGammaFromPacket();        

        //inline uint16_t _getinheritorIDFromPacket()    { return (uint16_t(_rx->getData(13)) << 8) + _rx->getData(12);  }
        //inline uint16_t _getleaderIDFromPacket()    { return (uint16_t(_rx->getData(15)) << 8) + _rx->getData(14);  }
        //inline uint16_t _getdeputyIDFromPacket()    { return (uint16_t(_rx->getData(17)) << 8) + _rx->getData(16);  }
        bool _waitForPacket(uint16_t header, unsigned long &rxTime, bool broadcast = false, int timeout = -1);
        bool _waitForPacket(uint16_t header, bool broadcast = false, int timeout = -1);
        uint16_t _waitForValidPacket(bool broadcast = false, int timeout = -1);
        
        // General scheduling methods
        void _waitForACKPacket(uint16_t header, unsigned long t0, unsigned long startTime, uint8_t iterations, uint16_t period);// General scheduling methods
        bool _waitForChildSchedulePacketRC(uint16_t header, int timeout, unsigned long startTime, uint8_t iterations, uint16_t period );
        bool _waitForChildSchedulePacketMaxMin(uint16_t header, int timeout, unsigned long startTime, uint8_t iterations, uint16_t period );
        bool _waitForChildSchedulePacketPD(int timeout, unsigned long startTime, uint16_t iterations);
        bool _waitForChildSchedulePacketED(int timeout, unsigned long startTime, uint16_t iterations);

        inline uint8_t _getIDFromPacket() {  return _rx->getData(2); }

        bool _waitToStart(unsigned long startTime, bool useMyMillis, int timeout = -1);
        inline uint32_t _getStartTimeFromPacket() {
            return (uint32_t(_rx->getData(5)) << 24) + (uint32_t(_rx->getData(4)) << 16) + (uint16_t(_rx->getData(3)) << 8) + _rx->getData(2);
        }
        inline uint8_t _getIterationsFromPacket() { return _rx->getData(6); }
        inline uint16_t _getPeriodFromPacket() { return (uint16_t(_rx->getData(8)) << 8) + _rx->getData(7); }
        inline uint16_t _getIterationsFromPacketPD() { return (uint16_t(_rx->getData(7)) << 8) + _rx->getData(6); }
        
        // Generate transmit time
        uint16_t _genTxTime(uint16_t iterationPeriod, uint8_t ITF);
        uint16_t _genTxTime(uint16_t iterationPeriod, uint8_t ITF, int seed);
        
        // General coordination helper functions
        bool _timeToTransmit(uint16_t startTime, uint16_t txTime);
        void _waitForSchedulePacket(uint16_t header, unsigned long &startTime, uint8_t &iterations, uint16_t &period, uint8_t id, int timeout);
        bool _waitForSchedulePacket_RSL(uint16_t header, unsigned long &startTime, uint8_t &iterations, uint16_t &period, int timeout);
        bool _waitForParentSchedulePacketRC(unsigned long &startTime, uint8_t &iterations, uint16_t &period,int timeout);
        bool _waitForParentSchedulePacketMaxMin(unsigned long &startTime, uint8_t &iterations, uint16_t &period,int timeout);        
        bool _waitForParentSchedulePacketPD(unsigned long &startTime, uint16_t &iterations,int timeout);
        bool _waitForParentSchedulePacketED(unsigned long &startTime, uint16_t &iterations,int timeout);
        bool _waitForNeighborPacket(uint8_t &neighborID, uint16_t header, bool broadcast, int timeout);
        bool _waitForUnicastPacket(uint8_t &neighborID, uint8_t nodeID, uint16_t header, bool broadcast, int timeout);
        void _broadcastSchedulePacket(uint16_t header, unsigned long startTime, uint8_t numIterations, uint16_t period);
        void _broadcastSchedulePacketPD(unsigned long startTime, uint16_t numIterations);
        void _broadcastSchedulePacketED(unsigned long startTime, uint16_t numIterations);
        uint32_t _getAvailableAgentLsb(uint8_t i);
        uint32_t _getUint32_tFromPacket(uint8_t &lsbByteNumber);
        inline long _getLongFromPacket(uint8_t &lsbByteNumber) { return long(_getUint32_tFromPacket(lsbByteNumber)); }
        uint8_t _getUint8_tFromPacket(uint8_t &byteNumber);
        uint8_t _addUint32_tToPayload(uint32_t data, uint8_t payload[], uint8_t ptr);
        
        // Constructor helper function
        void _prepareOAgent(XBee * xbee, ZBRxResponse * rx, OGraph * G, bool leader = false, bool quiet = true);

        //voltage control functions
        void _initializeVoltageControl( OLocalVertex * s, float diffV, float Vref, float secPercentage, float p, float q, float qtop, float qbottom, float S, float alphaVC );
        void _initializeVariablesSecStage(OLocalVertex * s);
};

#endif // OAgent_h
