/*
 Name:		ChebelaMCU.ino
 Created:	7/21/2016 9:12:47 AM
 Author:	Matej Lorenci
*/

#include <mcp_can.h>
#include "Conversion.h"
#include "MotorController.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include <HardwareSerial.h>

MotorController *mc;

// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(115200);
	Serial.println("Serial communication up and running.");

	mc = new MotorController(53, CAN_250KBPS);
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.print("CAN read status: ");
	Serial.print(mc->ReadRPM());
	Serial.println();
	delay(500);

}
