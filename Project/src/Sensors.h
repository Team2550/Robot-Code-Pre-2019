/*
 * Ultrasonic.h
 *
 *  Created on: Jan 11, 2018
 *      Author: Admin
 */
#include <I2C.h>
#ifndef SRC_ULTRASONIC_H_
#define SRC_ULTRASONIC_H_

class sensors{
public:
	void openI2C(int deviceAddress)
	{
		I2C::I2C(deviceAddress);
	}
};




#endif /* SRC_ULTRASONIC_H_ */
