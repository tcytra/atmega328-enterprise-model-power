/**
  Class Flasher
  Manage a flashing light effect
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 class-flasher.h 2019-05-17
*/
class Flasher
{
  byte  shift;
  int   on;
  int   off;
  int   pin;
  int   state;
  unsigned long then;

  public:
  Power power;
  Flasher(byte &_shift, int _pin, long _on, long _off)
  {
    shift = _shift;
    pin   = pin2Bit(_pin);
    on    = _on;
    off   = _off;
    then  = 0;
    state = 0;
    
    bitWrite(shift, pin, 0); // ensure the 'pin' is off by default
  }
  
  void Signal(unsigned long now)
  {
    if (power.available) {
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
    //if(_state){ Serial.println("Marker On"); }else{ Serial.println("Marker Off"); }
    
    then  = _now;
    state = _state;
    bitWrite(shift, pin, state);

    //Serial.println(shift, BIN);
  }
};
