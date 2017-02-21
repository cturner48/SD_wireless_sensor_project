

//Acceleromet code by mojavaton (www.mojavaton.org)
#include <Wire.h>
//#include "RTClib.h"
#include <LowPower.h>
#include <SPI.h>
#include <SD.h>



// VARIABLE DEFINITIONS
//
/*RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};*/
const int chipSelect = 10;
#define ACCELADDR 0x53

union XYZBuffer {
  struct {
    short x,y,z;
  } value;
  byte buff[6];
};

void changeEndian(union XYZBuffer *xyz) {
  for (int i=0;i<6;i+=2) {
    byte t=xyz->buff[i];
    xyz->buff[i]=xyz->buff[i+1];
    xyz->buff[i+1]=t;
  }
}

// Generically useful reading into a union type
void readXYZ(int device,union XYZBuffer *xyz) {    
  Wire.requestFrom(device, 6);      
  long start=millis();
  while (!Wire.available() && (millis()-start)<100);
  if (millis()-start<100) {
    for (int i=0;i<6;i++)
      xyz->buff[i]=Wire.read();
  }
}

void setupAccel(int device) {
  // Check ID to see if we are communicating
  Wire.beginTransmission(device);
  Wire.write(0x00); // One Reading
  Wire.endTransmission();
  Wire.requestFrom(device,1);
  while (!Wire.available());  
  byte ch=Wire.read();
  Serial.print("Accel id is 0x");
  Serial.println(ch,HEX);
  // Should output E5
 
  // https://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
  // Page 16
  Wire.beginTransmission(device);
  Wire.write(0x2d);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(device);
  Wire.write(0x38);
  Wire.write(0x84);
  Wire.endTransmission();
 
}
void readAccel(int device,union XYZBuffer *xyz) {
  Wire.beginTransmission(device);
  Wire.write(0x32); // One Reading
  Wire.endTransmission();
  readXYZ(device,xyz);
}

void pad(int width,int number) {
  int n=abs(number);
  int w=width;
  if (number<0) w--;
  while (n>0) {
    w--;
    n/=10;
  }
  if (number==0) w--;
  for (int i=0;i<w;i++) Serial.print(' ');
}

void output(union XYZBuffer xyz) {
//  pad(6,xyz.value.x);
  Serial.print(xyz.value.x);
  Serial.print(',');
//  pad(6,xyz.value.y);
  Serial.print(xyz.value.y);
  Serial.print(',');
//  pad(6,xyz.value.z);
  Serial.print(xyz.value.z);
}



// SETUP LOOP
//
void setup() {
  
    //pinMode(2, OUTPUT); // Previously used for indicator LED light.
    Wire.begin();        // join i2c bus (address optional for master)
    setupAccel(ACCELADDR);
    Serial.begin(9600);  // Initialize serial output for development purposes.
    while (!Serial);
    delay(200);
    Serial.println("-- DISPLAY ONLINE --");
    
  /*(if (! rtc.begin()) {
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
  }*/
  
Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
Serial.println("card initialized.");
  
}




// MAIN LOOP
//
void loop(){


    /////////////////////////// SLEEP STATE //////////////////////////////////////////////
    // Sleep in Low Power Mode Unitl Wake Up RX
    //LowPower.powerDown(SLEEP_8S,ADC_OFF, BOD_OFF);

    /////////////////////////// COLLECTION STATE /////////////////////////////////////////
    // Collect and Print RTC Time
    //digitalWrite(2,HIGH);
    /*DateTime now = rtc.now();
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
    Serial.println();*/
    //digitalWrite(2,LOW);
  
    //Accelerometer Reading
    union XYZBuffer accel;
    readAccel(ACCELADDR,&accel);
    Serial.print("A,");
    output(accel);


    // Collect Data and Save On SD: 
    // make a string for assembling the data to log:
    String dataString = "";
    //dataString += (now.year());
    dataString += "A: ";
    //dataString += (accel);
    
     // read three sensors and append to the string:
  /*for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }*/

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


  //////////////////////////////////// TRANSMIT STATE /////////////////////////////////////////////////
  
}
