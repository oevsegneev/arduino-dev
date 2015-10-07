/*
  SerialFlow.cpp - Communication library with packages of values.
  Created by Oleg Evsegneev, September 11, 2012.
  Last update 08.10.2015
  Released into the public domain.
*/
#include "SerialFlow.h"

SerialFlow::SerialFlow( HardwareSerial *serial ) {
    _escape = 0;
    _collecting = 0;
    _serial = serial;
}

void SerialFlow::begin( long baud_rate ) {
    _serial->begin(baud_rate);
}

void SerialFlow::setPacketFormat( byte v_length, byte p_size, boolean separate ) {
    _separate = separate;
    _p_size = p_size;
    _v_length = v_length;
    _vs_idx = 0;
    _vr_idx = 0;
}

void SerialFlow::setPacketValue( short value ) {
    if( _vs_idx < _p_size ){
        _vs[_vs_idx++] = value;
    }
}

void SerialFlow::sendPacket() {
    byte v;
    _serial->write( 0x12 );
    for( byte i=0; i<_vs_idx; i++ ){
        // low byte
        v = _vs[i] & 0xFF;
        if( v==0x12 || v==0x13 || v==0x7D || ( _separate && v==0x10 ))
            _serial->write( 0x7D );
        _serial->write( v );

        // high byte
        if( _v_length>1 ){
            v = (_vs[i]>>8) & 0xFF;
            if( v==0x12 || v==0x13 || v==0x7D || ( _separate && v==0x10 ))
                _serial->write( 0x7D );
            _serial->write( v );
        }
    
        // _separate values
        if( _separate && i < _vs_idx-1 )
            _serial->write(0x10);
    }
        
    _serial->write( 0x13 );
    _vs_idx = 0;
}

bool SerialFlow::receivePacket() {
    byte c;
    while( _serial->available() ){
        c = _serial->read();
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
                _vr[_vr_idx++] = _vr_val[0] | (_vr_val[1] << 8);
                _cr_idx = 0;
            }    
            // end
            else if( c == 0x13 ){
                if( _separate )
                    _vr[_vr_idx++] = _vr_val[0] | (_vr_val[1] << 8);
                _collecting = 0;
                return 1;
            }    
            else{
                _vr_val[_cr_idx++] = c;
                if( !_separate && _cr_idx == _v_length ){
                    _vr[_vr_idx++] = _vr_val[0] | (_vr_val[1] << 8);
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

short SerialFlow::getPacket( byte idx ) {
    return _vr[idx];
}

void SerialFlow::write( byte v ) {
    _serial->write(v);
}

byte SerialFlow::read() {
    return _serial->read();
}
