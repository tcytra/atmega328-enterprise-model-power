/**
  Class System
  Other System object extend this class

  @author   Todd Cytra <tcytra@gmail.com>
  @version  0.6 class-system.h 2019-05-20
*/
class System
{
  public:
  Button  *button1;
  Button  *button2;

  void addButtons(int primary = 0, int secondary = 0)
  {
    button1 = new Button(primary);
    button2 = new Button(secondary);
  }
};
