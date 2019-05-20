
byte pin2Bit(byte pin)
{
  return (7 - pin);
};

class Shift
{
  byte  value = 0B00000000;
  
  public:

  void read(byte _pin)
  {
    bitRead(value, _pin);
  }
  void write(byte _pin, byte _state)
  {
    bitWrite(value, _pin, _state);
  }
};
