#include "Arduino.h"
#include "SDHandler.h"
#include "String.h"

SDHandler::SDHandler()
{

}

void SDHandler::start(int CSPinNo)
{
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
  pinMode(10, OUTPUT);
   
  if (!SD.begin(CSPinNo)) {
    return;
  }
}

void SDHandler::openFile(char *filename)
{
	_openedFile = SD.open(filename, FILE_WRITE);
}

void SDHandler::writeString(String text)
{
  if (_openedFile) {
    _openedFile.println(text);
  }
} 

void SDHandler::writeTemperature(float temp_c, int32_t pressure)
{ 
  _openedFile.print(temp_c);
  _openedFile.print(F(","));
  _openedFile.println(pressure);
}

void SDHandler::writeTemperatureAndGPS(float temp_c, int32_t pressure, TinyGPSPlus& tgps)
{ 
  _openedFile.print(tgps.date.value());
  _openedFile.print(F(","));
  _openedFile.print(tgps.time.value());
  _openedFile.print(F(","));
  _openedFile.print(temp_c);
  _openedFile.print(F(","));
  _openedFile.print(pressure);
  _openedFile.print(F(","));
  _openedFile.print(tgps.location.lat());
  _openedFile.print(F(","));
  _openedFile.print(tgps.location.lng());
  _openedFile.print(F(","));
  _openedFile.print(tgps.speed.value());
  _openedFile.print(F(","));
  _openedFile.println(tgps.altitude.value());
}

/*void SDHandler::writeResetLine()
{
	_openedFile.println("SYSTEM RESET");
}*/

void SDHandler::close(){
   if (_openedFile) {
    _openedFile.close();
  }
}