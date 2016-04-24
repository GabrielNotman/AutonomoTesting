//Note: Do not remove the start up delay.
//After a reset, this delay gives you 
//time to upload a new sketch before
//the board enters sleep mode.

#define INT1 4

void setup() 
{
  pinMode(INT1, INPUT); 
  pinMode(13, OUTPUT);

  //Attach the interrupt and set the wake flag 
  attachInterrupt(INT1, ISR, RISING);
    
  // Set the XOSC32K to run in standby
  SYSCTRL->XOSC32K.bit.RUNSTDBY = 1;

  // Configure EIC to use GCLK1 which uses XOSC32K 
  // This has to be done after the first call to attachInterrupt()
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_EIC) | 
                      GCLK_CLKCTRL_GEN_GCLK1 | 
                      GCLK_CLKCTRL_CLKEN;

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
