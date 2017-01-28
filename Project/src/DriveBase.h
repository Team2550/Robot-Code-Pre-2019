#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "UDP-Receiver.h"
#include "Ports.h"
#include "Controls.h"

class DriveBase {
private:
    Joystick& driveController;
    Joystick& perifController;
    UDP_Receiver& udpReceiver;

    float maxSpeed;
    float maxBoostSpeed;
    VictorSP leftMotor;
    VictorSP rightMotor;
public:
    DriveBase(Joystick& _driveController, Joystick& _perifController,
    		  UDP_Receiver& _udpReceiver,
              float _maxSpeed, float _maxBoostSpeed);
    void RobotInit();
    void AutoInit();
    void AutoPeriodic();
    void TeleopInit();
    void TeleopPeriodic();

    void driveForward(float speed);
    void stop();
};

#endif
