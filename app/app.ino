/**
  Enterprise Power Systems
  Application
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 app.ino 2019-05-17
*/

//  include the libraries
#include <TimerOne.h>

//  include the local classes
#include "app-utility.h"
#include "class-power.h"
#include "class-button.h"
#include "class-flasher.h"
#include "class-flicker.h"
#include "class-system.h"

class LED
{
  public:
  int   pin;
  int   state;
  Power power;
  
  LED(int _pin)
  {
    pin   = pin2Bit(_pin);
    state = 0;
  }
  
  void signal(byte &shift)
  {
    if (power.available) {
      state = state ? 0 : 1;
    } else {
      state = 0;
    }

    bitWrite(shift, pin, state);
  }
};

//  include the power systems
#include "system-engines.h"
#include "system-exterior.h"

EngineSystem    *engine;
ExteriorSystem  *exterior;

//  ------------------------------------------------------------

//  engine pins
byte  dishPins[3]   = {9, 10, 11};
byte  thrustPins[3] = {3, 5, 6};

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

//  exterior pins
byte  markerPin   = 1;
byte  strobePin   = 2;

//  ------------------------------------------------------------

//  declare the shiftout pins
byte  latchPin          = 10; // ST_CP
byte  clockPin          = 13; // SH_CP
byte  dataPin           = 11; // DS

unsigned long appStarted;

void setup() {
  Serial.begin(9600);

  engine    = new EngineSystem(dishPins, thrustPins);    // init with Config object?
  exterior  = new ExteriorSystem(markerPin, strobePin);  // "
  exterior->addButtons(btnEngineFirstPin, btnEngineFinalPin);
  
  //  connect the shiftout pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(btnEngineFirstPin, INPUT);
  pinMode(btnEngineFinalPin, INPUT);
  pinMode(btnExteriorFirstPin, INPUT);
  pinMode(btnExteriorFinalPin, INPUT);
  
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
  Serial.println("Power ready.");
  //Serial.println("Engine Power: "+ engine->power.readStatus());
  //Serial.println("Exterior Power: " + exterior->power.readStatus());
  //Serial.println("  Marker Power: " + exterior->markers->power.readStatus());
  //Serial.println("  Strobe Power: " + exterior->strobes->power.readStatus());

  appStarted = millis();
}


SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();
  
  digitalWrite(latchPin, 0);

  exterior->signal(now);
  
  shiftOut(dataPin, clockPin, LSBFIRST, engine->shiftRead());
  shiftOut(dataPin, clockPin, LSBFIRST, exterior->shiftRead());

  digitalWrite(latchPin, 1);
}

void loop() {

  unsigned long ms = millis();
  
  if (ms - appStarted > 30000) {
    if (ms % 1000 == 0) {
      if (exterior->flood4->power.available) {
        exterior->flood4->power.down();
        Serial.println("Exterior Floods 4: " + exterior->flood4->power.readStatus());
      } else 
      if (exterior->flood3->power.available) {
        exterior->flood3->power.down();
        Serial.println("Exterior Floods 3: " + exterior->flood3->power.readStatus());
      } else 
      if (exterior->flood2->power.available) {
        exterior->flood2->power.down();
        Serial.println("Exterior Floods 2: " + exterior->flood2->power.readStatus());
      } else
      if (exterior->flood1->power.available) {
        exterior->flood1->power.down();
        Serial.println("Exterior Floods 1: " + exterior->flood1->power.readStatus());
      } else
      if (exterior->strobes->power.available) {
        exterior->strobes->power.down();
        Serial.println("Exterior Strobes: " + exterior->strobes->power.readStatus());
      } else
      if (exterior->markers->power.available) {
        exterior->markers->power.down();
        Serial.println("Exterior Markers: " + exterior->markers->power.readStatus());
      } else
      if (exterior->power.available) {
        exterior->power.down();
        Serial.println("Exterior Power: " + exterior->power.readStatus());

        appStarted = millis();
      }
    }
  } else
  
  if (ms - appStarted > 1000) {
    if (ms % 2000 == 0) {
      if (!exterior->power.available) {
        exterior->power.up();
        Serial.println("Exterior Power: " + exterior->power.readStatus());
      } else
      if (!exterior->markers->power.available) {
        exterior->markers->power.up();
        Serial.println("Exterior Markers: " + exterior->markers->power.readStatus());
      } else 
      if (!exterior->strobes->power.available) {
        exterior->strobes->power.up();
        Serial.println("Exterior Strobes: " + exterior->strobes->power.readStatus());
      } else 
      if (!exterior->flood1->power.available) {
        exterior->flood1->power.up();
        Serial.println("Exterior Floods 1: " + exterior->flood1->power.readStatus());
      } else 
      if (!exterior->flood2->power.available) {
        exterior->flood2->power.up();
        Serial.println("Exterior Floods 2: " + exterior->flood2->power.readStatus());
      } else 
      if (!exterior->flood3->power.available) {
        exterior->flood3->power.up();
        Serial.println("Exterior Floods 3: " + exterior->flood3->power.readStatus());
      } else 
      if (!exterior->flood4->power.available) {
        exterior->flood4->power.up();
        Serial.println("Exterior Floods 4: " + exterior->flood4->power.readStatus());
      }
    }
  }
  
  /*
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
  */
  
  delay(1);
}
