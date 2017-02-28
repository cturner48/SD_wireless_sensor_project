#include <LowPower.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SparkFunLSM9DS1.h>


// VARIABLE DEFINITIONS
//
const int chipSelect = 10;
LSM9DS1 motion;
#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW
#define DECLINATION -5.70 // Declination (degrees) in Atlanta, Ga.


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

  motion.settings.device.commInterface = IMU_MODE_I2C;
  motion.settings.device.mAddress = LSM9DS1_M;
  motion.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // motion.begin(), which verifies communication with the IMU
  // and turns it on.
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
}


// MAIN LOOP
//
void loop(){
  /////////////////////////// SLEEP STATE //////////////////////////////////////////////
  // Sleep in Low Power Mode Unitl Wake Up RX
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  /////////////////////////// COLLECTION STATE /////////////////////////////////////////
  //
  //Accelerometer Reading
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
    


    // Collect Data and Save On SD:
    // Create Data String of all data to be saved/printed. 
    int Ax = (motion.calcAccel(motion.ax),2);
    int Ay = (motion.calcAccel(motion.ay), 2);
    int Az = (motion.calcAccel(motion.az), 2);
    String dataString = "";
    dataString += "Ax: ";
    dataString += Ax;
    dataString += "  Ay: ";
    dataString += Ay;
    dataString += "  Az: ";
    dataString += Az;
    
    Serial.println(dataString);
    Serial.println();
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
