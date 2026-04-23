/*
  SerialFlow.cpp - Библиотека для пакетного обмена по последовательному порту
  Автор Олег Евсегнеев, 11.09.2012
  Последнее обновление 23.04.2026
  Лицензия MIT
*/
#include "SerialFlow.h"

#if defined FastSerial_h
SerialFlow::SerialFlow( FastSerial *serial ) {
    _serial = serial;
#elif defined RF24_H_
SerialFlow::SerialFlow( uint8_t cn, uint8_t csn ) {
	_serial = new RF24(cn, csn);
#else
SerialFlow::SerialFlow( HardwareSerial *serial ) {
    _serial = serial;
#endif
    _escape = 0;
    _collecting = 0;
}

#ifdef RF24_H_
void SerialFlow::begin( uint64_t address1, uint64_t address2, uint8_t channel = RF24_CHANNEL, rf24_datarate_e baud = RF24_250KBPS ) {
	_serial->begin();
    _serial->setChannel( channel );
	_serial->setRetries( RF24_REPEAT_DELAY, RF24_REPEAT_COUNT );
	_serial->setPayloadSize( RF24_PAYLOAD );
    _serial->setDataRate( baud );
	
	_serial->openWritingPipe( address1 );
	_serial->openReadingPipe( 1, address2 );
	
    _serial->startListening();	
}
#else
void SerialFlow::begin( uint32_t baud_rate ) {
    _serial->begin(baud_rate);
}
#endif

void SerialFlow::setPacketFormat( uint8_t v_length, uint8_t p_size ) {
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
    uint8_t payload_crc[MAX_PACKET_SIZE*4*2];
    uint8_t payload_idx = 0;

    payload_crc[payload_idx++] = 0x12;
    for( uint8_t i=0; i<_vs_idx; i++ ){
        for( uint8_t b=0; b<_v_length; b++ ){
            v = (_vs[i]>>(b<<3)) & 0xFF;
            if( v==0x12 || v==0x13 || v==0x7D )
                payload_crc[payload_idx++] = 0x7D;
            payload_crc[payload_idx++] = v;
        }
    }
        
    payload_crc[payload_idx++] = SerialFlow::_crc8(payload_crc, payload_idx);
    payload_crc[payload_idx++] = 0x13;
    
    write(payload_crc, payload_idx);

    _vs_idx = 0;
}

bool SerialFlow::receivePacket() {
    uint8_t c;

    while( _serial->available() ){
        c = read();
        if( _collecting ){
            #ifdef ENABLE_CRC
            _payload_crc[_payload_idx++] = c;
            #endif

            // сразу за экраном
            if( _escape ){
                _escape = 0;
            }
            // экран
            else if( c == 0x7D ){
                _escape = 1;
                continue;
            }
            // конец пакета
            else if( c == 0x13 ){
                if( !_vr_ready ){ // ошибка: размер пакета не соответствует
                    _collecting = 0;
                    continue;
                }

                #ifdef ENABLE_CRC
                if( SerialFlow::_crc8(_payload_crc, _payload_idx-2) != _crc ){ // ошибка: crc не соответствует
                    return 0;
                }
                #endif

                _collecting = 0;

                return 1;
            }    
            // ошибка: начало следующего пакета
            else if( c == 0x12 ){
                _collecting = 1;
                _cr_idx = 0;
                _vr_idx = 0;
                _vr_ready = 0;

                #ifdef ENABLE_CRC
                _crc_ready = 0;
                _payload_idx = 0;
                _payload_crc[_payload_idx++] = c;
                #endif
                continue;
            }

            if( _vr_ready ){ // ошибка: лишние данные в пакете
                #ifdef ENABLE_CRC
                if( !_crc_ready ){
                    _crc = c;
                    _crc_ready = 1;
                    continue;
                }
                #endif

                _collecting = 0;
                continue;
            }

            _vr_val[_cr_idx++] = c;
            if( _cr_idx == _v_length ){
                _vr[_vr_idx++] = _join_bytes(&_vr_val[0], _v_length);
                _cr_idx = 0;

                if( _vr_idx == _p_size ){
                    _vr_ready = 1;
                }
            }
        }
        // начало пакета
        else if( c == 0x12 ){
            _collecting = 1;
            _cr_idx = 0;
            _vr_idx = 0;
            _vr_ready = 0;

            #ifdef ENABLE_CRC
            _crc_ready = 0;
            _payload_idx = 0;
            _payload_crc[_payload_idx++] = c;
            #endif
        }    
    }
    return 0;
}

uint32_t SerialFlow::getPacketValue( uint8_t idx ) {
    return _vr[idx];
}

void SerialFlow::write( uint8_t v ) {
    #ifdef RF24_H_
    _serial->stopListening();
    _serial->write( &v, 1);
    _serial->startListening();	
    #else
    _serial->write( v );
    #endif
}

void SerialFlow::write( uint8_t *v, uint8_t length ) {
    #ifdef RF24_H_
    _serial->stopListening();
    for( uint8_t i=0; i<length; i++ )
        _serial->write( &v[i], 1);
    _serial->startListening();	
    #else
    for( uint8_t i=0; i<length; i++ )
        _serial->write( &v[i] );
    #endif
}

uint8_t SerialFlow::read() {
    #ifdef RF24_H_
    uint8_t v;
    _serial->read( &v, 1 );
    return v;
    #else
    return _serial->read();
    #endif
}

void SerialFlow::getHWInfo(){
    #ifdef RF24_H_
    _serial->printDetails();
    #endif
}

uint32_t SerialFlow::_join_bytes(uint8_t *bs, uint8_t length) {
    uint32_t v = 0;
    uint32_t x = 0;
    for( byte b=0; b<length; b++ ){
        x = bs[b];
        v |= (x << (b<<3));
    }
    return v;
}

uint8_t SerialFlow::_crc8(uint8_t *data, uint8_t length) {
    uint8_t crc = 0x00;

    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}