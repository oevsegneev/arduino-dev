//#include <Arduino.h>
#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

#define ledPin 13

#define RtcSquareWavePin 2
#define RtcSquareWaveInterrupt 0

volatile bool interuptFlag = false;
bool ledFlag = false;

void InteruptServiceRoutine(){
  interuptFlag = true;
}

void blink(){
  digitalWrite(ledPin, ledFlag);
  ledFlag = !ledFlag;
}

void setup(){
  Serial.begin(9600);
  pinMode(RtcSquareWavePin, INPUT);
  pinMode(ledPin, OUTPUT);

  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(2019,5,22,15,30,0);

  if (!Rtc.IsDateTimeValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning()){
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }
    
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); 

  RtcDateTime alarmTime = RtcDateTime(2019,5,22,15,38,0);
  DS3231AlarmOne alarm1(
    alarmTime.Day(),
    alarmTime.Hour(),
    alarmTime.Minute(), 
    alarmTime.Second(),
    DS3231AlarmOneControl_HoursMinutesSecondsMatch);
  Rtc.SetAlarmOne(alarm1);

  // throw away any old alarm state before we ran
  Rtc.LatchAlarmsTriggeredFlags();

  // setup external interupt 
  attachInterrupt(RtcSquareWaveInterrupt, InteruptServiceRoutine, FALLING);
}

void loop(){
  if(interuptFlag){
    Serial.println("alarm!");
    interuptFlag = false;
  }
  RtcDateTime now = Rtc.GetDateTime();
  Serial.print(now.Hour());
  Serial.print(":");
  Serial.print(now.Minute());
  Serial.print(":");
  Serial.println(now.Second());

  blink();
  delay(1000);
}
