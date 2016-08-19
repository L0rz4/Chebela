// 
// 
// 

#include "MotorController.h"

/*
*	MotorController constructor
*	int cs - Chip select pin for CAN BUS shield
*	int baudrate - CAN BUS baud rate
*/
MotorController::MotorController(int cs, int baudrate)
{
	CAN = new MCP_CAN(cs);
	bool init_successfull = false;

	// Try to initialise the communication with CAN hardware
	// Repeat if init fails
	delay(100);
	do
	{
		int result = CAN->begin(MCP_ANY,baudrate, MCP_16MHZ);
		CAN->setMode(MCP_NORMAL);
		//Serial.println(result);
		if (CAN_OK == result)
		{
			init_successfull = true;
			Serial.println("CAN BUS Shield init ok!");
		}
		else
		{
			Serial.println("CAN BUS Shield init fail");
			Serial.println("Init CAN BUS Shield again");
			delay(500);
		}

	} while (!init_successfull);
}


// Reads RPM data from motor controller
int MotorController::ReadRPM() {
	CAN->sendMsgBuf(0x60b, 0, 8, (byte*)stmp60b);
	delay(100);
	if (CAN_MSGAVAIL == CAN->checkReceive())            // check if data coming
	{
		unsigned long id = 0;
		unsigned char len = 0;
		unsigned char buf[8];

		Serial.println("Message on CANM");
		CAN->readMsgBuf(&id, (byte*)&len, (byte*)buf);


		if (buf[1] == 0x03 && buf[2] == 0x20 && buf[3] == 0x01) {

			RPM = Conversion::ByteToInt16(&buf[4]);
//			Serial.print("Motor speed received: ");
			Serial.println("Received data:");
			Serial.print(buf[0], HEX); Serial.print("\t");
			Serial.print(buf[1], HEX); Serial.print("\t");
			Serial.print(buf[2], HEX); Serial.print("\t");
			Serial.print(buf[3], HEX); Serial.print("\t");
			Serial.println();
			Serial.print(buf[4], HEX); Serial.print("\t");
			Serial.print(buf[5], HEX); Serial.print("\t");
			Serial.print(buf[6], HEX); Serial.print("\t");
			Serial.print(buf[7], HEX); Serial.print("\t");

			Serial.println();

			// Indicates that the data has been read and stored to cache
			return 1;

			/*
			Serial.print("Hitrost motorja: ");
			Serial.print();
			Serial.println();
			Serial.print(buf[4], HEX);Serial.print("\t");
			Serial.print(buf[5], HEX);Serial.print("\t");
			Serial.print(buf[6], HEX);Serial.print("\t");
			Serial.print(buf[7], HEX);Serial.print("\t");

			Serial.println();
			*/
		}

		// Indicates that the data hasa been read, but the returned data does not have RPM value
		return -1;
	}

	// No data has been returned
	return -2;
}

// Returns cached RPM value
int MotorController::ReturnRPM() {
	return RPM;
}

void MotorController::ReadData(unsigned char request[8]) {

}
