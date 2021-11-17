#include <Arduino.h>
#include <Servo.h>

#define PIEZO 5
#define SERVO_1 A1
#define SERVO_2 A2

class Led
{
private:
  int pin;

public:
  Led(int _pin)
  {
    pin = _pin;
  }
  int getPin()
  {
    return pin;
  }
  void setOn()
  {
    digitalWrite(pin, HIGH);
  }
  void setOff()
  {
    digitalWrite(pin, LOW);
  }
   
};
class Senzor
{
private:
  int trig;
  int echo;

public:
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

Servo myServo1;
Servo myServo2;

Senzor senzor1(7, 6);
Senzor senzor2(3, 4);

Led ledsOk[] = {Led(8), Led(9)};
Led leds1[] = {Led(10), Led(11)};
Led leds2[] = {Led(12), Led(13)};

const unsigned long eventInterval = 1000;
unsigned long previousTime = 0;
const unsigned long eventInterval2 = 2000;
unsigned long previousTime2 = 0;

bool vlak = false;
bool gate = true;

void barrierGate(Led* led1, Led* led2);

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 2; i++)
  {
    pinMode(leds1[i].getPin(), OUTPUT);
    pinMode(leds2[i].getPin(), OUTPUT);
    pinMode(ledsOk[i].getPin(), OUTPUT);
  }

  pinMode(PIEZO, OUTPUT);

  myServo1.attach(SERVO_1);
  myServo2.attach(SERVO_2);
  myServo1.write(0);
  myServo2.write(180);

  pinMode(senzor1.getEcho(), INPUT);
  pinMode(senzor1.getTrig(), OUTPUT);

  pinMode(senzor2.getEcho(), INPUT);
  pinMode(senzor2.getTrig(), OUTPUT);
}

void loop()
{
  unsigned long currentTime = millis();
  unsigned long currentTime2 = millis();
  //Serial.println(senzor1.geDistance()); //funkcni
  //Serial.println(senzor2.geDistance()); //funkcni
  if (Serial.available() > 0) //funkci
  {
    String comdata = "";
    comdata = Serial.readString();
    if (String("Vlak") == comdata)
    {
      vlak = true;
      gate = true;
      //tone(PIEZO, 131); //funguje, az moc dobre
      
    }
    else if (String("Volno") == comdata)
    {
      vlak = false;
      // for (int i = 0; i < 2; i++)
      // {
      //   leds1[i].setOff();
      //   leds2[i].setOff();
      //   ledsOk[i].setOn();
      // }
      // myServo1.write(90);
      // myServo2.write(90);
    }
  }
  if(vlak) {
    ledsOk[0].setOff();
    ledsOk[1].setOff();
    if(currentTime2 - previousTime2 >= 2000) { //funkcni
      barrierGate(leds1, leds2);
    }
    if(gate && currentTime - previousTime >= 10000) {
      //u klasickych prejezdu zavisi na rychlosti vlaku a smeru odkud jede, zavory se tedy v tomto projektu budou sklapet po 10s od spusteni signalizacniho znaceni
      myServo1.write(90);
      myServo2.write(90);
      gate = false;
    }
  }
}
void barrierGate(Led* led1, Led* led2) {
  led1[0].setOn();
  led2[0].setOn();
  led1[1].setOff();
  led2[1].setOff();
  delay(1000);
  led1[0].setOff();
  led2[0].setOff();
  led1[1].setOn();
  led2[1].setOn();
  delay(1000);
}
