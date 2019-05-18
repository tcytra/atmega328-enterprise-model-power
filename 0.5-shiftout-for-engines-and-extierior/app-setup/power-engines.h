/**
  Class EnginePower
  Manage the power to the various engine systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 power-engines.h 2019-05-17
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
  
  EnginePower(int dishPins[3])
  {
    powerAvailable = 1;

    dishPinRed    = dishPins[0];
    dishPinGreen  = dishPins[1];
    dishPinBlue   = dishPins[2];

    pinMode(dishPinRed,   OUTPUT);
    pinMode(dishPinGreen, OUTPUT);
    pinMode(dishPinBlue,  OUTPUT);

    analogWrite(dishPinRed,   0);
    analogWrite(dishPinGreen, 0);
    analogWrite(dishPinBlue,  0);

    //engineThrustInner.Power(0);
    //engineThrustOuter.Power(0);
  }

  void disengageEngines()
  {
    Serial.println("Engine Status: All Engines Disengaged");
    analogWrite(dishPinRed,     powerLevel(0));
    analogWrite(dishPinGreen,   powerLevel(0));
    analogWrite(dishPinBlue,    powerLevel(0));
    //engineThrustInner.Power(0);
    //engineThrustOuter.Power(0);
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
    //engineThrustInner.Power(level ? 1 : 0);
    //engineThrustOuter.Power(level ? 1 : 0);
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
