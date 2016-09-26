/*
 Name:		ChebelaMCU.ino
 Created:	7/21/2016 9:12:47 AM
 Author:	Matej Lorenci
*/

#include <Wire.h>
#include "Communication.h"
#include <mcp_can.h>
#include "Conversion.h"
#include "MotorController.h"
#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>
#include <HardwareSerial.h>


#define RPM_TO_KMH_RATIO 76

MotorController *mc;

int number = 0;
int state = 0;


// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(115200);
	Serial.println("Serial communication up and running.");

	mc = new MotorController(53, CAN_250KBPS);

	pinMode(13, OUTPUT);

	// initialize i2c slave
	Wire.begin(i2c_SLAVE_ADDRESS);
	Wire.onReceive(receiveData);
	Wire.onRequest(sendData);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Serial.print("CAN read status: ");
	//Serial.print(mc->ReadRPM());
	//Serial.println();
	//delay(500);

	if (mc->ReadRPM() == 1) {
		number = mc->ReturnRPM() / RPM_TO_KMH_RATIO;
	}
	//number = analogRead(A3) / 10;	
}


// callback for received data
void receiveData(int byteCount) {
	while (Wire.available()) {
		number = Wire.read();
		Serial.print("data received : ");
		Serial.println(number); 

		if (number == 1) {

			if (state == 0) {
				digitalWrite(13, HIGH); // set the LED on
				state = 1;
			}
			else {
				digitalWrite(13, LOW); // set the LED off
				state = 0;
			}
		}
	}
}

// callback for sending data
void sendData() {
	//Serial.print("Current velocity: ");
	//Serial.println(number);
	/*
	Byte 0: 8-bit unsigned integer - velocity
	Byte 1: 8-bit unsigned integer - SOC
	Byte 2-5: 32-bit unsigned integer - kilometers driven
	Byte 6-9: 32-bit unsigned integer - motor temperature
	Byte 10-11: 16-bit unsigned integer - motor current
	Byte 12: 8-bit char - Lights
			Bit 0: Turn signal left
			Bit 1: Turn signal right
			Bit 2: Hazard lights
			Bit 3: Head lights - High Beam
	*/

	Wire.write(number);
}