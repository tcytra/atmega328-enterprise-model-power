/**
  Class ExteriorPower
  Manage the power to the various external systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 power-exterior.h 2019-05-18
*/
class ExteriorPower
{
  byte  shift = 0B00000000;
  
  public:

  Button  *button1;
  Button  *button2;
  Flasher *markers;
  Flasher *strobes;
  LED     *flood1;
  LED     *flood2;
  LED     *flood3;
  LED     *flood4;
  Power    power;
  
  ExteriorPower(int markerPin, int strobePin)
  {
    markers = new Flasher(markerPin, 1000, 1000);
    strobes = new Flasher(strobePin, 64, 1200);
    flood1  = new LED(3);
    flood2  = new LED(4);
    flood3  = new LED(5);
    flood4  = new LED(6);
  }
  void addButtons(int primary = 0, int secondary = 0)
  {
    button1 = new Button(primary);
    button2 = new Button(secondary);
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
