#define APN ""
#define APN_USERNAME ""
#define APN_PASSWORD ""

#include "GPRSbee.h"

void setup()
{
  //Wait until the serial monitor is ready/open
  while(!SerialUSB);

  //Start the Bee Serial port initially
  Serial1.begin(57600);
  
  //Switch on the VCC for the Bee socket
  digitalWrite(BEE_VCC, HIGH);

  gprsbee.init(Serial1, CTS, DTR);  
  gprsbee.setDiag(SerialUSB);

  //Comment out this line when used with GPRSbee Rev.4
  gprsbee.setPowerSwitchedOnOff(true);
}

void loop()
{
  char buffer[512];

  memset(buffer, '\0', sizeof(buffer));
  bool retval = gprsbee.doHTTPGET(APN, APN_USERNAME, APN_PASSWORD, 
    "http://httpbin.org/get", buffer, sizeof(buffer));

  SerialUSB.println(retval);
  SerialUSB.println("GET result: ");

  //Lets be safe in case result length > 247
  for (int i=0; i<strlen(buffer); i++) {
    SerialUSB.print(buffer[i]);
  }
} 
