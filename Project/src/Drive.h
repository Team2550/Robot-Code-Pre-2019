#ifndef DRIVE_H
#define DRIVE_H

#include <WPILib.h>
#include "Utility.h"

class Drive {
private:
	Victor frontLeftMotor;
	Victor frontRightMotor;
	Victor backLeftMotor;
	Victor backRightMotor;
public:
	Drive(int frontLeftPort, int frontRightPort, int backLeftPort, int backRightPort);
	void autoInit();
	void autoPeriodic();
	void teleopInit();
	void teleopPeriodic(float speed, float rotation);
};

#endif
