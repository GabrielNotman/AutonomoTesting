/*
 * Copyright (c) 2013 Kees Bakker.  All rights reserved.
 *
 * This file is part of Sodaq_dataflash.
 *
 * Sodaq_dataflash is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published bythe Free Software Foundation, either version 3 of
 * the License, or(at your option) any later version.
 *
 * Sodaq_dataflash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sodaq_dataflash.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * This library was inspired by dataflash libraries by Atmel, asynclabs,
 * and others.
 */

#include <inttypes.h>
#include <Arduino.h>
#include <SPI.h>

#include "Sodaq_dataflash.h"

//Dataflash commands
#define FlashPageRead           0xD2    // Main memory page read
#define StatusReg               0xD7    // Status register
#define ReadMfgID               0x9F    // Read Manufacturer and Device ID
#define PageErase               0x81    // Page erase
#define ReadSecReg              0x77    // Read Security Register

#define FlashToBuf1Transfer     0x53    // Main memory page to buffer 1 transfer
#define Buf1Read                0xD4    // Buffer 1 read
#define Buf1ToFlashWE           0x83    // Buffer 1 to main memory page program with built-in erase
#define Buf1Write               0x84    // Buffer 1 write

#define FlashToBuf2Transfer     0x55    // Main memory page to buffer 2 transfer
#define Buf2Read                0xD6    // Buffer 2 read
#define Buf2ToFlashWE           0x86    // Buffer 2 to main memory page program with built-in erase
#define Buf2Write               0x87    // Buffer 2 write


void Sodaq_Dataflash::init(uint8_t csPin)
{
  // Setup the slave select pin
  _csPin = csPin;
  
  // Call the standard SPI initialisation
  SPI.begin();

  // This is used when CS != SS
  pinMode(_csPin, OUTPUT);

#if DF_VARIANT == DF_AT45DB081D
  _pageAddrShift = 1;
#elif DF_VARIANT == DF_AT45DB161D
  _pageAddrShift = 1;
#endif
}

void Sodaq_Dataflash::init(uint8_t misoPin, uint8_t mosiPin, uint8_t sckPin, uint8_t ssPin)
{
  init(ssPin);
}

uint8_t Sodaq_Dataflash::transmit(uint8_t data)
{
  // Call the standard SPI transfer method
  return SPI.transfer(data);
}

uint8_t Sodaq_Dataflash::readStatus()
{
  unsigned char result;

  activate();
  result = transmit(StatusReg);
  result = transmit(0x00);
  deactivate();

  return result;
}

// Monitor the status register, wait until busy-flag is high
void Sodaq_Dataflash::waitTillReady()
{
  while (!(readStatus() & 0x80)) {
    // WDT reset maybe??
  }
}

void Sodaq_Dataflash::readID(uint8_t *data)
{
  activate();
  transmit(ReadMfgID);
  data[0] = transmit(0x00);
  data[1] = transmit(0x00);
  data[2] = transmit(0x00);
  data[3] = transmit(0x00);
  deactivate();
}

// Reads a number of bytes from one of the Dataflash security register
void Sodaq_Dataflash::readSecurityReg(uint8_t *data, size_t size)
{
    activate();
    transmit(ReadSecReg);
    transmit(0x00);
    transmit(0x00);
    transmit(0x00);
    for (size_t i = 0; i < size; i++) {
      *data++ = transmit(0x00);
    }
    deactivate();
}

// Transfers a page from flash to Dataflash SRAM buffer
void Sodaq_Dataflash::readPageToBuf1(uint16_t pageAddr)
{
  activate();
  transmit(FlashToBuf1Transfer);
  setPageAddr(pageAddr);
  deactivate();
  waitTillReady();
}

// Reads one byte from one of the Dataflash internal SRAM buffer 1
uint8_t Sodaq_Dataflash::readByteBuf1(uint16_t addr)
{
  unsigned char data = 0;

  activate();
  transmit(Buf1Read);
  transmit(0x00);               //don't care
  transmit((uint8_t) (addr >> 8));
  transmit((uint8_t) (addr));
  transmit(0x00);               //don't care
  data = transmit(0x00);        //read byte
  deactivate();

  return data;
}

