

#include <Wire.h>
//#include "RTClib.h"
//#include <LowPower.h>

void setup() {
  pinMode(8, OUTPUT);
  // Initialize serial output for development purposes.
    Serial.begin(9600);
    while (!Serial);
    delay(200);
    Serial.println("-- DISPLAY ONLINE --");
    
 
}

// MAIN LOOP
//
void loop(){
    digitalWrite(8,HIGH);
    delay(3000);
    digitalWrite(8,LOW);
    //LowPower.powerDown(SLEEP_2S,ADC_OFF, BOD_OFF);
    
}
