#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "Ports.h"
#include "Controls.h"

class DriveBase {
private:
    Joystick& driveController;
    Joystick& perifController;

    float maxSpeed;
    float maxBoostSpeed;
    float maxTurtleSpeed;
    VictorSP leftMotor;
    VictorSP rightMotor;
public:
    DriveBase(Joystick& _driveController, Joystick& _perifController,
              float _maxSpeed, float _maxBoostSpeed, float _maxTurtleSpeed);
    void RobotInit();
    void AutoInit();
    void AutoPeriodic();
    void TeleopInit();
    void TeleopPeriodic();

    void driveForward(float speed);
    void stop();
};

#endif
