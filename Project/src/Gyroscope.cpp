#include "Gyroscope.h";

Gyroscope::Gyroscope(){
	gyro(port);
	angle = gyro.GetAngle();

	void setTargetAngle()
	{
		targetAngle = angle;
	}

}

