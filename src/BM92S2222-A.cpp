/*****************************************************************
File:        BM92S2222-A.cpp
Author:      BESTMODULES
Description: BM92S2222-A.cpp is the library for controlling the BM92S2222_A-A Fingerprint Sensor Module
History:
V1.0.1   -- initial version；2023-02-13；Arduino IDE : v1.8.16
******************************************************************/
#include "BM92S2222-A.h"

/* Global functions ----------------------------------------------------------------------------------------*/
/**********************************************************
Description: Constructor
Parameters: keyout:interrupt pin      
            *theSerial : Wire object if your board has more than one UART interface
Return:
Others:
**********************************************************/
BM92S2222_A::BM92S2222_A(uint8_t keyout,HardwareSerial* theSerial)
{
  _softSerial = NULL;
  _intPin = keyout;
  _serial = theSerial;
}

/**********************************************************
Description: Constructor
Parameters:  keyout:interrupt pin 
             rxPin : Receiver pin of the UART
             txPin : Send signal pin of UART
Return:
Others:
**********************************************************/
BM92S2222_A::BM92S2222_A(uint8_t keyout,uint8_t rxPin, uint8_t txPin)
{
  _serial = NULL;
  _rxPin = rxPin;
  _txPin = txPin;
  _intPin = keyout;
  _softSerial = new SoftwareSerial(_rxPin, _txPin);
}

/**********************************************************
Description: initialize the module
Parameters:  baud : baud rate 9600, 19200, 38400, 57600(default), 115200
Return:      -
Others:
**********************************************************/
void BM92S2222_A::begin(unsigned long baud)
{
  if (_softSerial != NULL)
  {
   _softSerial->begin(baud);
  }
   else
   {
    _serial->begin(baud);
   }   
  pinMode(_intPin,INPUT);
}

/**********************************************************
Description: Get the number of enrolled fingerprints
Parameters:
Return:      responseByte[4] : the number of enrolled fingerprints
Others:
**********************************************************/
int16_t BM92S2222_A::getEnrollCount()
{
 uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x20,0x00,0x20,0x01};
 uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);

  if (responseByte[8] == 0x30)
  {
    return responseByte[4];
  }
  else 
  {
    return 0;
  }
}

/**********************************************************
Description:  Get INT line status
Parameters:
Return:       0:low 1:high
Others:
**********************************************************/
uint8_t BM92S2222_A::getKeyout()
{
 return digitalRead(_intPin);
}

/**********************************************************
Description: Check to see if the ID number is enrolled or not
Parameters:  id : 0-99
Return:      true : the ID number is enrolled.
             false : the ID number is not enrolled.
Others:
**********************************************************/
bool BM92S2222_A::checkEnrolled(uint8_t id)
{
  uint8_t i;
  uint16_t sum = 0;
  uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,id,0,0,0,0x21,0};
  uint8_t responseByte[255];
  for (i=0; i<10; i++)  
  {
    sum += commandByte[i];
  }  
  commandByte[10] = (byte)sum&0xFF;
  commandByte[11] = (byte)(sum>>8)&0xFF;

  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);

  if (responseByte[8] == 0x30)
  {
    return true;
  }
  else 
  {
    return false;
  }
}

/**********************************************************
Description: Check to see if a finger is pressed on the fingerprint sensor
Parameters:
Return:      true : a finger is pressed on the fingerprint sensor
             false : no finger is pressed on the fingerprint sensor
Others:
**********************************************************/
bool BM92S2222_A::isPressFinger()
{
 uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x026,0x00,0x26,0x01};
 uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);

  if (responseByte[8] == 0x30)
  {
    return true;
  }
  else 
  {
    return false;
  }
}

