// Conversion.h

#ifndef _CONVERSION_h
#define _CONVERSION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Conversion {
public:
	static int ByteToInt16(unsigned char* b);
};



#endif

