#include <Wire.h> //I2C Library
#include <Adafruit_BMP085.h> //Tempertaure Library
#include <SDHandler.h> //Class to handle SD operations
#include <RadioHandler.h> //Class to handle APRS
#include <ParallaxGPS.h> //Class to recieve data from gps and send to parser object
#include <TinyGPS++.h> //Class to parse and store gps data
#include <SoftwareSerial.h> //Required communication library
#include <SD.h> //Required for SD Operations

Adafruit_BMP085 bmp;

SDHandler sd;

TinyGPSPlus tgps; //GPS NMEA String parser. Also stores latest gps info. used by other classes to recieve gps info
RadioHandler rh(6,7,tgps); //Recieves tinygps++ object (gps data), makes APRS pack and sends
ParallaxGPS para(2,3,tgps); //Recieves GPS serial data and passes to the parser

//Sending Variables
long int lastSend = 0; //stores that last time (in ms) that a packet was sent

void setup() {
  Serial.begin(9600);
  while(!Serial)
  {
    ;
  }
  //Init SD Handler, SparkFun SD shield use pin 8 for SS.
  sd.start(8);
  
  //Init BMP085
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
  
  para.start(); //start the software serial for gps and set pin-I/O modes
  rh.start("KJ6SRQ","RELAY,WIDE2-1"); //set up HAM callsign and APRS digipaths and set pin-I/O modes
  //Serial.println(rh.checkVersion());
  Serial.println("Begin");
}
  
void loop() {
    para.encode(); // recieve info from the GPS unit and send to parser    
    if(millis()-lastSend > 15000)
    {
      //Open File to write
      sd.openFile("DATA.LOG");
      //Write temperature info we get
      //sd.writeTemperature(bmp.readTemperature(),bmp.readPressure());
      //Write temperature and gps into to SD card
      sd.writeTemperatureAndGPS(bmp.readTemperature(),bmp.readPressure(),tgps);
      //After finish writing, close the file.
      sd.close();
    
      String packet = rh.sendPOSIT(); //generate APRS info from tinygps++ data and send packet
      Serial.println(packet);
      lastSend = millis(); //update lastSend time
    }
}
