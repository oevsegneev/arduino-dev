#include <SD.h>
#define SD_ChipSelectPin 10
#include <TMRpcm.h>
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

int ledPin = 8;

void blink(int count){
  for(int i=0; i<count; i++){
    digitalWrite( ledPin, HIGH );
    delay(200);
    digitalWrite( ledPin, LOW );
    delay(200);
  }
}

void setup(){
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    //Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  pinMode(ledPin, OUTPUT);
  blink(5);
}

void loop(){  
  if(!tmrpcm.isPlaying()){    
    tmrpcm.play("kedr8.wav");
    blink(3);
  }
}
