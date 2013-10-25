#ifndef SDHandler_h
#define SDHandler_h

#include <SD.h>
#include <TinyGPS++.h>

class SDHandler
{
	public:
		SDHandler();
		//init the library, pass CS Pin NO to function, for Sparkfun one, CS pin is 8
		void start(int CSPinNo);
		//Open a file to write, like openFile("data.log");
		void openFile(char *filename);
		//Write string to file
		void writeString(String text);
		//Write gps to file(newline)
		void writeGPS(TinyGPSPlus& tgps);
		//Write only temperature to file;
		void writeTemperature(float temp_c, int32_t pressure);
		//Write both temperature and gps to file.
		void writeTemperatureAndGPS(float temp_c, int32_t pressure, TinyGPSPlus& tgps);
		//you need to close the file after finish writing it.
		void close();
	private:
		File		 _openedFile;
};


#endif
