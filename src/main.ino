/*
 * Program to run flight operations
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 12, 2025
 */

#include "OutIMU.ino"

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
  }
  

}
