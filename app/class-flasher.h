/**
  Class Flasher
  Manage a flashing light effect
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 class-flasher.h 2019-05-17
  @todo     This needs to be implemented as a trait (of LED?)
*/
class Flasher
{
  unsigned int  delayOn;
  unsigned int  delayOff;
  unsigned long then;

  public:
  
  byte  pin;
  Power power;
  
  Flasher(int _pin, int _on, int _off)
  {
    pin = _pin;
    delayOn   = _on;
    delayOff  = _off;
  }

  void timer(unsigned long now)
  {
    if (power.active) {
      if ((power.state) && (now -then >= delayOn)) {
        power.state = 0;
        then  = now;
      } else
      if ((power.state == 0) && (now -then >= delayOff)) {
        power.state = ConstantPower;
        then  = now;
      }
    } else if (power.state) {
      power.state = 0;
      then  = 0;
    }

    ShiftPWM.SetOne(pin, power.state);
  }
};
