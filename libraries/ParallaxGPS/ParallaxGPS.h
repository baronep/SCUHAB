#ifndef ParallaxGPS_h
#define ParallaxGPS_h

#include "Arduino.h"
#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

class ParallaxGPS
{
	public:
		ParallaxGPS(int rx, int tx,TinyGPSPlus& tgps);
		void start();
		void encode();
		String generateAPRSPacket();
	private:
		int _pin;
		SoftwareSerial _s;
		int _rx;
		int _tx;
		TinyGPSPlus& _tgps;

};


#endif
