

#include <ESPZeroAT.h>

void setup() 
{
  SerialUSB.begin(115200);
  while(!SerialUSB);
  
  if(!ESP8266.begin(&Serial, 115200))
  {
    SerialUSB.println("No response from ESP8266 test command");
  }
}

void loop() 
{
  

}
