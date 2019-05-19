
int pin2Bit(int pin)
{
  return (7 - pin);
};

class Shift
{
  byte  value = 0B00000000;
  
  public:

  void read(int _pin)
  {
    bitRead(value, _pin);
  }
  void write(int _pin, int _state)
  {
    bitWrite(value, _pin, _state);
  }
};
