/*
 Name:		ChebelaMCU.ino
 Created:	7/21/2016 9:12:47 AM
 Author:	Matej Lorenci
*/

#include "BMS.h"
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
BMS *bms;

int number = 0;
int state = 0;

int driving_direction = 1;
long last_driving_direction = 0;


// the setup function runs once when you press reset or power the board
void setup() {

	Serial.begin(115200);
	Serial.println("Serial communication up and running.");

	pinMode(49, OUTPUT);

	mc = new MotorController(53, CAN_250KBPS);
	bms = new BMS(49, CAN_1000KBPS);

	pinMode(13, OUTPUT);
	pinMode(24, OUTPUT);
	pinMode(25, OUTPUT);
	pinMode(22, INPUT_PULLUP);
	pinMode(23, INPUT_PULLUP);


	// initialize i2c slave
	Wire.begin(i2c_SLAVE_ADDRESS);
	Wire.onRequest(sendData);
}

// the loop function runs over and over again until power down or reset
void loop() {
	//Serial.print("CAN read status: ");
	//Serial.print(mc->ReadRPM());
	//Serial.println();
	delay(500);

	if (mc->ReadRPM() == 1) {
		number = mc->ReturnRPM() / RPM_TO_KMH_RATIO;
		Serial.print("Vehicle velocity: ");
		Serial.println(number);
	}
	if (bms->ReadBMS() == 1) {
		Serial.print("SOC: ");
		Serial.println(bms->ReturnSOC());

	}
	//number = analogRead(A3) / 10;	
	if (digitalRead(22) == LOW) {
		if (millis() - last_driving_direction > 1000) {
			last_driving_direction = millis();
			driving_direction++;
			if (driving_direction > 2) {
				driving_direction = 2;
			}
		}
	}
	else if (digitalRead(23) == LOW) {
		if (millis() - last_driving_direction > 1000) {
			last_driving_direction = millis();
			driving_direction--;
			if (driving_direction < 0) {
				driving_direction = 0;
			}
		}
	}

	if (driving_direction == 2) {
		digitalWrite(24, LOW);
		digitalWrite(25, HIGH);
	}
	else if (driving_direction == 0) {
		digitalWrite(24, HIGH);
		digitalWrite(25, LOW);
	}
	else {
		digitalWrite(24, HIGH);
		digitalWrite(25, HIGH);
	}
	Serial.print("Driving direction: ");
	Serial.println(driving_direction);
	

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

	char tmp[12] = { (char)number , (char)bms->ReturnSOC(), (char)driving_direction };

	//Wire.write((char)number);
	//Wire.write((char)bms->ReturnSOC());
	//Wire.write((char)driving_direction);
	Wire.write(tmp, 12);
	/*
	Serial.println("Sent data:");
	Serial.print(tmp[0], HEX); Serial.print("\t");
	Serial.print(tmp[1], HEX); Serial.print("\t");
	Serial.print(tmp[2], HEX); Serial.print("\t");
	Serial.print(tmp[3], HEX); Serial.print("\t");
	Serial.println();
	Serial.print(tmp[4], HEX); Serial.print("\t");
	Serial.print(tmp[5], HEX); Serial.print("\t");
	Serial.print(tmp[6], HEX); Serial.print("\t");
	Serial.print(tmp[7], HEX); Serial.print("\t");
	Serial.println();
	*/
}