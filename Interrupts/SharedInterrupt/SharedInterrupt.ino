//D14 & A11 share external interrupt vector 3
#define INT1 14
#define INT2 A11

void setup() 
{
  //Wait until the serial monitor is ready/open
  while(!SerialUSB);
  SerialUSB.println("Starting");

  //This isn't strictly necessary as default is INPUT
  pinMode(INT1, INPUT); 
  pinMode(INT2, INPUT);

  //Attach the interrupts to RISING events
  attachInterrupt(INT1, ISR1, RISING);
  attachInterrupt(INT2, ISR2, RISING);
}

void ISR1()
{
  SerialUSB.println("Event1!");
  SerialUSB.println("D14=" + String(digitalRead(INT1)));
  SerialUSB.println("A11=" + String(digitalRead(INT2)));
}

void ISR2()
{
  SerialUSB.println("Event2!");
  SerialUSB.println("D14=" + String(digitalRead(INT1)));
  SerialUSB.println("A1=" + String(digitalRead(INT2)));
}


void loop() 
{
}
