#include "Arduino.h"
#include "ParallaxGPS.h"
#include "String.h"
#include <SoftwareSerial.h>

ParallaxGPS::ParallaxGPS(int rx, int tx,TinyGPSPlus& tgps):
	_s(rx,tx), _tgps(tgps)
{
	_rx = rx;
	_tx = tx;
}

void ParallaxGPS::start()
{
	pinMode(_rx,INPUT);
	pinMode(_tx,OUTPUT);
	_s.begin(4800);

}

void ParallaxGPS::encode()
{
	while(_s.available())  //IF SERIAL DATA FROM THE GPS IS AVAILABLE
    {
      _tgps.encode(_s.read());  //SEND EACH CHARACTER TO THE GPS DECODER OBJECT
    }
}

