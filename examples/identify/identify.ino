/*****************************************************************
File:         Identify.ino
Description:  1.SoftwareSerial interface (BAUDRATE 9600)is used to communicate with BM92S2222_A.
              2.hardware Serial (BAUDRATE 9600) is used to communicate with Serial port monitor.
              3. Fingerprint Identification
connection method： 
******************************************************************/
#include "BM92S2222-A.h"

BM92S2222_A fps(8,4, 5);        //keyout:8, rxPin:4, txPin:5, Please comment this line of code if don't use SW Serial
//BM92S2222_A fps(22,&Serial1);  // Please uncomment this line of code if you use HW Serial1 on BMduino
//BM92S2222_A fps(2,&Serial2);  // Please uncomment this line of code if you use HW Serial2 on BMduino
//BM92S2222_A fps(2,&Serial3);  // Please uncomment this line of code if you use HW Serial3 on BMduino
//BM92S2222_A fps(2,&Serial4);  // Please uncomment this line of code if you use HW Serial4 on BMduino

void setup()
{
  Serial.begin(9600);
  delay(100); // wait for the module stable
  fps.begin();
}

void loop()
{
  if(fps.getKeyout() == 0)//Finger press detected
  { 
    Serial.println(F("Please press finger withiin 2 milliseconds"));
    delay(200);      // wait 2 milliseconds for user to press finger
    if (fps.isPressFinger())
    {
        if(fps.captureFinger())
        {
          int id = fps.identify();
          if (id < 100)    // BM92S2222_A-A has 100 fingerprints capacity
          {
            Serial.print(F("Verified ID:"));
            Serial.println(id);
            Serial.println(F("END Fingerprint Identification demo"));
          }
        }
        else 
        {
          Serial.println("error captureFinger");
        }
      }
      else
      { // finger does not press on the FPS
        Serial.println(F("Please press finger"));
      }
      delay(1000);
  }
}
