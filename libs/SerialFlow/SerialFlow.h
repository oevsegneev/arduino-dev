/*
  SerialFlow.cpp - Библиотека для пакетного обмена по последовательному порту
  Автор Олег Евсегнеев, 11.09.2012
  Последнее обновление 23.04.2026
  Лицензия MIT
*/
#ifndef SerialFlow_h
#define SerialFlow_h

#include "Arduino.h"

// раскомментируй нужное
//#include <FastSerial.h>
//#include <RF24.h>
#include <HardwareSerial.h>

#define MAX_PACKET_SIZE 8
#define CRC8_POLYNOMIAL 0x07
#define ENABLE_CRC

#if defined RF24_H_
#define RF24_CHANNEL 2
#define RF24_REPEAT_COUNT 1
#define RF24_REPEAT_DELAY 15
#define RF24_PAYLOAD 1
#endif

class SerialFlow {
public:
    #if defined FastSerial_h
    SerialFlow( FastSerial *serial );
    #elif defined RF24_H_
    SerialFlow( uint8_t cn, uint8_t csn );
    #else
    SerialFlow( HardwareSerial *serial );
    #endif

    /** Настройка формата пакета
     *
     * @param v_length - длина значения в байтах
     * @param p_size - количество значений
     */
    void setPacketFormat( uint8_t v_length, uint8_t p_size );

    /**  Инициализация порта
     * @param baud_rate - скорость порта    
     * @param address1 - адрес передачтика    
     * @param address2 - адрес приёмника 
     */
    #ifdef RF24_H_
    void begin( uint64_t address1, uint64_t address2, uint8_t channel = RF24_CHANNEL, rf24_datarate_e baud = RF24_250KBPS );
    #else
    void begin( uint32_t baud_rate );
    #endif

    /**  Установка значения в пакет
     *
     * @param value - значение
     */
    void setPacketValue(uint32_t value);
        
    /**  Отправка пакета
     */
    void sendPacket();

    /**  Получение пакета
     */
    bool receivePacket();

    /**  Извлечение значения из пакета
     * @param idx - индекс значения в пакете    
     */
    uint32_t getPacketValue( uint8_t idx );

    /**  Передача байта
     * @param v - отправляемый байт
     */
    void write( uint8_t v );
    
    /**  Передача массива байт
     * @param *v - отправляемый массив
     * @param length - размер массива
     */
    void write( uint8_t *v, uint8_t length );

    /**  Приём байта
     */
    uint8_t read();

    void getHWInfo();

protected:
    #ifdef FastSerial_h
    FastSerial *_serial;
    #elif defined RF24_H_
    RF24 *_serial;
    #else
    HardwareSerial *_serial;
    #endif

    uint8_t _p_size;
    uint8_t _v_length;

    uint32_t _vs[MAX_PACKET_SIZE];
    uint8_t _vs_idx;

    uint32_t _vr[MAX_PACKET_SIZE];
    uint8_t _vr_val[4];
    uint8_t _vr_idx;
    uint8_t _cr_idx;
    uint8_t _vr_ready;
    bool _escape;
    bool _collecting;

    #ifdef ENABLE_CRC
    uint8_t _payload_crc[MAX_PACKET_SIZE*4*2];
    uint8_t _payload_idx = 0;
    uint8_t _crc;
    uint8_t _crc_ready = 0;
    #endif

    static uint32_t _join_bytes(uint8_t *bs, uint8_t length);
    static uint8_t _crc8(uint8_t *data, uint8_t length);
};

#endif
