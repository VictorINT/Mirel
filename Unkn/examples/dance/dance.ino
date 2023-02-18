#include <unkn.h>
#include <Arduino.h>

void setup()
{
  init();
}

void loop()
{
  drive(-MAX, MAX);
  delay(2000);
  drive(MAX, MAX);
  delay(2000);
  drive(0, 0);
  delay(2000);
}
