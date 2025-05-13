/*
 * Program to run flight operations
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 12, 2025
 */

#include "OutIMU.ino"
#include "BMP.ino"

int main();
{
  Bool flightControl = false;
  
  OutIMU.setup(void)
  if(!flightControl)
  {
    //Checks for launch
  }
  
  While(flightControl)
  {
    OutIMU.loop()
    BMP.loop()
  }
  

}
