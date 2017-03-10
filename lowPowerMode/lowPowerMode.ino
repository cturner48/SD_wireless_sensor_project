#include <LowPower.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SparkFunLSM9DS1.h>
#include <Adafruit_GPS.h>
//#include <SoftwareSerial.h>

/////////////////// VARIABLE DEFINITIONS ///////////////////
//
const int chipSelect = 10;
boolean accelChange = false;
float accelComp;
int sleepCount;
LSM9DS1 motion;
#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW
#define DECLINATION -5.70 // Declination (degrees) in Atlanta, Ga.
#define GPSECHO  false

  

  

/////////////////// SETUP LOOP ///////////////////////
//
void setup() {
  pinMode(10, OUTPUT); //SD Card Chip Select
  pinMode(9, OUTPUT);  //SD Card power toggle
  pinMode(4, OUTPUT);  //GPS/RTC power toggle
  pinMode(14, OUTPUT);  //ACCEL power toggle
  digitalWrite(9, HIGH); //SD On
  //digitalWrite(4, HIGH); //GPS On
  digitalWrite(14, HIGH); //Accel On
  
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // Initialize serial output for development purposes.
  while (!Serial){
     delay(200); 
  }
  Serial.println("-- DISPLAY ONLINE --");
  Serial.print("Initializing SD card...");
  
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  motion.settings.device.commInterface = IMU_MODE_I2C;
  motion.settings.device.mAddress = LSM9DS1_M;
  motion.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // motion.begin(), which verifies communication with the IMU
  // and turns it on.
  //Serial.println("Initializing IMU.");
  if (!motion.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
  //Serial.println("IMU Initialized.");
  accelAvg();
}

//////////////// FUNCTION SETUP //////////////////////
//
void accelAvg() {
  sleepCount = 5;
  float x = 0;
  float y = 0;
  float z = 0;
  while( sleepCount > 0) {
  if ( !motion.accelAvailable() )
    {
      // To read from the accelerometer, first call the
      // readAccel() function. When it exits, it'll update the
      // ax, ay, and az variables with the most current data.
      delay(200);
      Serial.println("Waiting for accelerometer reading.");
      Serial.println();
    } else {
      motion.readAccel();
    }
    x += motion.calcAccel(motion.ax);
    y += motion.calcAccel(motion.ay);
    z += motion.calcAccel(motion.az);
    sleepCount = sleepCount - 1;
    /*Serial.print("Initializing accel value.");
    Serial.print((x+y+z), 2);
    Serial.println("");
    Serial.println(sleepCount);*/
    delay(1000);
  }
  accelComp = (x + y + z)/5; 
}


void checkAccel(char* axbuff, char* aybuff, char* azbuff) {
 //Accelerometer Reading
    //digitalWrite(14, HIGH); //Accel On
    //delay(400);
    if ( !motion.accelAvailable() )
    {
      // To read from the accelerometer, first call the
      // readAccel() function. When it exits, it'll update the
      // ax, ay, and az variables with the most current data.
      Serial.println("Waiting for accelerometer reading.");
      Serial.println();
      delay(100);
    } else {
      motion.readAccel();
    }
    
    dtostrf(motion.calcAccel(motion.ax),5,2,axbuff);
    dtostrf(motion.calcAccel(motion.az),5,2,azbuff);
    dtostrf(motion.calcAccel(motion.ay),5,2,aybuff);
    
    if ( ((motion.calcAccel(motion.ax) + motion.calcAccel(motion.ay) + motion.calcAccel(motion.az)) < (accelComp - .2)) || ((motion.calcAccel(motion.ax) + motion.calcAccel(motion.ay) + motion.calcAccel(motion.az)) > (accelComp + .2)) ){
      accelChange = true;
      Serial.println("Motion detected, exiting sleep loop.");
    }
    //digitalWrite(14, LOW); //Accel Off
  
}

//////////////////////// MAIN LOOP/////////////////////////
//
void loop(){
  String dataString;
  /////////////////////////// SLEEP STATE //////////////////////////////////////////////
  // Sleep in Low Power Mode Unitl Wake Up RX
  //digitalWrite(9, LOW); //SD Off
  //digitalWrite(4, LOW); //GPS Off
  //digitalWrite(14, LOW); //Accel Off
  sleepCount = 4;
  char axbuff[8];
  char azbuff[8];
  char aybuff[8];
  accelChange = false;
  //Serial.println("Devices off.");
  //Serial.println();
  while( (sleepCount > 0) && (!accelChange)) {
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    Serial.print(accelComp);
    Serial.print("   ");
    Serial.println();
    checkAccel(axbuff, aybuff, azbuff);
    //Serial.println(accelChange);
    //Serial.println();
    sleepCount -= 1;
  }

  /////////////////////////// COLLECTION STATE /////////////////////////////////////////
  //
  // Collect Data and Save On SD:
  //digitalWrite(9, HIGH); //SD On
  //digitalWrite(4, HIGH); //GPS On
  //digitalWrite(14, HIGH); //Accel On
  dataString = "";
  dataString += "Test line.";
  dataString += "   Ax: ";
  //dataString += axbuff;
  
  File dataFile = SD.open("test2.txt", FILE_WRITE);
  // if the file is available, write to it:
  
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println("Write Successful.");
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
    
    
    
    
  
  
  //////////////////////////////////// TRANSMIT STATE ///////////////////////////////////////////////// 
}


