/**
  Class Flicker
  Create a candlelight type flicker effect

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.1 obj-led 2019-05-17
*/
class LED
{
  int pin;
  int power;
  int state;
  
  public:

  LED(int _pin)
  {
    pin   = _pin;
    power = 0;
    state = 0;
  }

  
};
