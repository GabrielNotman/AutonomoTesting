#include <RTCZero.h>

RTCZero rtc;

void setup()
{
  rtc.begin(H24);

  // Set the alarm at the 10 second mark
  rtc.setAlarmSeconds(10);

  // Match only seconds (Periodic alarm every minute)
  rtc.enableAlarm(RTCZero::MATCH_SS);

  // Set LED pin as output
  pinMode(13, OUTPUT);

  // Set sleep mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  // Sleep sketch startup delay
  delay(5000);
}

void loop()
{
  // Disable USB
  USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
 
  //Enter sleep mode
  __WFI();
  
  // ...Sleep
  
  // Enable USB
  USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;

  // Blink LED
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  // Stay awake for two seconds
  delay(2000);
}
