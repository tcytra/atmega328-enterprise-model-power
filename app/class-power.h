/**
  Class Power
  Instantiate within an object to provide power management within that object

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 class-power.h 2019-05-18
*/
class Power
{
  public:
  byte  action; // -1 = power down, 0 = steady, 1 = power up
  byte  active; //  1 = on, 0 = off
  bool  invert; //  true (state 255 is off), false
  byte  steady; //  0-255 = normal operational level
  byte  state;  //  0-255 = current power state
  
  Power()
  {
    action  = 0;
    active  = 0;
    invert  = false;
    state   = 0;
    steady  = ConstantPower;
  }
  
  void off()
  {
    action  = 0;
    active  = 0;
    state   = invert ? 255 : 0;
  }
  void on()
  {
    action  = 0;
    active  = 1;
    state   = invert ? 255 -steady : steady;
  }
};
