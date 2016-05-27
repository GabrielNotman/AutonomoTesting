/*
  AirQuality library v1.0
  2010 Copyright (c) Seeed Technology Inc.  All right reserved.
 
  Original Author: Bruce.Qin
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include"Arduino.h"
#include"AirQuality.h"

#define DEBUG_STREAM SerialUSB

//Get the avg voltage in 5 minutes.
void AirQuality::avgVoltage()
{
  if(i == 150) //sum 5 minutes
  {
    vol_standard = temp / 150; 
    temp = 0;
    DEBUG_STREAM.print("Vol_standard in 5 minutes: ");			
    DEBUG_STREAM.println(vol_standard);		
    i = 0;		
  }
  else {
    temp += first_vol;
    i++;
	}
}

void AirQuality::init(int pin)
{
  readPin = pin;
  pinMode(readPin, INPUT);

  delay(5000);
  DEBUG_STREAM.println("sys_starting...");
  delay(20000);
  
  init_voltage = analogRead(readPin);

  DEBUG_STREAM.println("The init voltage is ...");
  DEBUG_STREAM.println(init_voltage);

  unsigned char j = 0;
  while(init_voltage)
  {
    if((init_voltage < 798) && (init_voltage > 10)) // the init voltage is ok
    {
        first_vol = analogRead(readPin); //initialize first value
        last_vol = first_vol;
        vol_standard = last_vol;
        DEBUG_STREAM.println("Sensor ready.");
        error = false;
        break;
    }
    else {	
      j++;
      DEBUG_STREAM.println("waiting sensor init..(it takes 60 seconds to init)");
      delay(60000); //60000
      init_voltage = analogRead(readPin);
      if(j == 5) {
        j = 0;
        error = true;
        DEBUG_STREAM.println("Sensor Error!");
      }
    }
  }
  
  DEBUG_STREAM.println("Test begin...");
}

int AirQuality::slope(void)
{
  if (timer_index > 0) {
    if(((first_vol - last_vol) > 400) || (first_vol > 700)) {
      DEBUG_STREAM.println("High pollution! Force signal active.");		
      timer_index=0;	
      avgVoltage();	
      return 0;	
    }
    else if ((((first_vol - last_vol) > 400) && (first_vol < 700)) || ((first_vol - vol_standard) > 150)) {	
      DEBUG_STREAM.print("sensor_value:");		
      DEBUG_STREAM.print(first_vol);      		
      DEBUG_STREAM.println("\t High pollution!");		
      timer_index=0;	
      avgVoltage();
      return 1;
    }
    else if((((first_vol - last_vol) > 200) && (first_vol < 700))|| ((first_vol - vol_standard) > 50)) {
      //Serial.println(first_vol-last_vol);
      DEBUG_STREAM.print("sensor_value:");
      DEBUG_STREAM.print(first_vol);      		
      DEBUG_STREAM.println("\t Low pollution!");		
      timer_index=0;
      avgVoltage();
      return 2;	
    }
    else {
      avgVoltage();	
      DEBUG_STREAM.print("sensor_value:");
      DEBUG_STREAM.print(first_vol);
      DEBUG_STREAM.println("\t Air fresh");
      timer_index=0;
      return 3;
    }
	}
 
  return -1;
}

