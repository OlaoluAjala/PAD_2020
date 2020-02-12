/*
 *  Dyno.cpp
 *  
 *
 *  Created by Stanton T. Cady on 01/11/12
 *  Last edited by Stanton T. Cady on 01/27/13
 *
 *
 */
 
#include "Dyno.h"
#include "HardwareSerial.h"
#include "Streaming.h"

// HardwareSerial _s = Serial;

void Dyno::flushSerial() {
	// while(_s.read() != -1);
}

void Dyno::setSerial(HardwareSerial s) {
    // _s = s;
}

void Dyno::setTorque(float t) {
    // _sendWriteCommand(SET_TORQUE_HEADER,uint16_t(round(t*_float_scale)));
}

void Dyno::setTorqueInt(uint16_t t) {
    // _sendWriteCommand(SET_TORQUE_HEADER,uint16_t(t));
}

void Dyno::setVelocity(uint16_t v) {
    // _sendWriteCommand(SET_VELOCITY_HEADER,v);
}

void Dyno::setCurrent(float i) {
    // _sendWriteCommand(SET_CURRENT_HEADER,uint16_t(i*float(10)));
}

float Dyno::getTorqueFloat() {
	// return _getFloat(GET_TORQUE_HEADER);
}

uint16_t Dyno::getTorqueInt() {
    // return _sendReadCommand(GET_TORQUE_HEADER);
}

uint16_t Dyno::getOTorqueInt() {
    // return _sendReadCommand(GET_OTORQUE_HEADER);
}

float Dyno::getOTorqueFloat() {
	// return _getFloat(GET_OTORQUE_HEADER);
}

uint16_t Dyno::getVelocity() {
    // return _sendReadCommand(GET_VELOCITY_HEADER);
}

void Dyno::sendLogData(uint8_t key, uint32_t value) {
	// _sendLongWriteCommand((uint16_t(LOG_SUBHEADER) << 8) + key,value);
}

void Dyno::_sendLongWriteCommand(uint16_t header, uint32_t payload) {
    _sendTwoBytes(header);
	//delay(10);
    _sendTwoBytes(payload >> 16);
	//delay(10);
	_sendTwoBytes(payload);
    _sendByte(0x0D);	
}

void Dyno::sendShortLogData(uint8_t key, uint16_t value) {
	// _sendWriteCommand((uint16_t(LOG_SUBHEADER) << 8) + key,value);
}

/// Private methods

float Dyno::_getFloat(uint16_t header) {
	uint16_t rsp = _sendReadCommand(header);
	return float(rsp)/_float_scale;
}

void Dyno::_sendWriteCommand(uint16_t header, uint16_t payload) {
    _sendTwoBytes(header);
    _sendTwoBytes(payload);
    _sendByte(NEWLINE_CHAR);
}

uint16_t Dyno::_sendReadCommand(uint16_t header) {
	flushSerial();
	_sendTwoBytes(header);		// send header
	//_sendByte(NEWLINE_CHAR);	// send new line char
	return _readUintUntil();	// look for unsigned int until timeout or 0x0D
}

void Dyno::_sendByte(uint8_t b) {
    // _s << b;			//analogous to cout<<
}

void Dyno::_sendTwoBytes(uint16_t bs) {
    _sendByte(uint8_t(bs >> 8));
    _sendByte(uint8_t(bs));    
}

uint16_t Dyno::_readUintUntil() {
	_start_timeout = millis();
	uint16_t value = 0;
	do {
		int c;
		// c = _s.peek();
		if(c != -1) {
			if(c != NEWLINE_CHAR)
				value = (value << 8) + uint16_t(c);
			// _s.read();
		}
		if(c == NEWLINE_CHAR)
			break;
	} while((millis() - _start_timeout) <= _timeout);
	return value;
}

// int Dyno::_timedPeek() {
// 	int c;
// 	_peek_start_timeout = millis();
// 	do {
// 		c = _s.peek();
// 		if (c != -1) return c;
// 	} while(millis() - _peek_start_timeout < _peek_timeout);
// 	return -1;     // -1 indicates timeout
// }