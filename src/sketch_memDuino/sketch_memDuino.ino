// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#define pinA  3
#define pinB  7
#define pinC  11
#define pinD  9
#define pinE  8
#define pinF  4
#define pinG  12
#define DP    10

#define D1    2
#define D2    5
#define D3    6
#define D4    13 

void setup()
{
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);
  pinMode(DP, OUTPUT); 
  
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);
  
  Serial.begin(9600);
}

int charToInt(char c)
{
  return (int)c - 48;
}

class DigitFunc
{
private:
  void flushDigit()
  {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
  }
public:
  void D1ON()
  {
    flushDigit();
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
  }
  
  void D2ON()
  {
    flushDigit();
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
  }
  
  void D3ON()
  {
    flushDigit();
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
  }
  
  void D4ON()
  {
    flushDigit();
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
  }
  
  typedef void (DigitFunc::*func)();
  
  func funcArr[4] = 
  {
    &DigitFunc::D1ON,
    &DigitFunc::D2ON,
    &DigitFunc::D3ON,
    &DigitFunc::D4ON,
  };
};

class DisplayFunc
{
public:
  
  void display0()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
  }
  
  void display1()
  { 
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
  }
  
  void display2()
  { 
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  void display3()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  void display4()
  { 
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  void display5()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  void display6()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  void display7()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
  }
  
  void display8()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  void display9()
  {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
  }
  
  typedef void (DisplayFunc::*func) ();

  func funcArr [10] =
  {
    &DisplayFunc::display0,
    &DisplayFunc::display1, 
    &DisplayFunc::display2, 
    &DisplayFunc::display3, 
    &DisplayFunc::display4, 
    &DisplayFunc::display5, 
    &DisplayFunc::display6,
    &DisplayFunc::display7,
    &DisplayFunc::display8,
    &DisplayFunc::display9,    
  };
  
};

boolean go = false;
int currentDigit = 0;
char packet[5]; // 4 digits + final 'X' to confirm the packet
DisplayFunc displayFunc;
DigitFunc digitFunc;

void loop()
{ 
  if(Serial.available())
  {
    char c = (char)Serial.read();
    if(c != '\0')
    {
      packet[currentDigit++] = c;
    }
    else
    {
      packet[4] == 'X' ? go = true : go = false;
      currentDigit = 0;
    }
  }
  
  if(go)
  {
    for(int i = 0; i < 4; ++i)
    {
      DigitFunc::func digFunc = digitFunc.DigitFunc::funcArr[i];
      (digitFunc.*digFunc)();
      
      DisplayFunc::func disFunc = displayFunc.DisplayFunc::funcArr[charToInt(packet[i])];
      (displayFunc.*disFunc) ();
    }
  }
  else // only display 0s if no valid packet is received
  {
     DisplayFunc::func disFunc = displayFunc.DisplayFunc::funcArr[0];
    
     for(int i = 0; i < 4; ++i)
     {
       DigitFunc::func digFunc = digitFunc.DigitFunc::funcArr[i];
       (digitFunc.*digFunc)();
       
       (displayFunc.*disFunc)();
     }
  }
}

