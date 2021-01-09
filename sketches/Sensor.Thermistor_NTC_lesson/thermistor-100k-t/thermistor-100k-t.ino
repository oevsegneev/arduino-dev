#define B 3950 // B-коэффициент
#define SERIAL_R 102000 // сопротивление последовательного резистора, 102 кОм
#define THERMISTOR_R 100000 // номинальное сопротивления термистора, 100 кОм
#define NOMINAL_T 25 // номинальная температура (при которой TR = 100 кОм)

const byte tempPin = A0;

void setup() {
  Serial.begin( 9600 );
  pinMode( tempPin, INPUT );
}

void loop() {
  int t = analogRead( tempPin );
  float tr = 1023.0 / t - 1;
  tr = SERIAL_R / tr;
  Serial.print("R=");
  Serial.print(tr);
  Serial.print(", t=");

  float steinhart;
  steinhart = tr / THERMISTOR_R;           // (R/Ro)
  steinhart = log(steinhart);              // ln(R/Ro)
  steinhart /= B;                          // 1/B * ln(R/Ro)
  steinhart += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;             // Invert
  steinhart -= 273.15;  
  Serial.println(steinhart);
  
  delay(100);
}
