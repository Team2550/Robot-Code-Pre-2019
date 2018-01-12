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
	//definitions
	int deviceAddress = 1;

	void openI2C()
	{
		I2C(I2C::Port(1), deviceAddress);
	}
	void initializeUltrasonic(){
		openI2C();
	}
};




#endif /* SRC_ULTRASONIC_H_ */
