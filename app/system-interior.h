/**
  Class InteriorSystem
  Manage the power to the various internal systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 system-interior.h 2019-05-18
*/
class InteriorSystem: public System
{
  public:
  
  Power power;
  
  InteriorSystem(){}

  void signal(unsigned long now)
  {  }
};
