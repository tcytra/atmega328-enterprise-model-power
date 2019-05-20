/**
  Class Button
  Manage a circuit button
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 class-button.h 2019-05-20
*/
class Button
{
  byte  pin;
  byte  click;
  byte  state;
  unsigned long stamp;
  
  public:
  Button(byte _pin){
    pin   = _pin;
  }
};
