#ifndef RadioHandler_h
#define RadioHandler_h

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

class RadioHandler
{
	public:
		RadioHandler(int rx, int tx);
		void start(String callsign, String digipath);
		void sendMessage(String message); 
		void sendGPS(double longitude, double latitude);
		String generateAPRSPacket(TinyGPSPlus& tgps);
		String sendPOSIT(TinyGPSPlus& tgps);
	private:
		int _pin;
		SoftwareSerial _s;

};


#endif
