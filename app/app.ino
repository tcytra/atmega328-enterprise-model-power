/**
  Enterprise Power Systems
  Application
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 app.ino 2019-05-17
*/

const byte  ConstantPower = 255;
const byte  CriticalPower = 102;
const byte  NominalPower  = 204;

const int   ShiftPWM_latchPin = 10;
const bool  ShiftPWM_invertOutputs = false; 
const bool  ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>

//  include the power object
#include "class-power.h"

//  include the component objects
#include "class-button.h"
#include "class-led.h"

//  include the led effects
#include "class-flasher.h"
#include "class-flicker.h"

//  include the system object
#include "class-system.h"

//  include the power systems
#include "system-engines.h"
#include "system-exterior.h"

EngineSystem    *engine;
ExteriorSystem  *exterior;

//  ------------------------------------------------------------

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 120;

int numRegisters = 2;
int numOutputs = numRegisters*8;

unsigned long appStarted = 0;

//  engine pins
byte  impulsePin    = 8;
byte  nacellePin    = 9;
byte  dishPins[3]   = {13, 14, 15};
byte  thrustPins[3] = {10, 11, 12};

//  exterior pins
byte  markerPin   = 1;
byte  strobePin   = 2;

//  ------------------------------------------------------------

//  engine buttons
byte  btnEngineFirstPin   = 2;
byte  btnEngineFirstState = 0;
byte  btnEngineFirstPress = 0;

byte  btnEngineFinalPin   = 4;
byte  btnEngineFinalState = 0;
byte  btnEngineFinalPress = 0;

byte  btnExteriorFirstPin   = 7;
byte  btnExteriorFirstState = 0;
byte  btnExteriorFirstPress = 0;

byte  btnExteriorFinalPin   = 8;
byte  btnExteriorFinalState = 0;
byte  btnExteriorFinalPress = 0;

void setup() {
  while(!Serial){
    delay(100); 
  }
  Serial.begin(9600);

  ShiftPWM.SetAmountOfRegisters(numRegisters);
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  ShiftPWM.SetAll(0);
  
  pinMode(btnEngineFirstPin, INPUT);
  pinMode(btnEngineFinalPin, INPUT);
  pinMode(btnExteriorFirstPin, INPUT);
  pinMode(btnExteriorFinalPin, INPUT);

  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
  appStarted = millis();

  engine    = new EngineSystem(dishPins, thrustPins, impulsePin, nacellePin);
  exterior  = new ExteriorSystem(markerPin, strobePin);
  
  engine->power.on();
  exterior->power.on();
  
  Serial.println("Power ready.");
}

void loop() {
  unsigned long now = millis();
  
  btnEngineFirstState = digitalRead(btnEngineFirstPin);
  if (btnEngineFirstState) {
    if (!btnEngineFirstPress) {
      btnEngineFirstPress = 1;
      engine->cycle();
    }
  } else {
    btnEngineFirstPress = 0;
  }

  btnEngineFinalState = digitalRead(btnEngineFinalPin);
  if (btnEngineFinalState) {
    if (!btnEngineFinalPress) {
      btnEngineFinalPress = 1;
    }
  } else {
    btnEngineFinalPress = 0;
  }

  btnExteriorFirstState = digitalRead(btnExteriorFirstPin);
  if (btnExteriorFirstState) {
    if (!btnExteriorFirstPress) {
      btnExteriorFirstPress = 1;
      exterior->cycle(1);
    }
  } else {
    btnExteriorFirstPress = 0;
  }

  btnExteriorFinalState = digitalRead(btnExteriorFinalPin);
  if (btnExteriorFinalState) {
    if (!btnExteriorFinalPress) {
      btnExteriorFinalPress = 1;
      exterior->cycle(0);
    }
  } else {
    btnExteriorFinalPress = 0;
  }
  
  delay(1);
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();

  engine->signal(now);
  exterior->signal(now);
}
