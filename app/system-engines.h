/**
  Class EngineSystem
  Manage the power to the various engine systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 system-engines.h 2019-05-17
*/
class EngineSystem: public System
{
  byte  state    = 0;
  
  //char  allowCycleBack = 1;//!implemented
  char  allowThrustOff = 1;
  
  public:
  
  Flicker *thrust0;
  Flicker *thrust1;
  Flicker *thrust2;
  LED *dishRed;
  LED *dishGreen;
  LED *dishBlue;
  LED *impulse;
  LED *nacelle;
  Power power;
  
  EngineSystem(byte dishPins[3], byte thrustPins[3], byte impulsePin, byte nacellePin)
  {
    impulse   = new LED(impulsePin, NominalPower);
    nacelle   = new LED(nacellePin, NominalPower);
    
    dishRed   = new LED(dishPins[0], NominalPower);
    dishBlue  = new LED(dishPins[1], NominalPower);
    dishGreen = new LED(dishPins[2], round(NominalPower / 2));
    
    thrust0   = new Flicker(thrustPins[0], 80, NominalPower -48, NominalPower);
    thrust1   = new Flicker(thrustPins[1], 80, ConstantPower -48, ConstantPower);
    thrust2   = new Flicker(thrustPins[2], 80, NominalPower -48, NominalPower);
  }

  void signal(unsigned long now)
  {
    dishRed->timer(now);
    dishGreen->timer(now);
    dishBlue->timer(now);
    thrust0->timer(now);
    thrust1->timer(now);
    thrust2->timer(now);
    impulse->timer(now);
    nacelle->timer(now);
  }
  
  void cycle()
  {
    byte impulseActive    = bitRead(state, 1);
    byte thrustersActive  = bitRead(state, 0);
    byte warpActive       = bitRead(state, 2);

    switch (state) {
      case 1: state = allowThrustOff ? 3 : 3; break;
      case 2: state = allowThrustOff ? 5 : 0; break;
      case 3: state = allowThrustOff ? 2 : 5; break;
      case 4: state = 0; break;
      case 5: state = allowThrustOff ? 4 : 0; break;
      case 6: state = 0; break;
      case 0: state = 1; break;
    }

    if (!state) {
      disengageEngines();
      
    } else {
      //  manage thruster power
      if (bitRead(state, 0) == 1) {
        if (thrustersActive != 1) {
          Serial.println("Request: Engage Thrusters");
          //thrusterPower();
          thrust0->power.on();
          thrust1->power.on();
          thrust2->power.on();
          
        }
      } else {
        if (thrustersActive == 1) {
          Serial.println("Request: Disengage Thrusters");
          //thrusterPower(0);
          thrust0->power.off();
          thrust1->power.off();
          thrust2->power.off();
        }
      }
      
      //  apply impulse power
      if (bitRead(state, 1) == 1) {
        if (impulseActive != 1) {
          Serial.println("Request: Engage Impulse Engines");
          impulse->power.on();
          dishRed->power.on();
          dishGreen->power.on();
          dishBlue->power.off();
          nacelle->power.off();
        }
      }
      
      //  apply warp power
      if (bitRead(state, 2) == 1) {
        if (warpActive != 1) {
          Serial.println("Request: Engage Warp Engines");
          impulse->power.off();
          dishRed->power.off();
          dishGreen->power.off();
          dishBlue->power.on();
          nacelle->power.on();
        }
      }
    }
  }

  void disengageEngines()
  {
    Serial.println("Engine Status: All Engines Disengaged");
    
    dishRed->power.off();
    dishGreen->power.off();
    dishBlue->power.off();
    thrust0->power.off();
    thrust1->power.off();
    thrust2->power.off();
    nacelle->power.off();
    
    state = 0;
  }

  void impulsePower(byte level)
  {
    //analogWrite(dishPinRed,     powerLevel(rgbImpulse[0], level));
    //analogWrite(dishPinGreen,   powerLevel(rgbImpulse[1], level));
    //analogWrite(dishPinBlue,    powerLevel(rgbImpulse[2], level));
  }

  void thrusterPower(byte level)
  {
    //engineThrustInner.Power(level ? 1 : 0);
    //engineThrustOuter.Power(level ? 1 : 0);
  }
  
  void warpPower(byte level)
  {
    //analogWrite(dishPinRed,   powerLevel(rgbWarp[0], level));
    //analogWrite(dishPinGreen, powerLevel(rgbWarp[1], level));
    //analogWrite(dishPinBlue,  powerLevel(rgbWarp[2], level));
  }
};
