#include <SPI.h> 
#include "Sodaq_dataflash.h"

#define PAGE 5 // PAGE < DF_NR_PAGES
#define BLANK_PAGE 36 // BLANK_PAGE < DF_NR_PAGES
#define ADDR 12 // (ADDR + Length(DATA) < DF_PAGE_SIZE

void setup() 
{
  while (!SerialUSB) {}
  char DATA[] = "This is some test data 12345...";
  SerialUSB.begin(57600);
  
  SerialUSB.println("Initialising Flash...");
  dflash.init(); 
  
  //Erase both pages
  dflash.pageErase(PAGE);
  dflash.pageErase(BLANK_PAGE);
  
  //Clear buf1 by reading the blank page
  dflash.readPageToBuf1(BLANK_PAGE);
  
  //Write data to buf1
  SerialUSB.println("--------------------");
  SerialUSB.println("Writing to page: " + String(5, DEC));
  SerialUSB.println("At address: " + String(12, DEC));
  SerialUSB.println("Data: " + String(DATA));
  SerialUSB.println("--------------------");
  size_t length = strlen(DATA);
  dflash.writeStrBuf1(ADDR, (uint8_t*)DATA, length);
  
  //Write buf1 to flash PAGE
  dflash.writeBuf1ToPage(PAGE);
  
  //Clear buf1 by reading the blank page
  dflash.readPageToBuf1(BLANK_PAGE);
  
  //Read back the data from PAGE
  char buffer[length+1];
  dflash.readPageToBuf1(PAGE);
  dflash.readStrBuf1(ADDR, (uint8_t*)buffer, length);
  buffer[length] = '\0'; //Add null termination 
  
  SerialUSB.println("--------------------");
  SerialUSB.println("Reading data back...");
  SerialUSB.println(buffer);
  SerialUSB.println("--------------------");
}

void loop() 
{
}