/**********************************************************
Description: Get the fingerprint data and store in the memory of the module
             The data is then used for enrollment, verification, identification
Parameters:
Return:      true : successful
             false : finger is not pressed
Others:
**********************************************************/
bool BM92S2222_A::captureFinger()
{
 uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x060,0x00,0x60,0x01};
  uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(100);
  readBytes(responseByte, 12);
  delay(10);

  if (responseByte[8] == 0x30) 
  {
    return true;
  }
  else 
  {
    return false;
  }
}
/**********************************************************
Description: Change baud rate
Parameters:  baud : baud rate 9600, 19200, 38400, 57600, 115200
Return:      -
Others:      9600   baud command bytes = {0x55,0xAA,0x01,0x00,0x80,0x25,0x00,0x00,0x04,0x00,0xA9,0x01};
             19200  baud command bytes = {0x55,0xAA,0x01,0x00,0x00,0x4B,0x00,0x00,0x04,0x00,0x4F,0x01};
             38400  baud command bytes = {0x55,0xAA,0x01,0x00,0x00,0x96,0x00,0x00,0x04,0x00,0x9A,0x01};
             57600  baud command bytes = {0x55,0xAA,0x01,0x00,0x00,0xE1,0x00,0x00,0x04,0x00,0xE5,0x01};
             115200 baud command bytes = {0x55,0xAA,0x01,0x00,0x00,0xC2,0x01,0x00,0x04,0x00,0xC7,0x01};
**********************************************************/
void BM92S2222_A::changeBaud(unsigned long baud)
{
  uint8_t i;
  uint16_t sum = 0;
  if ((baud == 9600) || (baud == 19200) || (baud == 38400) || (baud == 57600) || (baud == 115200))
  {
    uint8_t commandByte[12]={0x55,0xAA,0x01,0};
    commandByte[4] = (baud & 0x000000ff);
    commandByte[5] = (baud & 0x0000ff00)>>8;
    commandByte[6] = (baud & 0x00ff0000)>>16;
    commandByte[7] = (baud & 0xff000000)>>24;
    commandByte[8] = 0x04;
    commandByte[9] = 0;
    for (i=0; i<10; i++)
    {
      sum += commandByte[i];
    }  
    commandByte[10] = (byte)sum&0xFF;
    commandByte[11] = (byte)(sum>>8)&0xFF;

    writeByres(commandByte,12);
    delay(10);

    if (_softSerial != NULL)
    {
        _softSerial->begin(baud);
    }
    else
    {
        _serial->begin(baud);
    }

  }
}
/**********************************************************
Description: Start the enrollment process
Parameters:  id : 0-99
Return:      0 : ACK
             1 : Database is full
             2 : Invalid Position
             3 : Position(ID) is already used
             4 : Failed 
Others:
**********************************************************/
int16_t BM92S2222_A::enrollStart(uint8_t id)
{
  uint8_t i;
  uint16_t sum = 0;
  uint8_t responseByte[255];
  uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,id,0,0,0,0x022,0x00};
 for (i=0; i<10; i++)  
 {
  sum += commandByte[i];
  }   
  commandByte[10] = (byte)sum&0xFF;
  commandByte[11] = (byte)(sum>>8)&0xFF;

  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12 );
  delay(10);

  if (responseByte[8] == ACK)
  {
    return 0;
  }
  if (responseByte[8] == NACK)
  {
    if (responseByte[4] == 0x09) 
    {
      return 1;
    }
    if (responseByte[4] == 0x03) 
    {
      return 2;
    }
    if (responseByte[4] == 0x05) 
    {
      return 3;
    }
  }
  return 4;
}

/**********************************************************
Description: Get the first scan of an enrollment
Parameters:
Return:      0 : ACK
             1 : Database is full
             2 : Invalid Position
             3 : Position(ID) is already used
             4 : Failed 
Others:
**********************************************************/
int16_t BM92S2222_A::enroll1()
{
 uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x023,0x00,0x23,0x01};
 uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);
  
  if (responseByte[8] == ACK)
  {
    return 0;
  }
  if (responseByte[8] == NACK)
  {
    if (responseByte[4] == 0x0D) 
    {
      return 1;
    }
    if (responseByte[4] == 0x0C) 
    {
      return 2;
    }
    if (responseByte[4] == 0x05) 
    {
      return 3;
    }
  }
  return 4;
}

/**********************************************************
Description: Get the 2nd scan of an enrollment
Parameters:
Return:      0 : ACK
             1 : Database is full
             2 : Invalid Position
             3 : Position(ID) is already used
             4 : Failed 
Others:
**********************************************************/
int16_t BM92S2222_A::enroll2()
{
 uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x024,0x00,0x24,0x01};
uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);

  if (responseByte[8] == ACK)
  {
    return 0;
  }
  if (responseByte[8] == NACK)
  {
    if (responseByte[4] == 0x0D)
    {
      return 1;
    }
    if (responseByte[4] == 0x0C) 
    {
      return 2;
    }
    if (responseByte[4] == 0x05) 
    {
      return 3;
    }
  }
  return 4;
}

/**********************************************************
Description: Get the 3rd scan of an enrollment
Parameters:
Return:      0 : ACK
             1 : Database is full
             2 : Invalid Position
             3 : Position(ID) is already used
             4 : Failed 
Others:
**********************************************************/
int16_t BM92S2222_A::enroll3()
{
 uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x025,0x00,0x25,0x01};
 uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);

  if (responseByte[8] == ACK)
  {
    return 0;
  }
  if (responseByte[8] == NACK)
  {
    if (responseByte[4] == 0x0D) 
    {
      return 1;
    }
    if (responseByte[4] == 0x0C) 
    {
      return 2;
    }
    if (responseByte[4] == 0x05) 
    {
      return 3;
    }
  }
  return 4;
}

