/*
 2-16-2013
 Spark Fun Electronics
 Nathan Seidle
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This example shows how to read and calculate the 64 alpha readings from the MLX90620 EEPROM
 
 Please also pay attention to your emissivity value: since in my case it was equal to 1, to save SRAM i 
 cut out that piece of calculation. You need to restore those lines if your emissivity value is not equal to 1. 
 
 Don't get confused by the bottom view of the device! The GND pin is connected to the housing.
 
 To get this code to work, attached a MLX90620 to an Arduino Uno using the following pins:
 A5 to 330 ohm to SCL
 A4 to 330 ohm to SDA
 3.3V to VDD
 GND to VSS
 
 Open a terminal window. It should show you something like this:
 
float alpha_ij[64] = {
  0.19091E-8, 0.57508E-8, 0.43538E-8, 0.00464E-8, 0.61000E-8, 0.97089E-8, 0.83119E-8, 0.49359E-8, 
  0.97089E-8, 1.29685E-8, 1.40163E-8, 0.78463E-8, 1.41327E-8, 1.77416E-8, 1.77416E-8, 1.06402E-8, 
  1.72759E-8, 2.10012E-8, 2.05355E-8, 1.49476E-8, 1.86729E-8, 2.40280E-8, 2.34459E-8, 1.78580E-8, 

 Next, copy and paste this into the MLX90620 example sketch, then run the example sketch. 
 
 I used the internal pull-ups on the SDA/SCL lines. Normally you should use ~4.7k pull-ups for I2C.
 
 */

#include <i2cmaster.h>
//i2cmaster comes from here: http://www.cheap-thermocam.bplaced.net/software/I2Cmaster.rar

#include "MLX90620_registers.h"

//Global variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
float alpha_ij[64];

byte eepromData[256];
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

void setup()
{
  Serial.begin(115200);
  Serial.println("MLX90620 alpha calculator");

  i2c_init(); //Init the I2C pins
  PORTC = (1 << PORTC4) | (1 << PORTC5); //Enable pull-ups

  delay(5); //Init procedure calls for a 5ms delay after power-on

  read_EEPROM_MLX90620(); //Read the entire EEPROM
  
  //seedTestValues(); //Do not use this function! Used only for testing equations to make sure the code is right
  
  printRawAlphas(); //Print raw alphas

  calculateAlphas(); //Calculate all the Alphas
  
  printAlphas(); //Print them in a pretty way so the user can easily copy/paste into the example sketch
}

void loop()
{
  //Blink forever indicating we're down with calculation
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}


//Read the 256 bytes from the MLX EEPROM and setup the various constants (*lots* of math)
//Note: The EEPROM on the MLX has a different I2C address from the MLX. I've never seen this before.
void read_EEPROM_MLX90620()
{
  i2c_start_wait(MLX90620_EEPROM_WRITE);
  i2c_write(0x00); //EEPROM info starts at location 0x00
  i2c_rep_start(MLX90620_EEPROM_READ);

  //Read all 256 bytes from the sensor's EEPROM
  for(int i = 0 ; i <= 255 ; i++)
    eepromData[i] = i2c_readAck();

  i2c_stop(); //We're done talking
}


//Calculate alphas using equation 7.3.3.2
//This equation doesn't seem to agree with the example
//The example calculation includes an extra: -TGC/32(256*aCP_H + aCP_L)
void calculateAlphas(void)
{
  //alpha(i,j) = ((a - d) / b) + (da(i) / c)
  
  //a = 256*alpha0_h + alpha0_l
  unsigned int a = 256 * eepromData[CAL_A0_H] + eepromData[CAL_A0_L];

  //d = TGC / 32 * (256.alphaCP_H + alphaCP_L)
  float d = (float)eepromData[CAL_TGC] / 32 * (256 * eepromData[CAL_alphaCP_H] + eepromData[CAL_alphaCP_L]);
  
  //b = 2 ^ alpha_scale
  long long b = pow(2, eepromData[CAL_A0_SCALE]);

  //c = 2 ^ delta_alpha_scale
  long long c = pow(2, eepromData[CAL_DELTA_A_SCALE]);

  for(int i = 0 ; i < 64 ; i++)
  {
    //alpha_ij[i] = ((float)(a - d) / b) + ((float)eepromData[0x80 + i] / c); //This is the equation from the example
    alpha_ij[i] = ((float)a / b) + ((float)eepromData[0x80 + i] / c); //This is the equation from 7.3.3.2
  }
}

//Prints the alpha array in an easy to copy/paste form in the terminal window
void printAlphas()
{
  Serial.println("Copy and paste the following block of text into the MLX90620_Example sketch:");
  Serial.println();

  Serial.print("float alpha_ij[64] = {");

  for(int i = 0 ; i < 64 ; i++)
  {
    if(i % 8 == 0)
    {
      Serial.println();
      Serial.print("  ");
    }
    
    Serial.print(alpha_ij[i] * pow(10, 8), 5); //Print 5 digits past the decimal
    Serial.print("E-8, ");
  }

  Serial.println();
  Serial.print("};");
}

void printRawAlphas()
{
  //Calculate a bunch of constants from the EEPROM data
  unsigned int alpha0 = 256 * eepromData[CAL_A0_H] + eepromData[CAL_A0_L];
  byte alpha0_scale = eepromData[CAL_A0_SCALE];
  byte delta_alpha_scale = eepromData[CAL_DELTA_A_SCALE];
  float emissitivity = (float)((unsigned int)256 * eepromData[CAL_EMIS_H] + eepromData[CAL_EMIS_L]) / 32768.0;

  Serial.print("alpha0: ");
  Serial.println(alpha0);

  Serial.print("alpha0_scale: ");
  Serial.println(alpha0_scale);

  Serial.print("delta_alpha_scale: ");
  Serial.println(delta_alpha_scale);

  Serial.print("emissitivity: ");
  Serial.println(emissitivity, 5);
  
}

//Let's load the example values from various examples in the datasheet to make sure our calculations are correct
void seedTestValues()
{
  //Example 7.3.4
  eepromData[0x22] = 0xD6;
  eepromData[0x62] = 0xC1;
  eepromData[0xA2] = 0x8F;
  eepromData[0xD4] = 0xD0;
  eepromData[0xD5] = 0xCA;
  eepromData[0xD6] = 0x00;
  eepromData[0xD7] = 0x00;
  eepromData[0xD8] = 0x23;
  eepromData[0xD9] = 0x08;
  eepromData[0xE0] = 0xE4;
  eepromData[0xE1] = 0xD5;
  eepromData[0xE2] = 0x2A;
  eepromData[0xE3] = 0x21;
  eepromData[0xE4] = 0x99;
  eepromData[0xE5] = 0x79;
  
  //Example 7.3.2
  eepromData[0xDA] = 0x78;
  eepromData[0xDB] = 0x1A;
  eepromData[0xDC] = 0x33;
  eepromData[0xDD] = 0x5B;
  eepromData[0xDE] = 0xCC;
  eepromData[0xDF] = 0xED;
}
