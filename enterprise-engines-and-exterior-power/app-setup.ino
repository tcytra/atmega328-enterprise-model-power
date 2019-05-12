/**
 *  Enterprise Power
 *  Application Setup
 */

class Flasher
{
  int on;
  int off;
  int pin;
  int power;
  int state;
  unsigned long then;

  public:
  
  Flasher(int _pin, long _on, long _off)
  {
    pin   = _pin;
    on    = _on;
    off   = _off;
    then  = 0;
    power = 0;
    state = LOW;
    
    pinMode(pin, OUTPUT);

    digitalWrite(pin, LOW);
  }
  
  void Power(int set = -1)
  {
    power = ((set > -1) ? set : ((power > 0) ? 0 : 1));
  }
  
  void Signal(unsigned long now)
  {
    if (power) {
      if ((state == HIGH) && (now -then >= on)) {
        Write(now, LOW);
      } else
      if ((state == LOW) && (now -then >= off)) {
        Write(now, HIGH);
      }
    } else
    if (state == HIGH) {
      Write(0, LOW);
      then = 0;
    }
  }
  
  void Write(unsigned long _now, int _state)
  {
    then  = _now;
    state = _state;
    digitalWrite(pin, state);
  }
};

class Flicker
{
  int pin;
  int wait;
  int level;
  int power;
  int state;
  unsigned long then;
  
  public:
  
  Flicker(int _pin)
  {
    pin   = _pin;
    wait  = 100;
    then  = 0;
    power = 0;
    state = 0;
    pinMode(pin, OUTPUT);
  }
  
  void Power(int set = -1)
  {
    power = ((set > -1) ? set : ((power > 0) ? 0 : 1));
  }
  
  void Signal(unsigned long now)
  {
    if (power) {
      if (now -then >= wait) {
        then = now;
        state = random(120) +135;
        analogWrite(pin, state);
      }
    } else
    if (state) {
      state = 0;
      analogWrite(pin, state);
    }
  }
};

Flasher exteriorMarkers(13, 1000, 1000);
Flasher exteriorStrobes(12, 64, 1200);

Flicker engineThrustInner(5);
Flicker engineThrustOuter(6);

//  ------------------------------------------------------------

int  dishRGBPins[3]   = {9, 10, 11};
//int  thrusterPins[3]  = {3, 5, 6};

/**
 *  Class   EnginePower
 */
class EnginePower
{
  int dishPinRed;
  int dishPinGreen;
  int dishPinBlue;
  int thrustPinInner;
  int thrustPinLower;
  int thrustPinUpper;

  char allowCycleBack = 0;
  char allowThrustOff = 0;
  char powerAvailable = 1;
  
  byte engineState    = 0;
  byte engineLevel[3] = {0, 0, 0};
  byte rgbImpulse[3]  = {255, 127, 0};
  byte rgbWarp[3]     = {0, 0, 255};

  int powerLevel(byte target, byte level = 0)
  {
    return level ? round( target *(level /100) ) : (target ? target : 0);
  }

  public:
  EnginePower(int powerOn)
  {
    powerAvailable = powerOn;

    dishPinRed    = dishRGBPins[0];
    dishPinGreen  = dishRGBPins[1];
    dishPinBlue   = dishRGBPins[2];

    pinMode(dishPinRed,   OUTPUT);
    pinMode(dishPinGreen, OUTPUT);
    pinMode(dishPinBlue,  OUTPUT);

    analogWrite(dishPinRed,   0);
    analogWrite(dishPinGreen, 0);
    analogWrite(dishPinBlue,  0);

    engineThrustInner.Power(0);
    engineThrustOuter.Power(0);
  }

  void disengageEngines()
  {
    Serial.println("Engine Status: All Engines Disengaged");
    analogWrite(dishPinRed,     powerLevel(0));
    analogWrite(dishPinGreen,   powerLevel(0));
    analogWrite(dishPinBlue,    powerLevel(0));
    engineThrustInner.Power(0);
    engineThrustOuter.Power(0);
    engineState = 0;
  }

  void impulsePower(int level)
  {
    analogWrite(dishPinRed,     powerLevel(rgbImpulse[0], level));
    analogWrite(dishPinGreen,   powerLevel(rgbImpulse[1], level));
    analogWrite(dishPinBlue,    powerLevel(rgbImpulse[2], level));
  }

  void thrusterPower(int level)
  {
    engineThrustInner.Power(level ? 1 : 0);
    engineThrustOuter.Power(level ? 1 : 0);
  }
  
  void warpPower(int level)
  {
    analogWrite(dishPinRed,   powerLevel(rgbWarp[0], level));
    analogWrite(dishPinGreen, powerLevel(rgbWarp[1], level));
    analogWrite(dishPinBlue,  powerLevel(rgbWarp[2], level));
  }

  void cycleEngines()
  {
    byte impulseActive    = bitRead(engineState, 1);
    byte thrustersActive  = bitRead(engineState, 0);
    byte warpActive       = bitRead(engineState, 2);

    switch (engineState) {
      case 1: engineState = allowThrustOff ? 3 : 3; break;
      case 2: engineState = allowThrustOff ? 5 : 0; break;
      case 3: engineState = allowThrustOff ? 2 : 5; break;
      case 4: engineState = 0; break;
      case 5: engineState = allowThrustOff ? 4 : 0; break;
      case 6: engineState = 0; break;
      case 0: engineState = 1; break;
    }

    if (!engineState) {
      disengageEngines();
      
    } else {
      //  manage thruster power
      if (bitRead(engineState,0) == 1) {
        if (thrustersActive != 1) {
          Serial.println("Request: Engage Thrusters");
          thrusterPower(100);
        }
      } else {
        if (thrustersActive == 1) {
          Serial.println("Request: Disengage Thrusters");
          thrusterPower(0);
        }
      }
      
      //  apply impulse power
      if (bitRead(engineState,1) == 1) {
        if (impulseActive != 1) {
          Serial.println("Request: Engage Impulse Engines");
          impulsePower(100);
        }
      }
      
      //  apply warp power
      if (bitRead(engineState,2) == 1) {
        if (warpActive != 1) {
          Serial.println("Request: Engage Warp Engines");
          warpPower(100);
        }
      }
    }
  }
};

EnginePower enginePower(1);

//  ------------------------------------------------------------

int  engineButtonPin    = 2;
byte engineButtonState  = 0;
byte engineButtonPress  = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(engineButtonPin, INPUT);

  enginePower.disengageEngines();
  exteriorMarkers.Power();
  exteriorStrobes.Power();
  //engineThrustInner.Power();
  //engineThrustOuter.Power();

  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
  Serial.println("Engine Power Ready.");
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();
  
  exteriorMarkers.Signal(now);
  exteriorStrobes.Signal(now);
  engineThrustInner.Signal(now);
  engineThrustOuter.Signal(now);
}

void loop() {
  // single click
  // double click
  // long click
  engineButtonState = digitalRead(engineButtonPin);

  if (engineButtonState) {
    if (!engineButtonPress) {
      engineButtonPress = 1;
      enginePower.cycleEngines();
    }
    
  } else {
    engineButtonPress = 0;
  }

  delay(1);
}
