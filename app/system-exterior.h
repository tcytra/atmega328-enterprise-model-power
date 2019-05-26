/**
  Class ExteriorSystem
  Manage the power to the various external systems

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 system-exterior.h 2019-05-18
*/
class ExteriorSystem: public System
{
  byte  state = 0;
  
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
    flood1  = new LED(3, ConstantPower);
    flood2  = new LED(4, ConstantPower);
    flood3  = new LED(5, ConstantPower);
    flood4  = new LED(6, ConstantPower);
  }

  void cycle(byte up = 1)
  {
    byte maxbit = 0;
    byte minbit = 0;
    
    for (int i = 1; i < 7; i++) {
      if (bitRead(state, i)) {
        if (!minbit) { minbit = i; }
      }
    }
    for (int i = 1; i < 7; i++) {
      maxbit = i;
      if (!bitRead(state, i)) {
        break;
      }
    }

    if (up) {
      bitWrite(state, maxbit, 1);
    } else if (state) {
      bitWrite(state, minbit, 0);
    }
    
    if (bitRead(state, 1)) {
      markers->power.on();
    } else {
      markers->power.off();
    }
    if (bitRead(state, 2)) {
      strobes->power.on();
    } else {
      strobes->power.off();
    }
    if (bitRead(state, 3)) {
      flood1->power.on();
    } else {
      flood1->power.off();
    }
    if (bitRead(state, 4)) {
      flood2->power.on();
    } else {
      flood2->power.off();
    }
    if (bitRead(state, 5)) {
      flood3->power.on();
    } else {
      flood3->power.off();
    }
    if (bitRead(state, 6)) {
      flood4->power.on();
    } else {
      flood4->power.off();
    }
  }
  
  void signal(unsigned long now)
  {
    markers->timer(now);
    strobes->timer(now);
    flood1->timer(now);
    flood2->timer(now);
    flood3->timer(now);
    flood4->timer(now);
  }
};
