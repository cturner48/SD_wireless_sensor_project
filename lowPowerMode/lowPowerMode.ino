
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


// VARIABLE DEFINITIONS
//
const int chipSelect = 10;
#include <SparkFunLSM9DS1.h>
LSM9DS1 motion;
#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW
#define PRINT_CALCULATED


// SETUP LOOP
//
void setup() {
  
    //pinMode(2, OUTPUT); // Previously used for indicator LED light.
    Wire.begin();        // join i2c bus (address optional for master)
    Serial.begin(115200);  // Initialize serial output for development purposes.
    while (!Serial);
    delay(200);
    Serial.println("-- DISPLAY ONLINE --");
  
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

    /////////////////////////// COLLECTION STATE /////////////////////////////////////////
    //
    //Accelerometer Reading
    Serial.print("A,");


    // Collect Data and Save On SD: 
    String dataString = "";
    dataString += "A: ";
    
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
