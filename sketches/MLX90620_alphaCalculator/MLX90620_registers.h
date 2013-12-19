/*
  Register values from Datasheet_90620.pdf dated 19 September 2012
*/

//Yes really, it's not 0x60, the EEPROM has its own address (I've never seen this before)
#define MLX90620_EEPROM_WRITE 0xA0
#define MLX90620_EEPROM_READ  0xA1

//The sensor's I2C address is 0x60. So 0b.1100.000W becomes 0xC0
#define MLX90620_WRITE 0xC0
#define MLX90620_READ  0xC1

//These are commands
#define CMD_READ_REGISTER 0x02

//Begin registers

#define CAL_ACP 0xD4
#define CAL_BCP 0xD5
#define CAL_alphaCP_L 0xD6
#define CAL_alphaCP_H 0xD7
#define CAL_TGC 0xD8
#define CAL_BI_SCALE 0xD9

#define VTH_L 0xDA
#define VTH_H 0xDB
#define KT1_L 0xDC
#define KT1_H 0xDD
#define KT2_L 0xDE
#define KT2_H 0xDF

//Common sensitivity coefficients
#define CAL_A0_L 0xE0
#define CAL_A0_H 0xE1
#define CAL_A0_SCALE 0xE2
#define CAL_DELTA_A_SCALE 0xE3
#define CAL_EMIS_L 0xE4
#define CAL_EMIS_H 0xE5

//KSTA

//Config register = 0xF5-F6

#define OSC_TRIM_VALUE 0xF7

//Bits within configuration register 0x92

#define POR_TEST 10

