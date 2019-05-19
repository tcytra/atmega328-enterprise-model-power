/**
  Enterprise Power
  Application Setup

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.1 app-setup 2019-05-17
*/

//  include the libraries
#include <TimerOne.h>

//  include the local classes
#include "app-utility.h"    //  utilities used by the classes
#include "class-power.h"    //  Power is a dependancy object
#include "class-button.h"   //  Button is a dependancy object
#include "class-flasher.h"
#include "class-flicker.h"

//  include the power systems
#include "power-engines.h"
#include "power-exterior.h"

EnginePower   *engine;
ExteriorPower *exterior;

//  ------------------------------------------------------------

//  engine pins
int   dishPins[3]   = {9, 10, 11};
int   thrustPins[3] = {3, 5, 6};

//  engine buttons
int   btnEngineFirstPin   = 2;
byte  btnEngineFirstState = 0;
byte  btnEngineFirstPress = 0;

int   btnEngineFinalPin   = 4;
byte  btnEngineFinalState = 0;
byte  btnEngineFinalPress = 0;

int   btnExteriorFirstPin   = 7;
byte  btnExteriorFirstState = 0;
byte  btnExteriorFirstPress = 0;

int   btnExteriorFinalPin   = 8;
byte  btnExteriorFinalState = 0;
byte  btnExteriorFinalPress = 0;

//  exterior pins
int   markerPin   = 1;
int   strobePin   = 2;

//  ------------------------------------------------------------

//  declare the shiftout pins
int   latchPin          = 10; // ST_CP
int   clockPin          = 13; // SH_CP
int   dataPin           = 11; // DS

void setup() {
  Serial.begin(9600);

  engine    = new EnginePower(dishPins, thrustPins);
  exterior  = new ExteriorPower(markerPin, strobePin);

  //  connect the shiftout pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(btnEngineFirstPin, INPUT);
  pinMode(btnEngineFinalPin, INPUT);
  pinMode(btnExteriorFirstPin, INPUT);
  pinMode(btnExteriorFinalPin, INPUT);

  //exterior.power.up();
  //exterior.strobes->power.up();

  //OCR0A = 0xAF;
  //TIMSK0 |= _BV(OCIE0A);
  
  Serial.println("Power ready.");
  Serial.println("Engine Power: "+ engine->power.readStatus());
  Serial.println("Exterior Power: " + exterior->power.readStatus());
  //Serial.println("  Marker Power: " + exterior->markers->power.readStatus());
  //Serial.println("  Strobe Power: " + exterior->strobes->power.readStatus());
}

/*
SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();

  //exterior.markers.Signal(now);
  //exterior.strobes.Signal(now);
  //engine.thrustInner.Signal(now);
  //engine.thrustLower.Signal(now);
  
  digitalWrite(latchPin, 0);

  shiftOut(dataPin, clockPin, LSBFIRST, shiftEngines);
  shiftOut(dataPin, clockPin, LSBFIRST, shiftExterior);

  digitalWrite(latchPin, 1);
}
*/

void loop() {
  
  // single click
  // double click
  // long click
  
  btnEngineFirstState = digitalRead(btnEngineFirstPin);
  if (btnEngineFirstState) {
    if (!btnEngineFirstPress) {
      btnEngineFirstPress = 1;
      //enginePower.cycleEngines();
    }
  } else {
    btnEngineFirstPress = 0;
  }

  btnExteriorFirstState = digitalRead(btnExteriorFirstPin);
  if (btnExteriorFirstState) {
    if (!btnExteriorFirstPress) {
      btnExteriorFirstPress = 1;
      //exterior.power.toggle();
    }
  } else {
    btnExteriorFirstPress = 0;
  }

  delay(1);
}
