/**
 *  Enterprise Power
 *  Engine Power Object
 */

int  pinEngineButton    = 2;
byte engineButtonState  = 0;
byte engineButtonPress  = 0;

/**
 *  Class   EnginePower
 */
class EnginePower
{
  int pinRed;
  int pinGreen;
  int pinBlue;

  char allowThrustOff = 0;
  char allowCycleBack = 0;
  char isAvailable    = 1;
  
  byte engineState    = 0;
  byte engineLevel[3] = {0, 0, 0};
  byte rgbImpulse[3]  = {255, 127, 0};
  byte rgbThrust[3]   = {0, 0, 0};
  byte rgbWarp[3]     = {0, 0, 255};

  int powerLevel(byte target, byte level)
  {
    return level ? (255 -round( target *(level /100) )) : 0;
  }

  public:
  EnginePower(int pin_red, int pin_green, int pin_blue)
  {
    pinRed    = pin_red;
    pinGreen  = pin_green;
    pinBlue   = pin_blue;
    
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinBlue, OUTPUT);

    analogWrite(pinRed,   255-0);
    analogWrite(pinGreen, 255-0);
    analogWrite(pinBlue,  255-0);
  }

  void disengageEngines()
  {
    Serial.println("Engine Status: All Engines Disengaged");
    analogWrite(pinRed,   255-0);
    analogWrite(pinGreen, 255-0);
    analogWrite(pinBlue,  255-0);
    engineState = 0;
  }

  void impulsePower(int level)
  {
    analogWrite(pinRed,   powerLevel(rgbImpulse[0], level));
    analogWrite(pinGreen, powerLevel(rgbImpulse[1], level));
    analogWrite(pinBlue,  powerLevel(rgbImpulse[2], level));
  }

  void thrusterPower(int level)
  {  }
  
  void warpPower(int level)
  {
    analogWrite(pinRed,   255 -(level ? rgbWarp[0] : 0));
    analogWrite(pinGreen, 255 -(level ? rgbWarp[1] : 0));
    analogWrite(pinBlue,  255 -(level ? rgbWarp[2] : 0));
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

    Serial.println("");
    Serial.print("EngineState: ");
    Serial.print(impulseActive);
    Serial.print(thrustersActive);
    Serial.print(warpActive);
    Serial.print(" -> ");
    Serial.print(bitRead(engineState,0));
    Serial.print(bitRead(engineState,1));
    Serial.println(bitRead(engineState,2));
    
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

EnginePower enginePower(9, 10, 11);

void setup() {
  Serial.begin(9600);
  
  pinMode(pinEngineButton, INPUT);
  
  Serial.println("Engine Power Ready.");
}

void loop() {
  engineButtonState = digitalRead(pinEngineButton);

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
