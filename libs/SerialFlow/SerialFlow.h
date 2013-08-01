/*
  SerialFlow.h - Communication library with packages of values.
  Created by Oleg Evsegneev, September, 2012.
  Released into the public domain.
  Ver 0.2 (for Arduino)
*/
#ifndef SerialFlow_h
#define SerialFlow_h

#include "Arduino.h"
#include <SoftwareSerial.h>

#define MAX_PACKET_SIZE 10

class SerialFlow {
public:
    /** Data format */
    enum DataFormat {
        SIMPLE     /**< Simple one byte (default) */
        , COMPLEX  /**< Packet of short-type values */
    };
    
    SerialFlow(int tx, int rx);

    /** Set data packet format
     *
     * @param p_format Format type constant.
     * @param v_length Length of value in bytes.
     * @param p_size Number of values.
     */
    void setPacketFormat(DataFormat p_format, byte v_length, byte p_size);

    /**  Initialize port
     * @param baud_rate Serial port baud rate.    
     */
    void begin( int baud_rate );

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

protected:
    SoftwareSerial _serial;
    byte _p_format;
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
