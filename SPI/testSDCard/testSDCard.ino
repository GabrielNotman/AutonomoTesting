//Standard SD sketch ListFiles
//Only modifications are chaning Serial to SerialUSB
//for output and changing the CS pin in SD.begin().
//Other SD sketch should work with the same modification.
/*
  Listfiles
 
 This example shows how print out the files in a 
 directory on a SD card 
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald
 
 This example code is in the public domain.

 */
#include <SPI.h>
#include <SD.h>

File root;

void setup()
{
  // Open serial communications and wait for port to open:
  SerialUSB.begin(9600);
  while (!SerialUSB) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  SerialUSB.println("MOSI " + String(MOSI));
  SerialUSB.println("MISO " + String(MISO));
  SerialUSB.println("SCK " + String(SCK));
  SerialUSB.println("SS " + String(SS));
  
  SerialUSB.print("Initializing SD card...");

  if (!SD.begin(SS_2)) {
    SerialUSB.println("initialization failed!");
    return;
  }
  SerialUSB.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  SerialUSB.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       SerialUSB.print('\t');
     }
     SerialUSB.print(entry.name());
     if (entry.isDirectory()) {
       SerialUSB.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       SerialUSB.print("\t\t");
       SerialUSB.println(entry.size(), DEC);
     }
     entry.close();
   }
}



