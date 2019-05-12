
class Flasher
{
  int on;
  int off;
  int pin;
  int power;
  int state;
  unsigned long then;

  public:
  Flasher(int _pin, long _on, long _off)
  {
    pin   = _pin;
    on    = _on;
    off   = _off;
    then  = 0;
    power = 0;
    state = LOW;
    
    pinMode(pin, OUTPUT);
  }
  
  void Power(int set = -1)
  {
    power = ((set > -1) ? set : ((power > 0) ? 0 : 1));
  }
  
  void Signal(unsigned long now)
  {
    if (power) {
      if ((state == HIGH) && (now -then >= on)) {
        Write(now, LOW);
      } else
      if ((state == LOW) && (now -then >= off)) {
        Write(now, HIGH);
      }
    } else
    if (state == HIGH) {
      Write(0, LOW);
      then = 0;
    }
  }
  
  void Write(unsigned long _now, int _state)
  {
    then  = _now;
    state = _state;
    digitalWrite(pin, state);
  }
};

class Flicker
{
  int pin;
  int wait;
  int level;
  int power;
  int state;
  unsigned long then;
  
  public:
  Flicker(int _pin)
  {
    pin   = _pin;
    wait  = 100;
    then  = 0;
    power = 0;
    state = 0;
    pinMode(pin, OUTPUT);
  }
  
  void Power(int set = -1)
  {
    power = ((set > -1) ? set : ((power > 0) ? 0 : 1));
  }
  
  void Signal(unsigned long now)
  {
    if (power) {
      if (now -then >= wait) {
        then = now;
        state = random(120) +135;
        analogWrite(pin, state);
      }
    } else
    if (state) {
      state = 0;
      analogWrite(pin, state);
    }
  }
};

int powerExterior;
int powerInterior;
int powerEngines;
int powerWeapons;

/** @todo Markers/Strobes will require powerExterior */
Flasher exteriorMarkers(13, 1000, 1000);
Flasher exteriorStrobes(12, 64, 1200);

/** @todo Thrusters will require powerEngines */
Flicker engineThrustInner(9);
Flicker engineThrustOuter(10);

void setup() {
  Serial.begin(9600);

  exteriorMarkers.Power();
  exteriorStrobes.Power();
  engineThrustInner.Power();
  engineThrustOuter.Power();
  
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();
  
  exteriorMarkers.Signal(now);
  exteriorStrobes.Signal(now);
  engineThrustInner.Signal(now);
  engineThrustOuter.Signal(now);
}

void loop() {
  
}
