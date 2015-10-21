// Make sure you select the new variant of the Autonomo
// as the target board.

// Serial2 D6  = RX, D7  = TX
// Serial3 D12 = RX, D13 = TX

void setup()
{
  Serial2.begin(9600);
  Serial3.begin(9600);
}

void loop() 
{
  while (Serial2.available()) {
    SerialUSB.write(Serial2.read());
  }

  while (Serial3.available()) {
    SerialUSB.write(Serial3.read());
  }
}
