//Note: Do not remove the start up delay.
//After a reset, this delay gives you 
//time to upload a new sketch before
//the board enters sleep mode.

#define INT1 4

void setup() 
{
  //This isn't strictly necessary as default is INPUT
  pinMode(INT1, INPUT); 
  pinMode(13, OUTPUT);

  //Attach the interrupt and set the wake flag 
  attachInterrupt(INT1, ISR, HIGH);
  EIC->WAKEUP.reg |= (1 << digitalPinToInterrupt(INT1));

  //Set sleep mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  
  //Start delay of 10s to allow for new upload after reset
  delay(10000);
}

void ISR()
{
  //Blink the LED for 5s
  digitalWrite(13, HIGH);
  delayMicroseconds(500000);
  digitalWrite(13, LOW);
}

void loop() 
{
  //Uncomment this line to sleep immediately
  //on exit from any ISR
  //Can't do this in setup() as it will
  //sleep when exiting from that method
  //SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
  
  //Disable USB
  USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
 
  //Enter sleep mode
  __WFI();
  
  //...Sleep
  
  //Enable USB
  USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE;

  //Stay awake 10s
  delay(10000);
}
