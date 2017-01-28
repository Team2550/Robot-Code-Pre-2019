#ifndef LIFT_H
#define LIFT_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "Ports.h"
#include "Controls.h"

class Lift {
private:
    Joystick& driveController;
    Joystick& perifController;

	Spark liftMotor;
public:
	Lift(Joystick& _driveController, Joystick& _perifController);
	void RobotInit();
	void AutoInit();
	void AutoPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
};

#endif
