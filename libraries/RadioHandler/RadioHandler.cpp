#include "Arduino.h"
#include "RadioHandler.h"
#include "String.h"
#include <SoftwareSerial.h>

RadioHandler::RadioHandler(int rx, int tx,TinyGPSPlus& tgps):
	_s(rx,tx), _tgps(tgps)
{
	pinMode(rx,INPUT);  //INPUT FOR SOFTWARE SERIAL PORT 10
	pinMode(tx,OUTPUT);
}

void RadioHandler::start(String callsign,String digipath)
{
	_s.begin(4800);	//start arduino serial port; 
	delay(3);								//delay 3ms; not sure if necessary
	_s.print("M"+callsign + "\r\n");	//set callsign
	delay(10);								//delay 10msl again not sure if necessary
	_s.print("P"+digipath + "\r\n");	//set digipath
	delay(10);								//same story as above
}

void RadioHandler::sendMessage(String message)
{
	_s.print(message + "\r\n");
} 

void RadioHandler::sendGPS(double longitude, double latitude)
{
	char lon[15];
	char lat[15];
	dtostrf(longitude,1,1,lon);
	dtostrf(latitude,1,1,lat);
	String message = String(lon) + "N/" + String(lat) + "W";
	sendMessage(message);
}

String RadioHandler::generateAPRSPacket()
{ 
  String APRSString="";
  
  char APRS_Lon[15];
  char APRS_Lat[15];
  char APRS_Course[15];
  char APRS_Speed[15];
  char APRS_Altitude[15];

  dtostrf(_tgps.location.lng(),1,1,APRS_Lon);
  dtostrf(_tgps.location.lat(),1,1,APRS_Lat);
  dtostrf(_tgps.course.deg(),1,1,APRS_Course);
  dtostrf(_tgps.speed.mps(),1,1,APRS_Speed);
  dtostrf(_tgps.altitude.meters(),1,1,APRS_Altitude);
  
  //format:   @DDHHMM/DDMM.hhN/DDDMM.hhW$CSE/SPD/comments...
  APRSString = "@" + _tgps.date.day();
  APRSString += _tgps.time.hour();
  APRSString += _tgps.time.minute();
  APRSString += "/";
  APRSString += APRS_Lon;
  APRSString += "N/";
  APRSString += APRS_Lat;
  APRSString +="W$";
  APRSString += APRS_Course;
  APRSString += "/";
  APRSString += APRS_Speed;
  APRSString += "/A";
  APRSString += APRS_Altitude;
  
  return APRSString;
}

String RadioHandler::sendPOSIT(){
	String packet = generateAPRSPacket();
	sendMessage(packet);
	
	return packet;
	
}

String RadioHandler::getVersion() {
	String version = "";
	sendMessage("V");
	delay(10);
	while(_s.available()){
		version += _s.read();
	}
	version += "\n";
	version = "Version: " + version;
	return version;
}

