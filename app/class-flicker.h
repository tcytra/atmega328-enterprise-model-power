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
  int   level;
  unsigned long then;
  
  public:
  int   pin;
  int   state;
  Power power;
  
  Flicker(int _pin, int _wait = 100)
  {
    pin   = pin2Bit(_pin);
    wait  = _wait;
    then  = 0;
    state = 0;
  }
  
  void signal(byte &shift, unsigned long now)
  {
    if (power.available) {
      if (now -then >= wait) {
        then = now;
        state = random(120) +135;
        //analogWrite(pin, state);
      }
    } else
    if (state) {
      state = 0;
      then  = 0;
      //analogWrite(pin, state);
    }

    bitWrite(shift, pin, state);
  }
};
