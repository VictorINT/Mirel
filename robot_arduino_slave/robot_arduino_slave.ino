#include <SPI.h>
#include <unkn.h>

char buff [100];
volatile byte index;
volatile bool receivedone;  /* use reception complete flag */

int motor_left = 0, motor_right = 0;

void setup (void)
{
  init();
  Serial.begin (9600);
  SPCR |= bit(SPE);         /* Enable SPI */
  pinMode(MISO, OUTPUT);    /* Make MISO pin as OUTPUT */
  index = 0;
  receivedone = false;
  SPI.attachInterrupt();    /* Attach SPI interrupt */
}

void parse()
{
  char mleft[5], mright[5];
  // motor left string
  mleft[0] = buff[0];
  mleft[1] = buff[1];
  mleft[2] = buff[2];
  mleft[3] = '\0'; // terminator
  // motor right string
  mright[0] = buff[4];
  mright[1] = buff[5];
  mright[2] = buff[6];
  mright[3] = '\0'; // terminator
  motor_left = atoi(mleft);
  motor_right = atoi(mright);
}

void loop (void)
{
  if (receivedone)          /* Check and print received buffer if any */
  {
    buff[index] = 0;
    Serial.println(buff);
    index = 0;
    receivedone = false;
    parse();
    drive(motor_left, motor_right);
  }
}

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  uint8_t oldsrg = SREG;
  cli();
  char c = SPDR;
  if (index <sizeof buff)
  {
    buff [index++] = c;
    if (c == '\n'){     /* Check for newline character as end of msg */
     receivedone = true;
    }
  }
  SREG = oldsrg;
}
