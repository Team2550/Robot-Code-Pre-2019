/*
 * Ultrasonic.h
 *
 *  Created on: Jan 11, 2018
 *      Author: Brayton Kerekffy and Nathan Walker
 */
#ifndef ULTRASONIC_I2C_H_
#define ULTRASONIC_I2C_H_

#include <I2C.h>

class UltrasonicI2C
{
public:
	UltrasonicI2C(int sensorAddress);

	bool GetDistance(double& distance);

private:
	// Bytes output by I2C ultrasonic sensor
	const int READ_BYTE_COUNT = 2;

	I2C sensorI2C;

};

#endif
