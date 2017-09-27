/** 
 *  @file     ESP8266_Test.ino
 *  @author   Clint Stevenson (Casco Logix)
 *  @license  GPLv3
 *  
 *  @description  
 *  This is a demo sketch for the Tuffduino for testing the existence of
 *  the ESP8266 WiFi module by using the test() method in the ESPZeroAT
 *	library. If the WiFi module is found the serial terminal will display
 *	a message as such; likewise if it is not found. 
 *
 *  Copyright (C) 2017  CascoLogix (http://cascologix.com)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  @history
 *  
 *  v1.0 - First release
 */

#include <ESPZeroAT.h>

void setup() 
{
  SerialUSB.begin(115200);
  while(!SerialUSB);

  SerialUSB.println("Begin.");
  
  if(!ESP8266.begin(&Serial, 115200))
  {
    SerialUSB.println("No response from ESP8266 test command");
  }
  
  else
  {
    SerialUSB.println("Found ESP8266 with test command!");
  }
}

void loop() 
{

}