// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#define PIN_A  3
#define PIN_B  7
#define PIN_C  11
#define PIN_D  9
#define PIN_E  8
#define PIN_F  4
#define PIN_G  12
#define DP    10

#define D1    2
#define D2    5
#define D3    6
#define D4    13

void setup()
{
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  pinMode(PIN_E, OUTPUT);
  pinMode(PIN_F, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(DP, OUTPUT);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  Serial.begin(9600);
  Serial.setTimeout(10);
}

class DigitTurnOnMethods
{
private:
    void flushDigit()
    {
      digitalWrite(PIN_A, LOW);
      digitalWrite(PIN_B, LOW);
      digitalWrite(PIN_C, LOW);
      digitalWrite(PIN_D, LOW);
      digitalWrite(PIN_E, LOW);
      digitalWrite(PIN_F, LOW);
      digitalWrite(PIN_G, LOW);
      digitalWrite(DP, LOW);
    }

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

public:
    typedef void (DigitTurnOnMethods::*method)();

    method methods[4] =
    {
      &DigitTurnOnMethods::D1ON,
      &DigitTurnOnMethods::D2ON,
      &DigitTurnOnMethods::D3ON,
      &DigitTurnOnMethods::D4ON,
    };
};

class DigitDisplayMethods
{
private:
    void display0()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_E, HIGH);
      digitalWrite(PIN_F, HIGH);
    }

    void display1()
    {
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
    }

    void display2()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_E, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

    void display3()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

    void display4()
    {
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_F, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

    void display5()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_F, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

    void display6()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_E, HIGH);
      digitalWrite(PIN_F, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

    void display7()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
    }

    void display8()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_E, HIGH);
      digitalWrite(PIN_F, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

    void display9()
    {
      digitalWrite(PIN_A, HIGH);
      digitalWrite(PIN_B, HIGH);
      digitalWrite(PIN_C, HIGH);
      digitalWrite(PIN_D, HIGH);
      digitalWrite(PIN_F, HIGH);
      digitalWrite(PIN_G, HIGH);
    }

public:
    void displayDP()
    {
      digitalWrite(DP, HIGH);
    }

    typedef void (DigitDisplayMethods::*method)();

    method methods[10] =
    {
      &DigitDisplayMethods::display0,
      &DigitDisplayMethods::display1,
      &DigitDisplayMethods::display2,
      &DigitDisplayMethods::display3,
      &DigitDisplayMethods::display4,
      &DigitDisplayMethods::display5,
      &DigitDisplayMethods::display6,
      &DigitDisplayMethods::display7,
      &DigitDisplayMethods::display8,
      &DigitDisplayMethods::display9,
    };
};

/* Max allowed RAM is 99.999 MB */
#define PACKET_SIZE 5

uint8_t packet_current_digit = 0;
/* Initialized to PACKET_SIZE + 1 so it won't try do display anything until a valid packet is received */
uint8_t packet_size = PACKET_SIZE + 1;
boolean packet_continue_reading = false;
char packet[PACKET_SIZE] = {0};
char buff_packet[PACKET_SIZE] = {0};
uint32_t ms_since_last_packet_received = 0;

DigitDisplayMethods digitDisplayMeth;
DigitTurnOnMethods digitTurnOnMeth;

static void try_read_packet()
{
  if (Serial.available())
  {
    char c = (char)Serial.read();

    if (packet_continue_reading)
    {
      /* Check if current digit exceeds the buff size */
      if (packet_current_digit == PACKET_SIZE)
      {
        packet_continue_reading = false;
        packet_current_digit = 0;
      }

      if (c == 'E') // 'E'nd of packet
      {
        for (int i = 0; i < packet_current_digit; ++i) // Swap buffers
        {
          packet[i] = buff_packet[i];
        }

        ms_since_last_packet_received = 0;
        packet_size = packet_current_digit;
        packet_continue_reading = false;
        packet_current_digit = 0;
      }
      else
      {
        buff_packet[packet_current_digit++] = c;
      }
    }
    else if (c == 'S') // 'S'tart of packet
    {
      packet_continue_reading = true;
    }
  }
}

static void display_ram()
{
  /* If the RAM  <= 9.999 MB */
  if (packet_size < 5)
  {
    for (unsigned int i = 0; i < packet_size; ++i)
    {
      DigitTurnOnMethods::method digit = digitTurnOnMeth.DigitTurnOnMethods::methods[4 - packet_size + i];
      (digitTurnOnMeth.*digit)();

      DigitDisplayMethods::method digitDisplay = digitDisplayMeth.DigitDisplayMethods::methods[(int)packet[i] - 48];
      (digitDisplayMeth.*digitDisplay) ();
    }
  }
  /* If the RAM > 9.999 but <= 99.999 */
  else if (packet_size == 5)
  {
    /* Omit the last digit */
    for (unsigned int i = 0; i < 4; ++i)
    {
      DigitTurnOnMethods::method digit = digitTurnOnMeth.DigitTurnOnMethods::methods[i];
      (digitTurnOnMeth.*digit)();

      DigitDisplayMethods::method digitDisplay = digitDisplayMeth.DigitDisplayMethods::methods[(int)packet[i] - 48];
      /* if the second digit is to be displayed, also activate the DP
         the used RAM is now displayed as GB instead of MB  "10.00" GB */
      if (i == 1)
      {
        digitDisplayMeth.displayDP();
      }
      
      (digitDisplayMeth.*digitDisplay)();
    }
  }
}

bool idle_animation_enabled = true;
#define IDLE_TIMEOUT_MS 3000

static bool is_idling()
{
  const uint32_t ellapsed_millis = millis();
  static uint32_t last_millis = 0;

  /* wrapped */
  if (last_millis > ellapsed_millis)
  {
    ms_since_last_packet_received += (__UINT32_MAX__ - last_millis) + ellapsed_millis;
  }
  else
  {
    ms_since_last_packet_received += ellapsed_millis - last_millis;
  }

  last_millis = ellapsed_millis;

  if (ms_since_last_packet_received >= IDLE_TIMEOUT_MS)
  {
    return true;
  }

  return false;
}

static void display_idle_animation()
{

}

void loop()
{
  try_read_packet();

  if (idle_animation_enabled && is_idling())
  {
    display_idle_animation();
    return;
  }

  display_ram();
}