// Reads a number of bytes from one of the Dataflash internal SRAM buffer 1
void Sodaq_Dataflash::readStrBuf1(uint16_t addr, uint8_t *data, size_t size)
{
  activate();
  transmit(Buf1Read);
  transmit(0x00);               //don't care
  transmit((uint8_t) (addr >> 8));
  transmit((uint8_t) (addr));
  transmit(0x00);               //don't care
  for (size_t i = 0; i < size; i++) {
    *data++ = transmit(0x00);
  }
  deactivate();
}

// Writes one byte to one to the Dataflash internal SRAM buffer 1
void Sodaq_Dataflash::writeByteBuf1(uint16_t addr, uint8_t data)
{
  activate();
  transmit(Buf1Write);
  transmit(0x00);               //don't care
  transmit((uint8_t) (addr >> 8));
  transmit((uint8_t) (addr));
  transmit(data);               //write data byte
  deactivate();
}

// Writes a number of bytes to one of the Dataflash internal SRAM buffer 1
void Sodaq_Dataflash::writeStrBuf1(uint16_t addr, uint8_t *data, size_t size)
{
  activate();
  transmit(Buf1Write);
  transmit(0x00);               //don't care
  transmit((uint8_t) (addr >> 8));
  transmit((uint8_t) (addr));
  for (size_t i = 0; i < size; i++) {
    transmit(*data++);
  }
  deactivate();
}

// Transfers Dataflash SRAM buffer 1 to flash page
void Sodaq_Dataflash::writeBuf1ToPage(uint16_t pageAddr)
{
  activate();
  transmit(Buf1ToFlashWE);
  setPageAddr(pageAddr);
  deactivate();
  waitTillReady();
}

void Sodaq_Dataflash::pageErase(uint16_t pageAddr)
{
  activate();
  transmit(PageErase);
  setPageAddr(pageAddr);
  deactivate();
  waitTillReady();
}

void Sodaq_Dataflash::chipErase()
{
  activate();
  transmit(0xC7);
  transmit(0x94);
  transmit(0x80);
  transmit(0x9A);
  deactivate();
  waitTillReady();
}

void Sodaq_Dataflash::settings(SPISettings settings)
{
  _settings = settings;
}

void Sodaq_Dataflash::deactivate()
{
    digitalWrite(_csPin,HIGH);
    SPI.endTransaction();
}
void Sodaq_Dataflash::activate()
{
    SPI.beginTransaction(_settings);
    digitalWrite(_csPin,LOW);
}

void Sodaq_Dataflash::setPageAddr(unsigned int pageAddr)
{
  transmit(getPageAddrByte0(pageAddr));
  transmit(getPageAddrByte1(pageAddr));
  transmit(getPageAddrByte2(pageAddr));
}

/*
 * From the AT45DB081D documentation (other variants are not really identical)
 *   "For the DataFlash standard page size (264-bytes), the opcode must be
 *    followed by three address bytes consist of three don’t care bits,
 *    12 page address bits (PA11 - PA0) that specify the page in the main
 *    memory to be written and nine don’t care bits."
 */
/*
 * From the AT45DB161B documentation
 *   "For the standard DataFlash page size (528 bytes), the opcode must be
 *    followed by three address bytes consist of 2 don’t care bits, 12 page
 *    address bits (PA11 - PA0) that specify the page in the main memory to
 *    be written and 10 don’t care bits."
 */
uint8_t Sodaq_Dataflash::getPageAddrByte0(uint16_t pageAddr)
{
  // More correct would be to use a 24 bits number
  // shift to the left by number of bits. But the uint16_t can be considered
  // as if it was already shifted by 8.
  return (pageAddr << (DF_PAGE_BITS - 8)) >> 8;
}
uint8_t Sodaq_Dataflash::getPageAddrByte1(uint16_t page)
{
  return page << (DF_PAGE_BITS - 8);
}
uint8_t Sodaq_Dataflash::getPageAddrByte2(uint16_t page)
{
  return 0;
}

// Use a single common instance
Sodaq_Dataflash dflash;
