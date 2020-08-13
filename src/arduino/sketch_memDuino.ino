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
  Serial.setTimeout(10);
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
    digitalWrite(DP, LOW);
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

  void displayDP()
  {
    digitalWrite(DP, HIGH);
  }
  
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

int currentDigit = 0, packetSize = 6; // initialized to 6 that way it won't try to daisplay anything until a valid packet is received
boolean continueReading = false;
char packet[5] = {0}, buffPacket[5] = {0}; // Max 5 digits

DisplayFunc displayFunc;
DigitFunc digitFunc;

void readPacket()
{
  if(Serial.available()) // Check to see if there are any incoming bytes
  { 
    char c = (char)Serial.read();
    
    if(continueReading)
    {
      if(c == 'E') // 'E'nd of packet
      {
        for(int i = 0; i < currentDigit; ++i) // Swap buffers
        {
          packet[i] = buffPacket[i];
        }
        
        packetSize = currentDigit;
        continueReading = false;
        currentDigit = 0;
      }
      else
      {
        buffPacket[currentDigit++] = c;
      }
    }
    else if(c == 'S') // 'S'tart of packet
    {
      continueReading = true;
    }
  }
}

void displayRAM()
{
  if(packetSize < 5) // if the used ram value is <= 9999 MB (4 digits)
  {
    for(int i = 0; i < packetSize; ++i)
    {
      DigitFunc::func digFunc = digitFunc.DigitFunc::funcArr[4 - packetSize + i];
      (digitFunc.*digFunc)();
      
      DisplayFunc::func disFunc = displayFunc.DisplayFunc::funcArr[charToInt(packet[i])];
      (displayFunc.*disFunc) ();
    }
  }
  else if(packetSize == 5) // if the used ram value is bigger than 9999 MB (5 digits)
  {
    for(int i = 0; i < 4; ++i)
    {
      DigitFunc::func digFunc = digitFunc.DigitFunc::funcArr[i];
      (digitFunc.*digFunc)();
      
      DisplayFunc::func disFunc = displayFunc.DisplayFunc::funcArr[charToInt(packet[i])];
      /* if the second digit is to be displayed, also activate the DP
         the used RAM is now displayed as GB instead of MB  "10.00" GB */
      if(i == 1)
      {
        displayFunc.displayDP();
      }
      (displayFunc.*disFunc)();
    }
  }
}

void loop()
{ 
  readPacket();
  
  displayRAM();
}


