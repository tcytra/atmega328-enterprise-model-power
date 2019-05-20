/**
  Class ExteriorSystem
  Manage the power to the various external systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 system-exterior.h 2019-05-18
*/
class ExteriorSystem: public System
{
  byte  shift = 0B00000000;
  
  public:

  Flasher *markers;
  Flasher *strobes;
  LED     *flood1;
  LED     *flood2;
  LED     *flood3;
  LED     *flood4;
  Power    power;
  
  ExteriorSystem(byte markerPin, byte strobePin)
  {
    markers = new Flasher(markerPin, 1000, 1000);
    strobes = new Flasher(strobePin, 64, 1200);
    flood1  = new LED(3);
    flood2  = new LED(4);
    flood3  = new LED(5);
    flood4  = new LED(6);
  }

  byte shiftRead()
  {
    return shift;
  }
  
  void signal(unsigned long now)
  {
    markers->signal(shift, now);
    strobes->signal(shift, now);
    flood1->signal(shift);
    flood2->signal(shift);
    flood3->signal(shift);
    flood4->signal(shift);
  }
};
