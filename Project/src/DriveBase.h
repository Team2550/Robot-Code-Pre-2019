#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>

class DriveBase {
public:
	DriveBase(float _maxSpeed, float _maxBoostSpeed, int rightPort, int leftPort);
	void drive(float leftSpeed, float rightSpeed, bool boost);
	void driveForward(float speed);
	void stop();
private:
	const float maxSpeed;
	const float maxBoostSpeed;
	VictorSP leftMotor;
	VictorSP rightMotor;
};

#endif
