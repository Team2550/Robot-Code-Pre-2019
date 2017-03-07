#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"
#include "Constants.h"

class DriveBase {
private:
    VictorSP leftMotor;
    VictorSP rightMotor;

    bool isReversed;
public:
    DriveBase();

    float getLeftSpeed();
    float getRightSpeed();
    void drive(float speed);
    void drive(float leftSpeed, float rightSpeed);
    void stop();

    void setReversed(bool reverse);
};

#endif
