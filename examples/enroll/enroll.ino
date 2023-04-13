/*****************************************************************
File:         enroll.ino
Description:   Fingerprint Enrollment
connection method： 
******************************************************************/
#include "BM92S2222-A.h"

BM92S2222_A fps(8,4,5);        // keyout:8,rxPin:4, txPin:5, Please comment this line of code if don't use SW Serial
//BM92S2222_A fps(22,&Serial1);  // Please uncomment this line of code if you use HW Serial1 on BMduino
//BM92S2222_A fps(2,&Serial2);  // Please uncomment this line of code if you use HW Serial2 on BMduino
//BM92S2222_A fps(2,&Serial3);  // Please uncomment this line of code if you use HW Serial3 on BMduino
//BM92S2222_A fps(2,&Serial4);  // Please uncomment this line of code if you use HW Serial4 on BMduino 
uint16_t id = 0;
uint8_t count = 1;
uint8_t keyout_flag = 0;
void setup() {
  Serial.begin(9600);
  delay(100); // wait for the module stable
  fps.begin();
}
void loop() {
  if((fps.getKeyout() == 0) && (keyout_flag==0))//Finger press detected
  { 
    uint8_t iret = 5;
    keyout_flag = 1;
    delay(300);
    if(fps.isPressFinger()==true)
    {
      if(count == 1)
      {
        while (fps.checkEnrolled(id)==true)
        {        
          id++;
        } 
        if(fps.enrollStart(id)==0)
        {

          Serial.print(F("Press finger to Enroll # "));
          Serial.println(id);
          Serial.println(F("Press finger again: 1nd time "));
          if (fps.captureFinger() != false)
          {  
            Serial.println("captureFinger success");
            iret= fps.enroll1();
            switch(iret)
            {
              case 0 : 
                  {
                    Serial.println("enroll1 success");
                    break;
                  }                 
              case 1 :
                  {
                    Serial.println("The database is full");
                    break;
                  }
              case 2:
                  {
                    Serial.println("inactive position");
                    break;
                  }
              case 3:
                  {
                    Serial.println("ID already used");
                    break;
                  }
              default:
                  {
                    Serial.println("enroll1 fail");
                    break;
                  }
             }
          }
         else
          {

            Serial.println(F("Enroll Failed with error captureFinger"));   // enroll1() returns error code
           
          }
        }
        else
        {

          Serial.println(F("Enroll Failed with error enrollStart"));   // enroll1() returns error code
         
        }
      }
      if(count == 2)
      {
        Serial.println(F("Press same finger again: 2nd time "));
       if (fps.captureFinger() != false)
          {  
            Serial.println("captureFinger success");
            iret= fps.enroll2();
            switch(iret)
            {
              case 0 : 
                  {
                    Serial.println("enroll2 success");
                    break;
                  }                 
              case 1 :
                  {
                    Serial.println("The database is full");
                    break;
                  }
              case 2:
                  {
                    Serial.println("inactive position");
                    break;
                  }
              case 3:
                  {
                    Serial.println("ID already used");
                    break;
                  }
              default:
                  {
                    Serial.println("enroll2 fail");
                    break;
                  }
             }
          }
           else
            {
              Serial.println(F("Enroll Failed with error captureFinger"));   // enroll2() returns error code   
            }
      }
     if(count == 3)
      {
        Serial.println(F("Press same finger again: 3nd time "));
        if (fps.captureFinger() != false)
          {  
            Serial.println("captureFinger success");
            
            iret= fps.enroll3();
            switch(iret)
            {
              case 0 : 
                  {
                    Serial.println("enroll3 success");
                    Serial.println("enroll END");
                    break;
                  }                 
              case 1 :
                  {
                    Serial.println("The database is full");
                    break;
                  }
              case 2:
                  {
                    Serial.println("inactive position");
                    break;
                  }
              case 3:
                  {
                    Serial.println("ID already used");
                    break;
                  }
              default:
                  {
                    Serial.println("enroll3 fail");
                    break;
                  }
             }
          }
           else
            {
              Serial.println(F("Enrol3 Failed with error captureFinger"));   // enroll3() returns error code
            }
      }
    }
    else
    {
       Serial.println(F("No fingerprint detected"));  
    }
    if(iret == 0)
    {
      count++;
      if( count == 4)
      {
        count = 1;
      }      
    }
    else if(iret == 3)
    {
     count = 1; 
    }
  }
  else if (fps.getKeyout() ==1) 
  {
    keyout_flag = 0;
  }
}
