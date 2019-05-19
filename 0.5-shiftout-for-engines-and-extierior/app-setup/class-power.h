/**
  Class Power
  Instantiate within an object to provide power management within that object

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.5 class-power.h 2019-05-18
*/
class Power
{
  public:
  Power()
  {  }
  
  int available = 0;
  int outlevel  = 0;
  
  void down()
  { available = 0; }
  
  void toggle()
  { available = available ? 0 : 1; }
  
  void up()
  { available = 1; }

  void signal()
  {  }

  String readStatus()
  {
    String powerStatus = (available ? "Up" : "Down");

    return powerStatus;
  }
};
