/*
  SerialFlow.cpp - Communication library with packages of values.
  Created by Oleg Evsegneev, September 11, 2012.
  Last update 13.11.2016
  Released into the public domain.
*/
#include "SerialFlow.h"

#if defined FastSerial_h
SerialFlow::SerialFlow( FastSerial *serial ) {
    _serial = serial;
#elif defined __RF24_H__
SerialFlow::SerialFlow( uint8_t cn, uint8_t csn ) {
	_serial = new RF24(cn, csn);
#else
SerialFlow::SerialFlow( HardwareSerial *serial ) {
    _serial = serial;
#endif
    _escape = 0;
    _collecting = 0;
}

#ifdef __RF24_H__
void SerialFlow::begin( uint64_t address1, uint64_t address2 ) {
	_serial->begin();
	_serial->setRetries( RF24_REPEAT_DELAY, RF24_REPEAT_COUNT );
	_serial->setPayloadSize( RF24_PAYLOAD );
	
	_serial->openWritingPipe( address1 );
	_serial->openReadingPipe( 1, address2 );
	
    _serial->startListening();	
}
#else
void SerialFlow::begin( uint16_t baud_rate ) {
    _serial->begin(baud_rate);
}
#endif

void SerialFlow::setPacketFormat( uint8_t v_length, uint8_t p_size, boolean separate ) {
    _separate = separate;
    _p_size = p_size;
    _v_length = v_length;
    _vs_idx = 0;
    _vr_idx = 0;
}

void SerialFlow::setPacketFormat( uint8_t v_length, uint8_t p_size ) {
    _separate = 0;
    _p_size = p_size;
    _v_length = v_length;
    _vs_idx = 0;
    _vr_idx = 0;
}

void SerialFlow::setPacketValue( uint32_t value ) {
    if( _vs_idx < _p_size ){
        _vs[_vs_idx++] = value;
    }
}

void SerialFlow::sendPacket() {
    uint8_t v;
    write( 0x12 );
    for( byte i=0; i<_vs_idx; i++ ){
        for( byte b=0; b<_v_length; b++ ){
            v = (_vs[i]>>(b<<3)) & 0xFF;
            if( v==0x12 || v==0x13 || v==0x7D || ( _separate && v==0x10 ))
                write( 0x7D );
            write( v );
        }

        // _separate values
        if( _separate && i < _vs_idx-1 )
            write(0x10);
    }
        
    write( 0x13 );
    _vs_idx = 0;
}

bool SerialFlow::receivePacket() {
    uint8_t c;
    while( _serial->available() ){
        c = read();
        if( _collecting )
            if( _escape ){
                _vr_val[_cr_idx++] = c;
                _escape = 0;
            }
            // escape
            else if( c == 0x7D ){
                _escape = 1;
            }
            // value separator
            else if( _separate && c == 0x10 ){
                _vr[_vr_idx++] = _join_bytes(&_vr_val[0]);
                _cr_idx = 0;
            }    
            // end
            else if( c == 0x13 ){
                if( _separate )
                    _vr[_vr_idx++] = _join_bytes(&_vr_val[0]);
                _collecting = 0;
                return 1;
            }    
            else{
                _vr_val[_cr_idx++] = c;
                if( !_separate && _cr_idx == _v_length ){
                    _vr[_vr_idx++] = _join_bytes(&_vr_val[0]);
                    _cr_idx = 0;
                }
            }    
        // begin
        else if( c == 0x12 ){
            _collecting = 1;
            _cr_idx = 0;
            _vr_idx = 0;
        }    
    }
    return 0;
}

uint32_t SerialFlow::getPacketValue( uint8_t idx ) {
    return _vr[idx];
}

void SerialFlow::write( uint8_t v ) {
	#ifdef __RF24_H__
    _serial->stopListening();
    _serial->write( &v, 1);
    _serial->startListening();	
	#else
    _serial->write(v);
	#endif
}

uint8_t SerialFlow::read() {
	#ifdef __RF24_H__
	uint8_t v;
	_serial->read( &v, 1 );
	return v;
	#else
    return _serial->read();
	#endif
}

uint32_t SerialFlow::_join_bytes(uint8_t *bs) {
    uint32_t v = 0;
    uint32_t x = 0;
    for( byte b=0; b<_v_length; b++ ){
        x = bs[b];
        v |= (x << (b<<3));
    }
    return v;
}

void SerialFlow::getHWInfo(){
    #ifdef __RF24_H__
    _serial->printDetails();
    #endif
}