
#include <RTCZero.h>

#define DEBUG_STREAM Serial
#define DEBUG_BAUD 9600

RTCZero rtc;

void setup() 
{
  DEBUG_STREAM.begin(DEBUG_BAUD);
  DEBUG_STREAM.println("----------- Startup -----------");

  // Reset the RTC time values to demonstrate bug  
  rtc.begin(true);
}

void loop() 
{
  DEBUG_STREAM.println(String("Date: ") + rtc.getDay() + ":" + rtc.getMonth() + ":" + (rtc.getYear() + 2000));
  DEBUG_STREAM.println(String("Time: ") + rtc.getHours() + ":" + rtc.getMinutes() + ":" + rtc.getSeconds());
  
  DEBUG_STREAM.print("Epoch: ");
  DEBUG_STREAM.println(rtc.getEpoch());
  delay(1000);
}
