/*
  Simple RTC for Arduino Zero

 Demonstrates the use of the RTC library for the Arduino Zero

 This example code is in the public domain

 http://arduino.cc/en/Tutorial/SimpleRTC

*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const uint8_t seconds = 0;
const uint8_t minutes = 0;
const uint8_t hours = 8;

/* Change these values to set the current initial date */
const uint8_t day = 15;
const uint8_t month = 6;
const uint8_t year = 15;

void setup()
{
  rtc.begin(H12); // initialize RTC 24H format. The dual option is H12
  
  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  
  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

  // Set the alarm at the 12 second mark
  rtc.setAlarmSeconds(12);

  // Match only seconds (Periodic alarm every minute)
  rtc.enableAlarm(RTCZero::MATCH_SS);

  // Attach ISR
  rtc.attachInterrupt(RTC_ISR);
  
  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(day, month, year);
}

void RTC_ISR()
{
  SerialUSB.println("RTC Alarm");
}

void loop()
{
  // Print date...
  SerialUSB.print(rtc.getDay());
  SerialUSB.print("/");
  SerialUSB.print(rtc.getMonth());
  SerialUSB.print("/");
  SerialUSB.print(rtc.getYear());
  SerialUSB.print("\t");
  
  // ...and time
  SerialUSB.print(rtc.getHours());
  SerialUSB.print(":");
  SerialUSB.print(rtc.getMinutes());
  SerialUSB.print(":");
  SerialUSB.print(rtc.getSeconds());
 
  SerialUSB.println();
  
  delay(1000);
}
