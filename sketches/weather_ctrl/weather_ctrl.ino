/*
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 */
#include <SPI.h> 
#include <SD.h>
#include <BMP085.h>
#include <Wire.h>

#include "DHT.h"


DHT dht(2, DHT22);
BMP085 bmp = BMP085(); 

const int temp_to = 1000;
const int save_to = 500;
long temp_t = 0;
long save_t = 0;

long tt = 0;

File logFile;

int buzzPin = 4;
int tempPin = A3;
int solPin = A2;

int val;
double d_temp = 0.0;
long d_alt = 0;
double d_hum = 0.0;
int d_sol = 0;
double baseline = 0.0;
long file_pos = 0;

void beep( int cnt, int dl ){
  for( int i=0; i<cnt; i++){
    digitalWrite( buzzPin, HIGH );
    delay(dl);
    digitalWrite( buzzPin, LOW );
    delay(dl);
  }
}

void saveData( double d_temp, long d_alt, double d_hum ){
  logFile = SD.open("log.txt", FILE_WRITE);
  
  if( logFile ){
    logFile.seek(file_pos);
    logFile.print( d_temp );
    logFile.print( "\t" );
    logFile.print( d_alt );
    logFile.print( "\t" );
    logFile.print( d_hum );
    //logFile.print( "\t" );
    //logFile.print( d_sol );
    logFile.println();
    file_pos = logFile.position();
    logFile.close();
  } else {
    beep(2, 500);
    Serial.println('SD write error');
  }
}

double getTemp(){
  val = analogRead( tempPin );
  val = (((val / 1023.0) * 5.0) * 100.0);
  val = val - 273.0;
  return val;
}

long getAlt(){
  long A;
  bmp.getAltitude(&A);
  return A;
}

double getHum(){
  return dht.readHumidity();
}

int getSol(){
  return analogRead(solPin);
}

double getPressure() {
  return 0;
}

void setup(){
  pinMode(buzzPin, OUTPUT);
  Serial.begin(9600);
  // init SD
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    beep(3, 500);
    Serial.println('SD init error');
    return;  
  }
  Wire.begin();
  dht.begin();
  
  bmp.init(MODE_ULTRA_HIGHRES, 25000, true);
  
  beep(3, 100);
}

void loop(){
  tt = millis();  
  if( tt > temp_t + temp_to ){
    temp_t = tt;
    d_temp = getTemp();
    d_alt = getAlt();
    d_hum = getHum();
    d_sol = getSol();
  }

  tt = millis();  
  if( tt > save_t + save_to ){
    save_t = tt;
    saveData( d_temp, d_alt, d_hum );
  }
}
