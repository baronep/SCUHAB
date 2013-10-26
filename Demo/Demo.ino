#include <Wire.h> //I2C Library
#include <Adafruit_BMP085.h> //Tempertaure Library
#include <SDHandler.h> //Class to handle SD operations
#include <RadioHandler.h> //Class to handle APRS
#include <ParallaxGPS.h> //Class to recieve data from gps and send to parser object
#include <TinyGPS++.h> //Class to parse and store gps data
#include <SoftwareSerial.h> //Required communication library
#include <SD.h> //Required for SD Operations

///////////////////////////////////  DEFINE PINS  /////////////////////////////////
#define GPSRX 2
#define GPSTX 3
#define RadioTX 6
#define RadioRX 7

#define verbose true
/////////////////////////////////////////////////////////////////////////////////

Adafruit_BMP085 bmp;

SDHandler sd;

TinyGPSPlus tgps; //GPS NMEA String parser. Also stores latest gps info. used by other classes to recieve gps info
RadioHandler rh(RadioTX,RadioRX,tgps); //Recieves tinygps++ object (gps data), makes APRS pack and sends
ParallaxGPS para(GPSRX,GPSTX,tgps); //Recieves GPS serial data and passes to the parser

//Sending Variables
unsigned long lastSend = 0; //stores that last time (in ms) that a packet was sent
unsigned long lastSave = 0; //stores the last time a data point was saved

///////////////////////////////////////  CONFIGURATION / OPTIONS /////////////////////////////////////////////////////

int sendInterval = 15000; //15 sec
int saveInterval = 30000; //30 sec
String digipath = "RELAY,WIDE2-2";
String callsign = "KJ6SRQ";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
  rh.start(callsign,digipath); //set up HAM callsign and APRS digipaths and set pin-I/O modes
  Serial.println(rh.getVersion());
  Serial.println("Begin");
}
  
void loop() {
  
    para.encode(); // recieve info from the GPS unit and send to parser    
    
    if(millis()-lastSend > sendInterval)
    {
      String packet = rh.sendPOSIT(); //generate APRS info from tinygps++ data and send packet
      
      lastSend = millis(); //update lastSend time
      
      if(verbose)
      {
        Serial.println("APRS Packet:");
        Serial.println(packet); 
        Serial.println();
      }
      
    }
    
    if(millis()-lastSave > saveInterval)
    {
      //Open File to write
      sd.openFile("DATA.LOG");
      //Write temperature info we get
      //sd.writeTemperature(bmp.readTemperature(),bmp.readPressure());
      //Write temperature and gps into to SD card
      sd.writeTemperatureAndGPS(bmp.readTemperature(),bmp.readPressure(),tgps);
      //After finish writing, close the file.
      sd.close();
      lastSave = millis(); //update lastSend time
      if(verbose)
      {
        Serial.println("Temperature: ");
        Serial.println(bmp.readTemperature());
        Serial.println("Pressure: ");
        Serial.println(bmp.readPressure());
        Serial.println();
      }
    }
    
    
}
