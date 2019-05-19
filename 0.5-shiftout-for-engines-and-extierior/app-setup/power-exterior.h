/**
  Class ExteriorPower
  Manage the power to the various external systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 power-exterior.h 2019-05-18
*/
class ExteriorPower
{
  byte shift = 0B00000000;
  
  public:
  
  Flasher *markers;
  Flasher *strobes;
  Power power;
  
  ExteriorPower(int markerPin, int strobePin)
  {
    markers = new Flasher(shift, markerPin, 1000, 1000);
    strobes = new Flasher(shift, strobePin, 64, 1200);
  }
};
