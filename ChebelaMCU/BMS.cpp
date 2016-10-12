// 
// 
// 

#include "BMS.h"

/*
*	BMS constructor
*	int cs - Chip select pin for CAN BUS shield
*	int baudrate - CAN BUS baud rate
*/
BMS::BMS(int cs, int baudrate) {
	CAN = new MCP_CAN(cs);
	bool init_successfull = false;

	// Try to initialise the communication with CAN hardware
	// Repeat if init fails
	delay(100);
	do
	{
		int result = CAN->begin(MCP_ANY, baudrate, MCP_16MHZ);
		CAN->setMode(MCP_NORMAL);
		//Serial.println(result);
		if (CAN_OK == result)
		{
			init_successfull = true;
			Serial.println("CAN BUS for BMS Shield init ok!");
		}
		else
		{
			Serial.println("CAN BUS for BMS Shield init fail");
			Serial.println("Init CAN BUS Shield again");
			delay(500);
		}

	} while (!init_successfull);
}

int BMS::ReadBMS() {
	if (CAN_MSGAVAIL == CAN->checkReceive())            // check if data coming
	{
		unsigned long id = 0;
		unsigned char len = 0;
		unsigned char buf[8];

		Serial.println("Message on CAN BMS");
		CAN->readMsgBuf(&id, (byte*)&len, (byte*)buf);

		if (id == 0x031) {
			stmp31[0] = buf[0];
			stmp31[1] = buf[1];
			stmp31[2] = buf[2];
			stmp31[3] = buf[3];
			stmp31[4] = buf[4];
			stmp31[5] = buf[5];
			stmp31[6] = buf[6];
			stmp31[7] = buf[7];
		}
		else if (id == 0x032) {
			stmp32[0] = buf[0];
			stmp32[1] = buf[1];
			stmp32[2] = buf[2];
			stmp32[3] = buf[3];
			stmp32[4] = buf[4];
			stmp32[5] = buf[5];
			stmp32[6] = buf[6];
			stmp32[7] = buf[7];
		}
		// Indicates that the data has been read and stored to cache
		return 1;
	}

	// No data has been returned
	return -2;
}

int BMS::ReturnSOC() {
	return stmp31[0] / 2;
}

float BMS::ReturnVoltage() {
	unsigned int battery_voltage = (stmp31[1] << 8) | stmp31[2]; // mV
	return battery_voltage / 1000;
}

float BMS::ReturnCurrent() {
	signed int battery_current = (stmp31[3] << 8) | stmp31[4];
	battery_current = battery_current * 10; //mA
	return battery_current / 1000;
}

float BMS::ReturnTemperature() {
	return (float)stmp31[5];
}

int BMS::ReturnSOH() {
	return stmp32[0] / 2;
}
float BMS::ReturnLowVoltage() {
	unsigned int low_cell_voltage = (stmp32[1] << 8) | stmp32[2]; // mV
	return low_cell_voltage / 1000;
}

float BMS::ReturnHighVoltage() {
	unsigned int high_cell_voltage = (stmp32[3] << 8) | stmp32[4]; // mV
	return high_cell_voltage / 1000; 
}