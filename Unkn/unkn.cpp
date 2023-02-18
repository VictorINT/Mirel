#include "unkn.h"

void reset()
{
  analogWrite(M1A, 0);
  analogWrite(M1B, 0);
  analogWrite(M2A, 0);
  analogWrite(M2B, 0);
}

void drive(int M1Speed, int M2Speed)
{
  reset();
  if (M1Speed == 0)
  {
    analogWrite(M1A, 0);
    analogWrite(M1B, 0);
  }
  if (M2Speed == 0)
  {
    analogWrite(M2A, 0);
    analogWrite(M2B, 0);
  }
  if (M1Speed < 0)
  {
    M1Speed = -M1Speed;
    analogWrite(M1A, M1Speed);
  }
  else
  {
    analogWrite(M1B, M1Speed);
  }
  if (M2Speed < 0)
  {
    M2Speed = -M2Speed;
    analogWrite(M2A, M2Speed);
  }
  else
  {

    analogWrite(M2B, M2Speed);
  }
}

void MInit()
{
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
}
