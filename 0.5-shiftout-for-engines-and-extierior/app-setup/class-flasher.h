/**
  Class Flasher
  Manage a flashing light effect
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 class-flasher.h 2019-05-17
*/
class Flasher
{
  int   on;
  int   off;
  unsigned long then;

  public:
  int   pin;
  int   state;
  Power power;
  
  Flasher(int _pin, long _on, long _off)
  {
    pin   = pin2Bit(_pin);
    on    = _on;
    off   = _off;
    then  = 0;
    state = 0;
  }
  
  void signal(byte &shift, unsigned long now)
  {
    if (power.available) {
      if ((state == 1) && (now -then >= on)) {
        state = 0;
        then  = now;
      } else
      if ((state == 0) && (now -then >= off)) {
        state = 1;
        then  = now;
      }
    } else
    if (state == 1) {
      state = 0;
      then  = 0;
    }

    bitWrite(shift, pin, state);
  }
  
  /*void Write(unsigned long _now, int _state)
  {
    //if(_state){ Serial.println("Marker On"); }else{ Serial.println("Marker Off"); }
    
    then  = _now;
    state = _state;
    bitWrite(shift, pin, state);

    //Serial.println(shift, BIN);
  }*/
};
