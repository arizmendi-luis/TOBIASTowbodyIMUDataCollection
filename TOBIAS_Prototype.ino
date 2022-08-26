/* File: TOBIAS_Prototype 
 * Author: Luis Arizmendi, SSI Balloons, TOBIAS subteam 
 * Date: April 30, 2022 - May 23, 2022
 * -----------------------------------------------------------------------------
 * TOBIAS Prototype utalizes an accelerometer and altimeter to take measuremnts,  
 * writing them onto an SD card 
 * ------------------------------------------------------------------------------ 
 * Credit to Adafruit documentation surrounding their LSM6DOX, BMP280, and SD card
 * reading for sevral pieces of code 
 */ 
/* -------------------------------------Altimeter-------------------------------*/   
#include <Wire.h>
#include <Adafruit_BMP280.h>//9600
Adafruit_BMP280 bmp; // I2C  
// Variables/Constants  
double Pressure = 1013.25; //Deafualt 1013.25 changes depending on the day  
/*-------------------------------------Accelermoter-------------------------------*/  
#include <Adafruit_LSM6DSOX.h> //115200
Adafruit_LSM6DSOX sox;   
/*-------------------------------------SD Card Stuff------------------------------*/ 
#include <SD.h> 
//SD card file to write on 
File myFile;  
char textN[12] = {'t','e','s','t','0','_','_','.','t','x','t','\0'};     
/*-------------------------------------More Varibales and Constants----------------*/  
//Modifiable speed/ time interval data is collected for
double ReadSpeed = 10;  
//Decalration of variable to used to hold time from start
unsigned long tiempo; 
/*---------------------------------------------------------------------------------*/ 
void setup() {
  //Altimeter

  Serial.begin(115200);
  Serial.println(F("BMP280 test"));  
  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    while (1) delay(10);
  }


  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */   

  //SD Card    
  Serial.println("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
  // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  //Accelerometer
Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit LSM6DSOX test!");

  if (!sox.begin_I2C()) {
    // if (!sox.begin_SPI(LSM_CS)) {
    // if (!sox.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    // Serial.println("Failed to find LSM6DSOX chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DSOX Found!");

  // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (sox.getAccelRange()) {
  case LSM6DS_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case LSM6DS_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case LSM6DS_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case LSM6DS_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
  switch (sox.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    break; // unsupported range for the DSOX
  }

  // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (sox.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (sox.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  } 
  //More SD card 
  int i = 1;  
  while(SD.exists(textN) == true && i != 998) {   
    //precaution below as a number greater than 999 will cause issues
    if (!SD.exists(textN)) { 
      break;  
    }
    if( i < 10){  
      char iChar[2];
      itoa(i, iChar, 10); 
      textN[4] = iChar[0]; 
    } 
    else if (i >= 10 && i < 100) { 
      char iChar[3];
      itoa(i, iChar, 10); 
      textN[4] = iChar[0]; 
      textN[5] = iChar[1];  
    }  
    else if (i <=100) { 
      char iChar[4];
      itoa(i, iChar, 10); 
      textN[4] = iChar[0]; 
      textN[5] = iChar[1]; 
      textN[6] = iChar[2];  
    } 
    i = i + 1;    
  } 
  Serial.println(textN);  
}

void loop() {    
  myFile = SD.open(textN, FILE_WRITE);  
   
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);  

  tiempo = millis(); 
  
  Serial.print(F("Temperature = ")); 
  myFile.print(F("Temperature = ")); 
  Serial.print(bmp.readTemperature()); 
  myFile.print(bmp.readTemperature());
  Serial.println(" *C"); 
  myFile.println(" *C"); 
  

  Serial.print(F("Temperature = ")); 
  myFile.print(F("Temperature = ")); 
  Serial.print((bmp.readTemperature()) * (1.8) + (32)); 
   myFile.print(bmp.readTemperature() * (1.8) + (32));
  Serial.println(" *F"); 
  myFile.println(" *F");

  Serial.print(F("Pressure = ")); 
  myFile.print(F("Pressure = "));
  Serial.print(bmp.readPressure()); 
  myFile.print(bmp.readPressure());
  Serial.println(" Pa");
  myFile.println(" Pa");

  Serial.print(F("Approx altitude = ")); 
  myFile.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(Pressure)); /* Adjusted to local forecast!; defualt is 1013.25 */ 
  myFile.print(bmp.readAltitude(Pressure));

  Serial.println(" m"); 
  myFile.println(" m");

  Serial.println(); 
  myFile.println();
  
//delay(ReadSpeed);
//Acelerometer 


  Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");

  myFile.print("Accel X: ");
  myFile.print(accel.acceleration.x);
  myFile.print(" \tY: ");
  myFile.print(accel.acceleration.y);
  myFile.print(" \tZ: ");
  myFile.print(accel.acceleration.z);
  myFile.println(" m/s^2 "); 
  
  /* Display the results (rotation is measured in rad/s) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println(); 

  myFile.print("Gyro X: ");
  myFile.print(gyro.gyro.x);
  myFile.print(" \tY: ");
  myFile.print(gyro.gyro.y);
  myFile.print(" \tZ: ");
  myFile.print(gyro.gyro.z);
  myFile.println(" radians/s ");
  myFile.println();

  myFile.println(tiempo); 
  myFile.print(" milliseconds");
  
  myFile.close();
  
  delay(ReadSpeed);
}
