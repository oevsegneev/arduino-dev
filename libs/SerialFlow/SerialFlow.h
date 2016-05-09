/*
  SerialFlow.h - Communication library with packages of values.
  Created by Oleg Evsegneev, September, 2012.
  Last update 09.05.2016
  Released into the public domain.
  Ver 0.7 (for Arduino)
*/
#ifndef SerialFlow_h
#define SerialFlow_h

#include "Arduino.h"

#ifdef FastSerial_h
#include <FastSerial.h>
#else
#include <HardwareSerial.h>
#endif

#define MAX_PACKET_SIZE 128

class SerialFlow {
public:
    #ifdef FastSerial_h
    SerialFlow( FastSerial *serial );
    #else
    SerialFlow( HardwareSerial *serial );
    #endif

    /** Set data packet format
     *
     * @param v_length Length of value in bytes.
     * @param p_size Number of values.
     */
    void setPacketFormat( byte v_length, byte p_size, boolean separate );

    /**  Initialize port
     * @param baud_rate Serial port baud rate.    
     */
    void begin( long baud_rate );

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
    uint32_t getPacket( byte idx );

    /**  Serial write
     * @param v Byte to send.    
     */
    void write( byte v );

    /**  Serial read
     */
    byte read();

protected:
    #ifdef FastSerial_h
    FastSerial *_serial;
    #else
    HardwareSerial *_serial;
    #endif

    boolean _separate;
    byte _p_size;
    byte _v_length;

    uint32_t _vs[MAX_PACKET_SIZE];
    byte _vs_idx;

    uint32_t _vr[MAX_PACKET_SIZE];
    byte _vr_val[4];
    byte _vr_idx;
    byte _cr_idx;
    bool _escape;
    bool _collecting;


    uint32_t _join_bytes(byte *bs);
};

#endif
