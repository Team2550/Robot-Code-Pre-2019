/*
 * Gyroscope.h
 *
 *  Created on: Feb 05, 2018
 *      Author: Brayton Kerekffy
 */

#include <WPILib.h>

class Gyroscope
{
	AnalogGyro gyro;
	//static const float kP = 0.03;

public:
	Gyroscope();
	int port;
	float targetAngle;
	float angle;
};
