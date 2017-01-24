#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase {
private:
	float maxSpeed;
	float maxBoostSpeed;
	VictorSP leftMotor;
	VictorSP rightMotor;
public:
	DriveBase(float _maxSpeed, float _maxBoostSpeed, int rightPort, int leftPort);
	void RobotInit();
	void AutoInit();
	void AutoPeriodic();
	void TeleopInit();
	void TeleopPeriodic(float leftSpeed, float rightSpeed, bool boost, bool autoaiming, int openCVData[]);

	void driveForward(float speed);
	void stop();
};

#endif
