#define APN ""
#define APN_USERNAME ""
#define APN_PASSWORD ""

#include "GPRSbee.h"

void setup()
{
  //Wait until the serial monitor is ready/open
  while(!SerialUSB);
  
  //Open Serial1 for the GPRSbee
  Serial1.begin(57600);

  //Switch on the VCC for the Bee socket
  //Also keep DTR high
  digitalWrite(BEE_VCC, HIGH);
  digitalWrite(DTR, HIGH);

  //Power switching is still being resolved 
  gprsbee.init(Serial1, CTS, 0);  
  gprsbee.setDiag(SerialUSB);

  char buffer1[512];

  memset(buffer1, '\0', sizeof(buffer1));
  bool retval = gprsbee.doHTTPGET(APN, APN_USERNAME, APN_PASSWORD, 
    "http://httpbin.org/get", buffer1, sizeof(buffer1));
  
  SerialUSB.println(retval);
  SerialUSB.println("GET result: ");

  //Lets be safe in case result length > 247
  for (int i=0; i<strlen(buffer1); i++) {
    SerialUSB.print(buffer1[i]);
  }
}

void loop()
{
} 
