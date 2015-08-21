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
  digitalWrite(BEE_VCC, HIGH);

  //Power switching is still being resolved 
  gprsbee.init(Serial1, CTS, DTR);  
  gprsbee.setDiag(SerialUSB);

  //Comment out this line when used with GPRSbee Rev.4
  gprsbee.setPowerSwitchedOnOff(true);
}

void loop()
{
  //Some send data
  char* data="Some payload data...";

  //Response buffer
  char buffer[512];
  memset(buffer, '\0', sizeof(buffer));
  
  bool retval = gprsbee.doHTTPPOSTWithReply(APN, APN_USERNAME, APN_PASSWORD, 
    "http://httpbin.org/post", data, strlen(data), buffer, sizeof(buffer));
  
  SerialUSB.println(retval);
  SerialUSB.println("POST result: ");

  //Lets be safe in case result length > 247
  for (int i=0; i<strlen(buffer); i++) {
    SerialUSB.print(buffer[i]);
  }
} 
