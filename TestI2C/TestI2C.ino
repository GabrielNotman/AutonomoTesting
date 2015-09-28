#include <Wire.h>
#include <Sodaq_SHT2x.h>

void setup() 
{
  Wire.begin();
}

void loop() 
{
  //Read the SHT2x temperature from TPH
  //BMP085 library is not SAMD ready yet
  SerialUSB.println(SHT2x.GetTemperature());
  delay(1000);
}
