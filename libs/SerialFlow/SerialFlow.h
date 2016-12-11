/*
  SerialFlow.h - Communication library with packages of values.
  Created by Oleg Evsegneev, September, 2012.
  Last update 13.11.2016
  Released into the public domain.
  Ver 0.8 (for Arduino)
*/
#ifndef SerialFlow_h
#define SerialFlow_h

#include "Arduino.h"

// uncomment desired library
//#include <FastSerial.h>
//#include <RF24.h>
#include <HardwareSerial.h>

#define MAX_PACKET_SIZE 8

#if defined __RF24_H__
#define RF24_REPEAT_COUNT 1
#define RF24_REPEAT_DELAY 15
#define RF24_PAYLOAD 1
#endif

class SerialFlow {
public:
    #if defined FastSerial_h
    SerialFlow( FastSerial *serial );
    #elif defined __RF24_H__
	SerialFlow( uint8_t cn, uint8_t csn );
	#else
	SerialFlow( HardwareSerial *serial );
    #endif

    /** Set data packet format
     *
     * @param v_length Length of value in bytes.
     * @param p_size Number of values.
     * @param separate Flag to separate values by 0x10 char (optional).
     */
    void setPacketFormat( uint8_t v_length, uint8_t p_size, boolean separate );
    void setPacketFormat( uint8_t v_length, uint8_t p_size );

    /**  Initialize port
     * @param baud_rate Serial port baud rate.    
     */
    #ifdef __RF24_H__
	void begin( uint64_t address1, uint64_t address2 );
	#else
    void begin( uint16_t baud_rate );
	#endif

    /**  Set value to data packet
     *
     * @param value Value.
     */
    void setPacketValue(uint32_t value);
        
    /**  Send packet to serial port
     */
    void sendPacket();

    /**  Receive packet from serial port
     */
    bool receivePacket();

    /**  Get received packet
     * @param idx Index of value from packet.    
     */
    uint32_t getPacketValue( uint8_t idx );

    /**  Serial write
     * @param v Byte to send.    
     */
    void write( uint8_t v );

    /**  Serial read
     */
    uint8_t read();

    void getHWInfo();

protected:
    #ifdef FastSerial_h
    FastSerial *_serial;
    #elif defined __RF24_H__
	RF24 *_serial;
    #else
    HardwareSerial *_serial;
    #endif

    boolean _separate;
    uint8_t _p_size;
    uint8_t _v_length;

    uint32_t _vs[MAX_PACKET_SIZE];
    uint8_t _vs_idx;

    uint32_t _vr[MAX_PACKET_SIZE];
    uint8_t _vr_val[4];
    uint8_t _vr_idx;
    uint8_t _cr_idx;
    bool _escape;
    bool _collecting;


    uint32_t _join_bytes(uint8_t *bs);
};

#endif
