// MotorController.h

#ifndef _MOTORCONTROLLER_h
#define _MOTORCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Conversion.h"
#include <mcp_can.h>

class MotorController {
public:
	MotorController(int cs, int baudrate);
	int ReadRPM();
	void ReadData(unsigned char request[8]);
	int ReturnRPM();

private:
	/*
	CAN BUS message headers
	*/
	// Retrieve RPM from Letrika Motor Controller
	unsigned char stmp60b[8] = { 0b01000000, 0x03, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00 };

	MCP_CAN *CAN;	// Chip select 11
	
	int RPM;
};


#endif

