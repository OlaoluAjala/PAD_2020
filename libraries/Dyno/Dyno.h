/*
 *  Dyno.h
 *  
 *  Created by Stanton T. Cady on 01/11/12
 * Last edited by Stanton T. Cady on 1/27/13
 *  
 *
 */

#ifndef Dyno_h
#define Dyno_h

#include "HardwareSerial.h"
#include <inttypes.h>

#define SET_TORQUE_HEADER   'sT'
#define SET_VELOCITY_HEADER 'sV'
#define SET_CURRENT_HEADER  'sI'

#define LOG_SUBHEADER		'l'

#define GET_TORQUE_HEADER   'gT'
#define GET_VELOCITY_HEADER 'gV'
#define GET_OTORQUE_HEADER	'gO'
#define NEWLINE_CHAR		0x0D

class Dyno {
    public:
        Dyno() { _timeout = 1000; _float_scale = 100; }
		void flushSerial();
        void setTorque(float t);
        void setTorqueInt(uint16_t t);
        void setTorque(uint16_t t);
        void setVelocity(uint16_t v);
        void setCurrent(float i);
        float getTorqueFloat();
        uint16_t getTorqueInt();
		uint16_t getOTorqueInt();
		float getOTorqueFloat();
        uint16_t getVelocity();
        uint16_t getCurrent();
        void setSerial(HardwareSerial s);
		void sendShortLogData(uint8_t key, uint16_t value);
		void sendLogData(uint8_t key, uint32_t value);
		uint16_t _readUintUntil();
		inline void setTimeout(unsigned long timeout) { _timeout = timeout; }
		// inline void setPeekTimeout(int timeout) { _peek_timeout = timeout; }
		
    private:
		// Properties
	    unsigned long _timeout;
		// int _peek_timeout;
	    unsigned long _start_timeout;
		// int _peek_start_timeout;
		uint16_t _read_byte;
		float _float_scale;
		// Methods
		void _sendHeader(uint16_t header);
		float _sendReadFloatCommand(uint16_t header);
		float _getFloat(uint16_t header);
        void _sendWriteCommand(uint16_t header, uint16_t payload);
		void _sendLongWriteCommand(uint16_t header, uint32_t payload);
        uint16_t _sendReadCommand(uint16_t header);
        void _sendByte(uint8_t b);
        void _sendTwoBytes(uint16_t bs);
		// int _timedPeek();
};

#endif // Dyno_h

