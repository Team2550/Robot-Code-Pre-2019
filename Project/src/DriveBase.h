#ifndef DRIVEBASE_H
#define DRIVEBASE_H

#include <WPILib.h>
#include <math.h>
#include "Utility.h"

class DriveBase {
private:
    float maxSpeed;
    float maxBoostSpeed;
    float maxTurtleSpeed;
    VictorSP leftMotor;
    VictorSP rightMotor;
public:
    DriveBase(int leftPort, int rightPort, float _maxSpeed, float _maxBoostSpeed, float _maxTurtleSpeed);

    void drive(float leftSide, float rightSide, bool boost, bool turtle);
    void driveForward();
    void driveBackwards();
    void turnLeft();
    void turnRight();
    void stop();
};

#endif
