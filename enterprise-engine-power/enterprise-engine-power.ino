/**
 *  Enterprise Power
 *  Engine Power Object
 */

int  dishRGBPins[3]   = {9, 10, 11};
//int  domeRGBPins[3] = {};
int  thrusterPins[3]  = {3, 5, 6};

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
  char powerAvailable = 0;
  
  byte engineState    = 0;
  byte engineLevel[3] = {0, 0, 0};
  byte rgbImpulse[3]  = {255, 127, 0};
  byte rgbThrust[3]   = {0, 0, 0};
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

    analogWrite(dishPinRed,   255 -powerLevel(0));
    analogWrite(dishPinGreen, 255 -powerLevel(0));
    analogWrite(dishPinBlue,  255 -powerLevel(0));

    thrustPinInner  = thrusterPins[0];
    thrustPinLower  = thrusterPins[1];
    thrustPinUpper  = thrusterPins[2];
    
    pinMode(thrustPinInner, OUTPUT);
    pinMode(thrustPinLower, OUTPUT);
    pinMode(thrustPinUpper, OUTPUT);
    
    //analogWrite(thrustPinInner, powerLevel(0));
    //analogWrite(thrustPinLower, powerLevel(0));
    //analogWrite(thrustPinUpper, powerLevel(0));
  }

  void disengageEngines()
  {
    Serial.println("Engine Status: All Engines Disengaged");
    analogWrite(dishPinRed,   255 -powerLevel(0));
    analogWrite(dishPinGreen, 255 -powerLevel(0));
    analogWrite(dishPinBlue,  255 -powerLevel(0));
    analogWrite(thrustPinInner, powerLevel(0));
    analogWrite(thrustPinLower, powerLevel(0));
    analogWrite(thrustPinUpper, powerLevel(0));
    engineState = 0;
  }

  void impulsePower(int level)
  {
    analogWrite(dishPinRed,   255 -powerLevel(rgbImpulse[0], level));
    analogWrite(dishPinGreen, 255 -powerLevel(rgbImpulse[1], level));
    analogWrite(dishPinBlue,  255 -powerLevel(rgbImpulse[2], level));
  }

  void thrusterPower(int level)
  {
    analogWrite(thrustPinInner, powerLevel(200));
    analogWrite(thrustPinLower, powerLevel(200));
    analogWrite(thrustPinUpper, powerLevel(200));
  }
  
  void warpPower(int level)
  {
    analogWrite(dishPinRed,   255 -powerLevel(rgbWarp[0], level));
    analogWrite(dishPinGreen, 255 -powerLevel(rgbWarp[1], level));
    analogWrite(dishPinBlue,  255 -powerLevel(rgbWarp[2], level));
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

    /*Serial.println("");
    Serial.print("EngineState: ");
    Serial.print(impulseActive);
    Serial.print(thrustersActive);
    Serial.print(warpActive);
    Serial.print(" -> ");
    Serial.print(bitRead(engineState,0));
    Serial.print(bitRead(engineState,1));
    Serial.println(bitRead(engineState,2));*/
    
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
  
  Serial.println("Engine Power Ready.");
}

void loop() {
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