/**********************************************************
Description: Checks the currently pressed finger against a specific ID
Parameters:  id : 0-99 (the ID number to be checked)
Return:      0 : Verified OK (the correct finger)
             1 : Invalid Position
             2 : ID is not used
             3 : Verified FALSE (not the correct finger)
             4 : Failed (NACK_COMM_ERR)
Others:
**********************************************************/
int16_t BM92S2222_A::verifyID(uint8_t id)
{
  uint8_t i;
  uint16_t sum = 0;
  uint8_t responseByte[255];
  uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,id,0,0,0,0x050,0x00};
  for (i=0; i<10; i++)  
  {
    sum += commandByte[i];
  }  
  commandByte[10] = (byte)sum&0xFF;
  commandByte[11] = (byte)(sum>>8)&0xFF;
 
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12 );
  delay(10);

  if (responseByte[8] == ACK)
  {
    return 0;
  }
  if (responseByte[8] == NACK)
  {
    if (responseByte[4] == 0x03) 
    {
      return 1;
    }
    if (responseByte[4] == 0x04) 
    {
      return 2;
    }
    if (responseByte[4] == 0x07) 
    {
      return 3;
    }
  }
  return 4;
}

/**********************************************************
Description: Checks the currently pressed finger against all enrolled fingerprints
Parameters:
Return:      responseByte[4] :  Verified against the specified ID 0-99 (found, and here is the ID number)
Others:
**********************************************************/
int16_t BM92S2222_A::identify()
{
uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x051,0x00,0x51,0x01};
uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(1000);
  readBytes(responseByte, 12 );
  delay(10);
  
  if (responseByte[8] == ACK)
  {
    return responseByte[4];
  }
  if (responseByte[8] == NACK)
  {
    if (responseByte[4] == 0x0A) 
    {
      return 0;
    }
    if (responseByte[4] == 0x08) 
    {
      return 0;
    }
  }
  return 0;
}

/**********************************************************
Description: Deletes the specified ID from the database
Parameters:       id : 0-99 (the ID number to be deleted)
Return:      true : successful
             false : invalid position
Others:
**********************************************************/
bool BM92S2222_A::deleteID(uint8_t id)
{
  uint8_t i;
  uint16_t sum = 0;
  uint8_t responseByte[255];
  uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,id,0,0,0,0x040,0x00};
  for (i=0; i<10; i++) 
  {
    sum += commandByte[i];
  }  
  commandByte[10] = (byte)sum&0xFF;
  commandByte[11] = (byte)(sum>>8)&0xFF;

   writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12);
  delay(10);
  
  if (responseByte[8] == 0x30)
  {
    return true;
  }
  else 
  {
    return false;
  }
}

/**********************************************************
Description: Deletes all IDs from the database
Parameters:
Return:      true : successful
             false : database is empty
Others:
**********************************************************/
bool BM92S2222_A::deleteAll()
{
  uint8_t commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x041,0x00,0x41,0x01};
  uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12 );
  delay(10);

  if (responseByte[8] == 0x30) 
  {
    return true;
  }
  else 
  {
    return false;
  }
}

/* Private functions ---------------------------------------------------------------------------------------*/
/**********************************************************
Description: Get the response date from the BM92S2222_A-A module
Parameters:  response[] : the pointer of the array for storing response date
             length : the response date length for READ commands, length=6. for WRITE commands, length=3.
             timeout:Time out for storing input
Return:       1: Timeout error
              0:read success
Others:
**********************************************************/
uint8_t BM92S2222_A::readBytes(uint8_t response[] ,uint8_t length,uint16_t timeout)
{
  uint8_t i ;
  uint8_t delayCnt = 0;
  if (_softSerial != NULL)
  {
    for (i=0; i < length; i++)
    {
        delayCnt = 0;
        while (_softSerial->available() == 0)
        {
          if (delayCnt > timeout)
          {
            Serial.println(F("Timeout error ")); // Timeout error
            return 1;
          }
          delay(1);
          delayCnt++;
        }
        response[i] = (byte) _softSerial->read();
    }
  }
  else
  {
    for (i=0; i < length; i++)
    {
      while (_serial->available() == 0)
      {
          delay(1);
          delayCnt++;
         if (delayCnt > timeout)
          {
            Serial.println(F("Timeout error ")); // Timeout error
            return 1;
          }
      }
      *(response+i) = (byte) _serial->read();
    }
  }
  return 0;
}
/**********************************************************
Description: writeByres
Parameters:  wbuf : Data to be written
             wlen : Length of data to be written
Return:
Others:
**********************************************************/
void BM92S2222_A::writeByres(uint8_t wbuf[] ,uint8_t wlen)
{
   if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  else
  {
    while (_serial->available() > 0)
    {
      _serial->read();
    }
    _serial->write(wbuf, wlen);
  }
}

/**********************************************************
Description: Shut down the module
Parameters:
Return:      0x30 : ACK
             0x1006 : Failed (NACK_COMM_ERR)
Others:
**********************************************************/
int16_t BM92S2222_A::close()
{
  uint8_t  commandByte[12]={0x55,0xAA,0x01,0x00,0,0,0,0,0x02,0x00,0x02,0x01};
  uint8_t responseByte[255];
  writeByres(commandByte,12);
  delay(10);
  readBytes(responseByte, 12 );
  delay(10);  

  if (responseByte[8] == 0x30)
  {
    return ACK;
  }
  else 
  {
    return NACK_COMM_ERR;
  }
}
