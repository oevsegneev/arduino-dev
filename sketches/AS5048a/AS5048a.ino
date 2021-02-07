#include <AS5048A.h>


AS5048A angleSensor(8);

void setup()
{
  Serial.begin(19200);
  angleSensor.init();
}

void loop()
{
  delay(1000);
  if (Serial.available()) {
    word val = angleSensor.getRawRotation();
    val = map(val, 0, 16383, 0, (200 * 16 - 1));
    Serial.print("Got rotation of: ");
    Serial.println(val, DEC);
    Serial.print("State: ");
    angleSensor.printState();
    Serial.print("Errors: ");
    Serial.println(angleSensor.getErrors());
  }
}

