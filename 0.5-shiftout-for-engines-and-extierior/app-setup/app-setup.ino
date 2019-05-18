/**
  Enterprise Power
  Application Setup

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.1 app-setup 2019-05-17
*/

//  various app utilities
#include "app-utility.h"

//  declare the shift registers
byte  shiftEngines      = 0B00000000; // register 0
byte  shiftExterior     = 0B00000000; // register 1

//  include the classes
#include "class-button.h"
#include "class-flasher.h"
#include "class-flicker.h"

//  create the instances
Flasher exteriorMarkers(1, 1000, 1000, 1);
Flasher exteriorStrobes(2, 64, 1200, 1);
Flicker engineThrustInner(1);
Flicker engineThrustLower(2);
Flicker engineThrustUpper(3);

//  include the power sections
#include "power-engines.h"
#include "power-exterior.h"

//  ------------------------------------------------------------

//  engine pin variables
int   dishRGBPins[3]    = {9, 10, 11};
int   thrusterPins[3]   = {3, 5, 6};

//  engine btn variables
int   engineButtonPin   = 2;
byte  engineButtonState = 0;
byte  engineButtonPress = 0;

//EnginePower enginePower(dishRGBPins);

//  ------------------------------------------------------------

//  declare the shiftout pins
int   latchPin          = 10;
int   clockPin          = 13;
int   dataPin           = 11;

void setup() {
  Serial.begin(9600);

  //  connect the shiftout pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(engineButtonPin, INPUT);

  //enginePower.disengageEngines();
  exteriorMarkers.Power();
  exteriorStrobes.Power();
  //engineThrustInner.Power();
  //engineThrustOuter.Power();

  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
  Serial.println("Power ready.");
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();

  exteriorMarkers.Signal(now);
  exteriorStrobes.Signal(now);
  //engineThrustInner.Signal(now);
  //engineThrustOuter.Signal(now);
  
  digitalWrite(latchPin, 0);

  shiftOut(dataPin, clockPin, LSBFIRST, shiftEngines);
  shiftOut(dataPin, clockPin, LSBFIRST, shiftExterior);

  digitalWrite(latchPin, 1);
}

void loop() {
  //Serial.println(shiftExterior, BIN);

  
  
  delay(20);
  
  /*
  // single click
  // double click
  // long click
  engineButtonState = digitalRead(engineButtonPin);

  if (engineButtonState) {
    if (!engineButtonPress) {
      engineButtonPress = 1;
      //enginePower.cycleEngines();
    }
    
  } else {
    engineButtonPress = 0;
  }

  delay(1);
  */
}
