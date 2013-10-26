#include <TinyGPS++.h>

#include <SoftwareSerial.h>


SoftwareSerial s(2,3);
TinyGPSPlus gps;

void setup(){
  Serial.begin(9600);
  s.begin(4800);
  
}

void loop(){
  while (s.available() > 0){
    gps.encode(s.read());
  }
  
  Serial.println(gps.location.lat()); // Latitude in degrees (double)
  Serial.println(gps.location.lng());
  
}
