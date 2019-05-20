/**
  Class Button
  Manage a circuit button
  
  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 class-button.h 2019-05-19
*/
class Button
{
  int   pin;
  
  unsigned long stamp;
  
  public:
  Button(int _pin){
    pin   = _pin;
  }
};
