void setup() 
{
  //Wait for SerialUSB or 10 seconds
  while ((!SerialUSB) && (millis() < 10000));
  
  SerialUSB.println("Serial monitor opened...");

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
