/**
  Class Flasher
  Create a flashing light effect
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.1 obj-flicker 2019-05-17
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
    state = 0;
    
    pinMode(pin, OUTPUT);

    digitalWrite(pin, 0);
  }
  
  void Power(int set = -1)
  {
    power = ((set > -1) ? set : ((power > 0) ? 0 : 1));
  }
  
  void Signal(unsigned long now)
  {
    if (power) {
      if ((state == 1) && (now -then >= on)) {
        Write(now, 0);
      } else
      if ((state == 0) && (now -then >= off)) {
        Write(now, 1);
      }
    } else
    if (state == 1) {
      Write(0, 0);
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
