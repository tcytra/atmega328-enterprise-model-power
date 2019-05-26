/**
  Class Flicker
  Manage a candlelight type flicker effect

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 class-flicker.h 2019-05-17
  @todo     This needs to be implemented as a trait (of LED?)
*/
class Flicker
{
  int   wait;
  int   maximum;
  int   minimum;
  
  unsigned long then;
  
  public:
  
  int   pin;
  Power power;
  
  Flicker(int _pin, int _wait = 100, byte _min = 0, byte _max = 255)
  {
    pin   = _pin;
    wait  = _wait;
    then  = 0;
    maximum = _max;
    minimum = _min;
  }

  void timer(unsigned long now)
  {
    if (power.active) {
      if (now -then >= wait) {
        then = now;
        power.state = random(maximum - minimum) +minimum;
        ShiftPWM.SetOne(pin, power.state);
      }
    } else {
      power.state = 0;
      then  = 0;
      ShiftPWM.SetOne(pin, power.state);
    }
  }
};
