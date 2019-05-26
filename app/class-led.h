/**
  Class LED
  Instantiate within an object to provide LED management

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 class-led.h 2019-05-26
*/
class LED
{
  byte  pin;
  
  public:
  
  Power power;
  
  LED(byte _pin, byte _target = 255, bool _invert = false)
  {
    pin = _pin;
    power.invert = _invert;
    power.steady = _target;
  }

  void timer(unsigned long now)
  {
    if (power.active) {
      if (power.action) {
        // apply effects
      } 
      //if (ShiftPWM.GetOne(pin) != power.state) { // this requires modifying the ShiftPWM library
        ShiftPWM.SetOne(pin, power.state);
      //}
    } else
    if (power.state) {
      power.off();
      ShiftPWM.SetOne(pin, power.state);
    }
  }
};
