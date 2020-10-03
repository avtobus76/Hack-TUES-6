#include <SoftwareSerial.h>

SoftwareSerial bt(0, 1);

char data = '0';
int state = 0;

#define POWER_ON 2

void turn_on_kolichka();
void turn_off_kolichka();

void setup() {
  bt.begin(9600);
  pinMode(POWER_ON, OUTPUT);
}

void loop() {
  if(bt.available())
    data = bt.read();

  switch(data)
  {
    case 'A':
      if(state == 0)
        turn_on_kolichka();
        
      if (state == 1)
        turn_off_kolichka();

      data = '0';
      break;

    default:
      break;
  }
}

void turn_on_kolichka()
{
  digitalWrite(POWER_ON, HIGH);
  state = 1;
}

void turn_off_kolichka()
{
  digitalWrite(POWER_ON, LOW);
  state = 0;
}
