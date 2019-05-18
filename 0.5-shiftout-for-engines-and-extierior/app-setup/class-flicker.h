/**
  Class Flicker
  Manage a candlelight type flicker effect

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 class-flicker.h 2019-05-17
*/
class Flicker
{
  int pin;
  int wait;
  int level;
  int power;
  int state;
  unsigned long then;
  
  public:
  
  Flicker(int _pin, int _shift = 0)
  {
    pin   = _shift ? pin2Bit(_pin) : _pin;
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
