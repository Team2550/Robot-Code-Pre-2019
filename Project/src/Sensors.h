/*
 * Ultrasonic.h
 *
 *  Created on: Jan 11, 2018
 *      Author: Brayton Kerekffy
 */
#include <I2C.h>
#ifndef SRC_ULTRASONIC_H_
#define SRC_ULTRASONIC_H_

class Ultrasonic{
	public:
	Ultrasonic();
	int deviceAddress = 1;
	bool addressOnly = true;
	int readCount = 10;
	bool read = false;

	//Ultrasonic initialize;
	//definitions

};





#endif /* SRC_ULTRASONIC_H_ */
