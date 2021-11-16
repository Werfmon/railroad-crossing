#include <Arduino.h>
#include <Servo.h>

Servo myServo1;
Servo myServo2;

int leds1[] = {10, 11};
int leds2[] = {12, 13};

#define PIEZO 9
#define SERVO_1 A1
#define SERVO_2 A2

class Senzor
{
public:
  int trig;
  int echo;
  Senzor(int _trig, int _echo)
  {
    trig = _trig;
    echo = _echo;
  }
  int getTrig()
  {
    return trig;
  }
  int getEcho()
  {
    return echo;
  }
  int geDistance()
  {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(5);
    digitalWrite(trig, LOW);
    int odezva = pulseIn(echo, HIGH);
    return (odezva / 2) / 29.1;
  }
};

Senzor senzor1(7, 6);
Senzor senzor2(3, 4);

void setup()
{
  Serial.begin(9600);
  
  for (int i = 0; i < 2; i++)
  {
    pinMode(leds1[i], OUTPUT);
    digitalWrite(leds1[i], LOW);
    pinMode(leds2[i], OUTPUT);
    digitalWrite(leds2[i], LOW);
  }

  pinMode(PIEZO, OUTPUT);

  myServo1.attach(SERVO_1);
  myServo2.attach(SERVO_2);

  myServo1.write(90); //setup
  myServo2.write(90); //setup

  pinMode(senzor1.getEcho(), INPUT);
  pinMode(senzor1.getTrig(), OUTPUT);

  pinMode(senzor2.getEcho(), INPUT);
  pinMode(senzor2.getTrig(), OUTPUT);
}

void loop()
{
  Serial.println(senzor1.geDistance()); //funkcni
  Serial.println(senzor2.geDistance()); //funkcni

  if (Serial.available() > 0) //funkci
  {
    String comdata = "";
    comdata = Serial.readString();
    if (String("1") == comdata)
    {
      Serial.print("fsdfsd");
      digitalWrite(leds1[0], HIGH);
      digitalWrite(leds1[1], HIGH);
    }
    else if (String("2") == comdata)
    {
      digitalWrite(leds2[0], HIGH);
      digitalWrite(leds2[1], HIGH);
    }
    else
    {
      digitalWrite(leds1[0], LOW);
      digitalWrite(leds1[1], LOW);
      digitalWrite(leds2[0], LOW);
      digitalWrite(leds2[1], LOW);
    }
  }
  //tone(PIEZO, 131); //funguje, az moc dobre
  
  delay(5000);
  myServo1.write(0);  
  myServo2.write(180);
}
