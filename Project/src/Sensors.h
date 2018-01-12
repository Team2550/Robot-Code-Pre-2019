/*
 * Ultrasonic.h
 *
 *  Created on: Jan 11, 2018
 *      Author: Brayton Kerekffy
 */
#include <I2C.h>
#ifndef SRC_ULTRASONIC_H_
#define SRC_ULTRASONIC_H_

class sensors{
public:
	class ultrasonic{
	public:
	sensors::ultrasonic initialize;
	//definitions
	int deviceAddress = 1;
	bool addressOnly = true;
	int readCount = 10;
	unsigned char *readData;
	bool read = false;

	//defined device address to I2C ultrasonic sensor
	void openI2C()
	{
		//the I2C::Port is a preset bool. Only device address needs to be defined
		I2C(I2C::Port(1), deviceAddress);
	}
	void readUltrasonic(){
		I2C::ReadOnly(initialize.readCount, *readData);
	}
	//function that can be ran to automatically setup the ultrasonic system
	void initializeUltrasonic(){
		openI2C();

	}
	};
};




#endif /* SRC_ULTRASONIC_H_ */
