/*
 * Ultrasonic.h
 *
 *  Created on: Jan 11, 2018
 *      Author: Brayton Kerekffy and Nathan Walker
 */

#include <I2C.h>
#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

class UltrasonicI2C
{
private:
	// Bytes output by I2C ultrasonic sensor
	const int READ_BYTE_COUNT = 10;

	I2C sensorI2C;

public:
	UltrasonicI2C(int sensorAddress);

	bool GetDistance(double& distance);
};

#endif
