#define inputPin A0
int volume;

void setup() 
{
 while(!SerialUSB);
 SerialUSB.write("Start");
}

void loop() 
{
 SerialUSB.print("Loop: ");
 volume = analogRead(inputPin);
 SerialUSB.println(volume);
 delay(100);
}
