// BMS.h

#ifndef _BMS_h
#define _BMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <mcp_can.h>

class BMS {
public: 
	BMS(int cs, int baudrate);
	int ReadBMS();
	int ReturnSOC();
	float ReturnVoltage();
	float ReturnCurrent();
	float ReturnTemperature();
	int ReturnSOH();
	float ReturnLowVoltage();
	float ReturnHighVoltage();
private:
	MCP_CAN *CAN;
	int SOC;
	unsigned char stmp31[8];
	unsigned char stmp32[8];

};

#endif

