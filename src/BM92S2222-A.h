/*****************************************************************
File:             BM92S2222-A.h
Author:           BESTMODULES
Description:      Define classes and required variables
History:
V1.0.1   -- initial version；2023-02-13；Arduino IDE : v1.8.16
******************************************************************/
#ifndef BM92S2222_A_H
#define BM92S2222_A_H

#include "Arduino.h"
#include "SoftwareSerial.h"

#define ACK                   0x30
#define NACK                  0x31
#define NACK_INVALID_POS      0x1003
#define NACK_IS_ALREADY_USED  0x1005
#define NACK_COMM_ERR         0x1006
#define NACK_VERIFY_FAILED    0x1007
#define NACK_IDENTIFY_FAILED  0x1008
#define NACK_DB_IS_FULL       0x1009
#define NACK_DB_IS_EMPTY      0x100A
#define NACK_BAD_FINGER       0x100C
#define NACK_ENROLL_FAILED    0x100D

class BM92S2222_A
{
  public:
    
    BM92S2222_A(uint8_t keyout,HardwareSerial *theSerial = &Serial);
    BM92S2222_A(uint8_t keyout,uint8_t rxPin, uint8_t txPin);
    void begin(unsigned long baud = 57600);
    int16_t getEnrollCount();
    uint8_t getKeyout();
    bool checkEnrolled(uint8_t id);
    bool isPressFinger();  
    bool captureFinger(); 
    void changeBaud(unsigned long baud);     
    int16_t enrollStart(uint8_t id);
    int16_t enroll1();
    int16_t enroll2();
    int16_t enroll3();
    int16_t verifyID(uint8_t id);
    int16_t identify();
    bool deleteID(uint8_t id);
    bool deleteAll();

    
  private:
    uint8_t _rxPin;
    uint8_t _txPin;
    uint8_t _intPin;
    HardwareSerial *_serial = NULL;
    SoftwareSerial *_softSerial = NULL;
    void writeByres(uint8_t wbuf[] ,uint8_t wlen);
    uint8_t readBytes(uint8_t response[], uint8_t length,uint16_t timeout = 270);
    void printSend();
    int16_t close();
};
#endif
