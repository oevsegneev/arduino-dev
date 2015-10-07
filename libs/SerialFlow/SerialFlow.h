/*
  SerialFlow.h - Communication library with packages of values.
  Created by Oleg Evsegneev, September, 2012.
  Last update 08.10.2015
  Released into the public domain.
  Ver 0.5 (for Arduino)
*/
#ifndef SerialFlow_h
#define SerialFlow_h

#include "Arduino.h"

#include <HardwareSerial.h>

#define MAX_PACKET_SIZE 128

class SerialFlow {
public:
    SerialFlow( HardwareSerial *serial );

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
    void setPacketValue(short value);
        
    /**  Send packet to serial port
     */
    void sendPacket();

    /**  Receive packet from serial port
     */
    bool receivePacket();

    /**  Get received packet
     * @param idx Index of value from packet.    
     */
    short getPacket( byte idx );

    /**  Serial write
     * @param v Byte to send.    
     */
    void write( byte v );

    /**  Serial read
     */
    byte read();

protected:
    HardwareSerial *_serial;

    boolean _separate;
    byte _p_size;
    byte _v_length;

    short _vs[MAX_PACKET_SIZE];
    byte _vs_idx;

    short _vr[MAX_PACKET_SIZE];
    byte _vr_val[2];
    byte _vr_idx;
    byte _cr_idx;
    bool _escape;
    bool _collecting;
};

#endif
