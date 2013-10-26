#include "Arduino.h"
#include "RadioHandler.h"
#include "String.h"
#include <SoftwareSerial.h>

RadioHandler::RadioHandler(int rx, int tx):
	_s(rx,tx)//, _tgps(tgps)
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

String RadioHandler::generateAPRSPacket(TinyGPSPlus& tgps)
{ 
  String APRSString="";
  
  char APRS_Lon[10];
  char APRS_Lat[10];
  char APRS_Course[10];
  char APRS_Speed[10];
  char APRS_Altitude[10];

  dtostrf(tgps.location.lng(),1,1,APRS_Lon);
  dtostrf(tgps.location.lat(),1,1,APRS_Lat);
  dtostrf(tgps.course.deg(),1,1,APRS_Course);
  dtostrf(tgps.speed.mps(),1,1,APRS_Speed);
  dtostrf(tgps.altitude.meters(),1,1,APRS_Altitude);
  
  //format:   @DDHHMM/DDMM.hhN/DDDMM.hhW$CSE/SPD/comments...
  APRSString = "!" + tgps.date.day();
  APRSString += tgps.time.hour();
  APRSString += tgps.time.minute();
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

String RadioHandler::sendPOSIT(TinyGPSPlus& tgps){
	String packet = generateAPRSPacket(tgps);
	sendMessage(packet);
	
	return packet;
	
}
