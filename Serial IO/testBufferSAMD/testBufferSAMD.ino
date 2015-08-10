void setup() 
{
  //Wait until the serial monitor is ready/open
  while(!SerialUSB);
  char buffer1[512];

  //Strangely the limit seems to be 247 characters?
  //But it seems to start to fail after 240?

  //This works
  memset(buffer1, 'a', sizeof(buffer1));
  buffer1[247] = 0;
  SerialUSB.println("Attempting to print a 247 character buffer: ");
  SerialUSB.println(buffer1);

  //But this hangs
  memset(buffer1, 'a', sizeof(buffer1));
  buffer1[248] = 0;
  SerialUSB.println("Attempting to print a 248 character buffer: ");
  SerialUSB.println("Individual characters:");
  for (int i=0; i<248; i++)
    SerialUSB.print(buffer1[i]);
  SerialUSB.println();
  SerialUSB.println("In a single call to SerialUSB.println():");
  SerialUSB.println(buffer1);
}

void loop() 
{
}
