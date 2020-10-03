#define ho1 9
#define ho2 10
#define lo1 4
#define lo2 3
#define sd1 2
#define sd2 5

void setup() {
  pinMode(ho1, OUTPUT);
  pinMode(ho2, OUTPUT);
  pinMode(lo1, OUTPUT);
  pinMode(lo2, OUTPUT);
  pinMode(sd1, OUTPUT);
  pinMode(sd2, OUTPUT);
  digitalWrite(sd2, LOW);
  digitalWrite(sd1, LOW);
}

void loop() {
  digitalWrite(lo2, HIGH);
  digitalWrite(lo1, LOW);
  digitalWrite(ho2, LOW);  
  
  for(int i = 0; i < 256; i++)
  {
    analogWrite(ho1, i);
    delay(100);
  }

  delay(3000);

  for(int i = 255; i >= 0; i--)
  {
    analogWrite(ho1, i);
    delay(100);
  }

  delay(3000);

  digitalWrite(ho1, LOW);
  digitalWrite(lo2, LOW);
  digitalWrite(lo1, HIGH); 
  
  for(int i = 0; i < 256; i++)
  {
    analogWrite(ho2, i);
    delay(100);
  }

  delay(3000);

  for(int i = 255; i >= 0; i--)
  {
    analogWrite(ho2, i);
    delay(100);
  }

  delay(3000);
}
