
class Flasher
{
  int on;
  int off;
  int pin;
  int state;
  
  unsigned long then;

  public:
  Flasher(int _pin, long _on, long _off)
  {
    pin   = _pin;
    on    = _on;
    off   = _off;
    then  = 0;
    state = LOW;
    
    pinMode(pin, OUTPUT);
  }
  
  void Signal(unsigned long now)
  {
    if ((state == HIGH) && (now -then >= on)) {
      state = LOW;
      then  = now;
      digitalWrite(pin, state);
    } else
    if ((state == LOW) && (now -then >= off)) {
      state = HIGH;
      then  = now;
      digitalWrite(pin, state);
    }
  }
};

Flasher exteriorMarkers(13, 1000, 1000);
Flasher exteriorStrobes(12, 64, 1200);

void setup() {
  Serial.begin(9600);

  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long now = millis();
  
  exteriorMarkers.Signal(now);
  exteriorStrobes.Signal(now);
}

void loop() {
  
}
