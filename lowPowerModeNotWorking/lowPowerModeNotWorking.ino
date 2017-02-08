
#include <Wire.h>
#include "RTClib.h"
#include <LowPower.h>



// VARIABLE DEFINITIONS
//
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// SETUP LOOP
//
void setup() {
  pinMode(8, OUTPUT);
  // Initialize serial output for development purposes.
    Serial.begin(9600);
    while (!Serial);
    delay(200);
    Serial.println("-- DISPLAY ONLINE --");
    
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  
}

// MAIN LOOP
//
void loop(){
    //digitalWrite(8,HIGH);
    delay(3000);
    //digitalWrite(8,LOW);
    //LowPower.powerDown(SLEEP_2S,ADC_OFF, BOD_OFF);
    // Serial output for development feedback.
      //Serial.write("Initiating daily reset. Waiting for ZedBoard to power off.");
      //Serial.println();
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  
}


