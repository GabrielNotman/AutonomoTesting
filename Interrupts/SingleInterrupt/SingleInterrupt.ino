//Interrupt on D6
#define INT1 6

void setup() 
{
  //Wait until the serial monitor is ready/open
  while(!SerialUSB);
  SerialUSB.println("Starting");

  //This isn't strictly necessary as default is INPUT
  pinMode(INT1, INPUT); 

  //Attach the interrupt to RISING event
  attachInterrupt(INT1, ISR, RISING);
}

void ISR()
{
  SerialUSB.println("Event!");
}


void loop() 
{
}
